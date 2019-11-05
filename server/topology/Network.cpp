#include "Network.h"
#include "../const.h"
#include "../Exception/Exception.h"
#include "../conn/Connection.h"

int Network::addChannel(Json message) {
    Channel *channel = new Channel (message);
    conn::judger->onChannelBuild(message, channel);
    channelMap[channel->getId()] = channel;
    return channel->getId();
}

void Network::destroyChannel(Json message) {
    int channelId = message["channelId"];
    Channel *channel = channelMap[channelId];
    channelMap.erase(channelId);
    delete channel;
}

Channel* Network::getChannel(int channelId) {
    auto itr = channelMap.find(channelId);
    return itr == channelMap.end() ? nullptr : itr->second;
}

void Network::prepareToBuild(Json message) {
    int target = message["sysMessage"]["target"], source = message["sourceId"];
    float buildTime = (float) Channel::getConfig(message["channelType"])["buildTime"];
    preparing[source][target].push(GlobalUtil::curTime() + buildTime);
    preparingHeap.push(new PrepareChannel(source, target, buildTime));
}

int Network::recvPrepareMessage(Json message) {
    deletePrepareMessage(message);
    return addChannel(message);
}

void Network::deletePrepareMessage(Json message) {
    int source = message["sysMessage"]["target"], target = message["sourceId"];
    try {
        deletePrepareMessage(source, target);
    } catch (Exception *e) {
        e->setMessage(message);
        throw e;
    }

}

void Network::deletePrepareMessage(int source, int target) {
    if (preparing[source][target].empty()) {
        throw new Exception("{}"_json, Const::ERR_CODE_NO_SUCH_CHANNEL);
    } else {
        preparing[source][target].pop();
    }
}

void Network::tick() {
    while (!preparingHeap.empty() && preparingHeap.top()->isTimeout()) {
        PrepareChannel *channel = preparingHeap.top();
        preparingHeap.pop();
        std::priority_queue<float> &heap = preparing[channel->getSource()][channel->getTarget()];
        if (!heap.empty() && heap.top() <= channel->getTimeout() + Const::EXP) {
            deletePrepareMessage(channel->getSource(), channel->getTarget());
            sendTimeoutMessage(channel);
        }
        delete channel;
    }
}

void Network::sendTimeoutMessage(PrepareChannel *channel) {
    Json message = Const::EMPTY_MESSAGE;
    message["callType"] = Const::CALL_TYPE_CHANNEL_BUILD;
    message["sysMessage"]["target"] = channel->getTarget();
    message["state"] = Const::STATE_REFUSE;
    message["errCode"] = Const::ERR_CODE_CHANNEL_BUILD_TARGET_TIMEOUT;
    message["targetId"] = channel->getSource();
    conn::send(message);
}