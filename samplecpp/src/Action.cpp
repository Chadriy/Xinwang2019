#include "Action.h"
#include "Const.h"

Action::Action(int target, Scheduler* scheduler) {
    this->target = target;
    this->scheduler = scheduler;

    timeout = 0;
    waiting_count = 0;
    queue.clear();
    clearChannelInfo();
    Print("channel type:%d", channelType)
}

void Action::clearChannelInfo() {
    channelType = Const::CHANNEL_TYPE_ERROR;

    channelState = Const::CHANNEL_STATE_NONE;
    channelId = 0;
}

bool Action::isValid() {
    Print("check valid %d %d", channelType, channelState)
    return (channelType != Const::CHANNEL_TYPE_ERROR) && (channelState == Const::CHANNEL_STATE_SUCCESS);
}

int Action::getOtherType() {
    return channelType == Const::CHANNEL_TYPE_NORMAL ? Const::CHANNEL_TYPE_FAST : Const::CHANNEL_TYPE_NORMAL;
}

void Action::doRequest(int channelType) {
    if (this->channelType == Const::CHANNEL_TYPE_ERROR) {
        this->channelType == channelType;
        channelState = Const::STATE_REQUEST;
        scheduler->sendChannelBuild(target, Const::STATE_REQUEST, Const::ERR_CODE_NONE, channelType);
    }
}

void Action::onRequest(Json message) {
    int target = message[Const::sys_message][Const::target];
    int cType = message[Const::channel_type];
    if (channelState != Const::ERR_CODE_NONE && scheduler->getId() < target) {
        scheduler->sendChannelBuild(target, Const::STATE_REFUSE, Const::ERR_CODE_CHANNEL_BUILD_TARGET_REFUSE, cType);
        return;
    }
    scheduler->sendChannelBuild(target, Const::STATE_ACCEPT, Const::ERR_CODE_NONE, cType);
    channelType = cType;
    channelState = Const::CHANNEL_STATE_ACCEPT;
}

void Action::onSucc(Json message) {
    channelType = message[Const::channel_type];
    channelState = Const::CHANNEL_STATE_SUCCESS;
    channelId = message[Const::channel_id];
    filterQueue();
    Print("after filter queue [%d]", queue.size())
    for (Json msg: queue){
        doSend(msg);
    }
    queue.clear();
}

void Action::onRefuse(Json message) {
    if (channelState != Const::CHANNEL_STATE_SUCCESS) {
        clearChannelInfo();
        filterQueue();
        if (waiting_count > 0 || queue.size()) {
            doRequest(getOtherType());
        }
    }
}

void Action::onDestroy(Json message) {
    if (channelState == Const::CHANNEL_STATE_SUCCESS) {
        clearChannelInfo();
    }
    filterQueue();
    if (waiting_count > 0 || queue.size()) {
        doRequest(getOtherType());
    }
}

void Action::onPrepare(Json message) {
    std::vector<std::string> args {Const::main_config, Const::timeout};
    timeout = std::max(timeout, Const::curTime() + (double) (scheduler->getChannel()->getConfig(args)));
    waiting_count++;
    Print("channelstate %d", channelState)
    if (channelState == Const::CHANNEL_STATE_NONE) {
        doRequest(getOtherType());
    }
}

void Action::onSend(Json message) {
    if (scheduler->getId() == (int) message[Const::sys_message][Const::target]) {
        Print("succ received message: %s", message[Const::sys_message][Const::data].dump().data());
        return ;
    }
    waiting_count--;
    if (isValid()) {
        Print("do send")
        doSend(message);
    } else {
        Print("push back")
        queue.push_back(message);
    }
}

void Action::doSend(Json message) {
    if ((float) message[Const::recv_time] >= getTimeLimit()) {
        message[Const::channel_id] = channelId;
        scheduler->doSend(message, message[Const::sys_message][Const::target]);
    }
}

void Action::filterQueue() {
    double limit = getTimeLimit();
    while (!queue.empty()) {
        Json msg = queue.front();
        if ((float) msg[Const::recv_time] < limit){
            queue.pop_front();
        } else {
            break;
        }
    }
}

double Action::getTimeLimit() {
    Json channelConfig = getConfig();
    double lag = channelConfig.empty() ? 0 : (double)channelConfig[Const::lag];
    std::vector<std::string> args {Const::main_config, Const::timeout};
    double timeout = (double) scheduler->getChannel()->getConfig(args);
    return Const::curTime() + lag - timeout;
}

Json Action::getConfig() {
    std::vector<std::string> args;
    if (channelType == Const::CHANNEL_TYPE_FAST) {
        args.push_back(Const::channel_config);
        args.push_back(Const::high_speed);
    } else if (channelType == Const::CHANNEL_TYPE_NORMAL) {
        args.push_back(Const::channel_config);
        args.push_back(Const::normal_speed);
    } else {
        return "{}"_json;
    }
    return scheduler->getChannel()->getConfig(args);
}
