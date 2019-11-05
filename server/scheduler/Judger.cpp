#include "Judger.h"
#include "../const.h"
#include "../conn/Connection.h"
#include "Logic.h"

Judger::Judger(Json config):
        highSpeed((int) config["channelConfig"]["highSpeed"]["maxCount"]),
        lowSpeed((int) config["channelConfig"]["normalSpeed"]["maxCount"]),
        highMessage((int) config["channelConfig"]["highSpeed"]["maxMessageCount"]),
        lowMessage((int) config["channelConfig"]["normalSpeed"]["maxMessageCount"]) {
    int N = config["mainConfig"]["nodeCount"];
    channelCount = new int[N + 1];
    for (int i = 0; i < N; i++) {
        channelCount[i+1] = config["mainConfig"]["maxChannelCount"][i];
    }
}

int Judger::checkChannelCount(int channelType, int *port) {
    if (channelType == Const::CHANNEL_TYPE_FAST && highSpeed <=0) {
        return Const::ERR_CODE_CHANNEL_BUILD_TOTAL_LIMIT;
    } else if (channelType == Const::CHANNEL_TYPE_NORMAL && lowSpeed <=0) {
        return Const::ERR_CODE_CHANNEL_BUILD_TOTAL_LIMIT;
    } else if (channelCount[port[0]] <=0) {
        return Const::ERR_CODE_CHANNEL_BUILD_SOURCE_LIMIT;
    } else if (channelCount[port[1]] <= 0) {
        return Const::ERR_CODE_CHANNEL_BUILD_TARGET_LIMIT;
    }
    return Const::ERR_CODE_NONE;
}

void Judger::onChannelPrepare(Json message) {
    int target = message["sysMessage"]["target"], source = message["sourceId"];
    int port[] = {source, target};
    int errCode = checkChannelCount(message["channelType"], port);
    if (errCode) {
        throw new Exception(message, errCode);
    }
}

void Judger::onChannelBuild(Json message, Channel *channel) {
    int target = message["sysMessage"]["target"], source = message["sourceId"];
    int port[] = {source, target};
    int channelType = message["channelType"];
    int errCode = checkChannelCount(channelType, port);
    if (errCode) {
        throw new Exception(message, errCode);
    }
    if (channelType == Const::CHANNEL_TYPE_FAST) {
        --highSpeed;
        messageCount[channel->getId()] = highMessage;
    } else {
        --lowSpeed;
        messageCount[channel->getId()] = lowMessage;
    }
    --channelCount[port[0]];
    --channelCount[port[1]];
}

void Judger::onChannelDestroy(Channel * channel, Json message) {
    if (!channel) {
        throw new Exception(message, Const::ERR_CODE_NO_SUCH_CHANNEL);
    }
    int sourceId = message["sourceId"];
    if (channel->getPort()[0] != sourceId && channel->getPort()[1] != sourceId) {
        throw new Exception(message, Const::ERR_CODE_PORT_NOT_VALID);
    }
    int type = channel->getType();
    if (type == Const::CHANNEL_TYPE_FAST) {
        ++highSpeed;
    } else if (type == Const::CHANNEL_TYPE_NORMAL) {
        ++lowSpeed;
    }
    messageCount.erase(channel->getId());
    ++channelCount[channel->getPort()[0]];
    ++channelCount[channel->getPort()[1]];
}

void Judger::onMessageRecv(Json message) {
    int channelId = message["channelId"];
    if (messageCount[channelId] <= 0) {
        throw new Exception(message, Const::ERR_CODE_SEND_COUNT_LIMIT);
    }
    int maxSize = 0;
    if (message["callType"] == Const::CALL_TYPE_PREPARE) {
        maxSize = conn::Config["messageConfig"]["maxPendingMsgSize"];
    } else if (message["callType"] == Const::CALL_TYPE_SEND) {
        maxSize = conn::Config["messageConfig"]["maxCallMsgSize"];
    } else if (message["callType"] == Const::CALL_TYPE_SYS) {
        maxSize = conn::Config["messageConfig"]["maxInnerMsgSize"];
    }
    if (message["extMessage"].dump().size() > maxSize) {
        throw new Exception(message, Const::ERR_CODE_SEND_SIZE_LIMIT);
    }
    --messageCount[channelId];
}

void Judger::onMessageSend(Json message) {
    ++messageCount[message["channelId"]];
}

void Judger::onChannelUsing(Channel *channel, Json message) {
    if (!channel) {
        throw new Exception(message, Const::ERR_CODE_NO_SUCH_CHANNEL);
    }
    int *port = channel->getPort();
    int s = message["sourceId"], t = message["targetId"];
    if (!((s == port[0] && t == port[1]) || (s == port[1] && t == port[0]))) {
        throw new Exception(message, Const::ERR_CODE_PORT_NOT_VALID);
    }
}

bool Judger::beforeSendingCheck(Json message) {
    if (message["state"] == Const::STATE_REFUSE) return true;
    int channelId = message["channelId"];
    if ((message["callType"] == Const::CALL_TYPE_PREPARE && channelId != 0) ||
        message["callType"] == Const::CALL_TYPE_SEND ||
        message["callType"] == Const::CALL_TYPE_SYS) {
        if (messageCount.find(channelId) == messageCount.end())
        {
            message["targetId"] = message["sourceId"];
            conn::send(Logic::toErrMessage(message, Const::ERR_CODE_NO_SUCH_CHANNEL));
            return false;
        }
    }
    return true;
}

bool Judger::afterRecvingCheck(Json message) {
    if (message["state"] == Const::STATE_REFUSE) return true;
    int channelId = message["channelId"];
    if (message["callType"] == Const::CALL_TYPE_PREPARE ||
        message["callType"] == Const::CALL_TYPE_SEND ||
        message["callType"] == Const::CALL_TYPE_SYS) {
        if (messageCount.find(channelId) == messageCount.end())
        {
            message["targetId"] = message["sourceId"];
            conn::send(Logic::toErrMessage(message, Const::ERR_CODE_NO_SUCH_CHANNEL));
            return false;
        }
    }
    return true;
}
