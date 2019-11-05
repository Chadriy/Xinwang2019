#include "Logic.h"
#include "../GlobalUtil.h"
#include "../const.h"
#include "../conn/Connection.h"
#include "../topology/Network.h"
#include "../topology/Channel.h"
#include "../Exception/Exception.h"

Json Logic::beforeSend(Json message) {
    const std::string validKey[] = {
            "callType",
            "channelId",
            "sysMessage",
            "extMessage",
            "state",
            "errCode",
            "channelType"
    };
    Json result;
    for (std::string key : validKey) {
        result[key] = message[key];
    }
    return result;
}

Json Logic::afterRecv(Json message) {
    message["recvTime"] = GlobalUtil::curTime();
    return std::move(message);
}

Json Logic::toErrMessage(Json message, int errCode) {
    message["state"] = Const::STATE_REFUSE;
    message["errCode"] = errCode;
    message["targetId"] = message["sourceId"];
    return message;
}

void Logic::route(Json message) {
    Network &network = conn::scheduler.getNetwork();
    Channel *channel = network.getChannel(message["channelId"]);
    if (channel == nullptr) {
        throw new Exception(message, Const::ERR_CODE_NO_SUCH_CHANNEL);
    } else {
        message["timestamp"] = GlobalUtil::curTime() + (float)(channel->getConfig()["lag"]);
        conn::scheduler.addMessage(message);
    }
}
