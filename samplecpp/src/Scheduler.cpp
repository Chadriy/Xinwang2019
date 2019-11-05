#include "Scheduler.h"
#include "Const.h"

Scheduler::Scheduler(Channel *channel) {
    this->channel = channel;
    std::vector<std::string> args {Const::main_config, Const::node_count};
    int n = channel->getConfig(args);
    action.resize(n + 1);
    for (int i=1 ; i<=n ; i++) {
        action[i] = new Action(i, this);
    }
    channelMap.clear();
}

int Scheduler::getId() {
    return getChannel()->getId();
}

void Scheduler::onRecv(Json message) {
    if (message[Const::err_code] != Const::ERR_CODE_NONE){
        return ;
    }
    std::string call = message[Const::call_type];
    if (call == Const::CALL_TYPE_PREPARE) onPrepare(message);
    else if (call == Const::CALL_TYPE_SEND) onSend(message);
    else if (call == Const::CALL_TYPE_SYS) onSys(message);
    else if (call == Const::CALL_TYPE_CHANNEL_BUILD) onChannelBuild(message);
    else if (call == Const::CALL_TYPE_CHANNEL_DESTROY) onDestroy(message);
}

void Scheduler::onPrepare(Json message) {
    Print("prepare");
    action[message[Const::sys_message][Const::target]]->onPrepare(message);
}

void Scheduler::onSend(Json message) {
    Print("send")
    action[message[Const::sys_message][Const::target]]->onSend(message);
}

void Scheduler::onChannelBuild(Json message) {
    Print("build")
    int target = message[Const::sys_message][Const::target];
    if (message[Const::channel_id] != 0) {
        Print("succ")
        action[target]->onSucc(message);
        channelMap[message[Const::channel_id]] = target;
    } else {
        int state = message[Const::state];
        if (state == Const::STATE_NOTICE) {
            Print("request")
            action[target]->onRequest(message);
        } else if (state == Const::STATE_REFUSE) {
            Print("refuse")
            action[target]->onRefuse(message);
        }
    }
}

void Scheduler::onDestroy(Json message) {
    Print("destroy");
    int channelId = message[Const::channel_id];
    auto target = channelMap.find(channelId);
    if (target != channelMap.end()) {
        action[target->second]->onDestroy(message);
        channelMap.erase(target);
    }
}

void Scheduler::sendChannelBuild(int target, int state, int errCode, int channelType) {
    Json msg = Const::empty_message;
    msg[Const::call_type] = Const::CALL_TYPE_CHANNEL_BUILD;
    msg[Const::state] = state;
    msg[Const::sys_message][Const::target] = target;
    msg[Const::sys_message][Const::data] = "";
    msg[Const::sys_message][Const::delay] = 0;
    msg[Const::err_code] = errCode;
    msg[Const::channel_type] = channelType;
    doSend(msg, 0);
}

void Scheduler::doSend(Json message, int target) {
    channel->send(message, target);
}

Channel* Scheduler::getChannel() {
    return channel;
}