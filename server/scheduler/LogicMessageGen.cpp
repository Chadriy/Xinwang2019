#include "Logic.h"
#include "../const.h"
#include "../GlobalUtil.h"
#include "../conn/Connection.h"
#include "../topology/Channel.h"
#include "MessageScheduler.h"

void Logic::MessageGen::sendBuildMessage(int channelId,
                                        int sourceId, int targetId, int state,
                                        int errCode, int channelType) {
    Json result = Const::EMPTY_MESSAGE;
    result["callType"] = Const::CALL_TYPE_CHANNEL_BUILD;
    result["channelId"] = channelId;
    result["channelType"] = channelType;
    result["state"] = state;
    result["errCode"] = errCode;
    result["targetId"] = targetId;
    result["sysMessage"]["target"] = sourceId;
    float buildTime = (float) Channel::getConfig(channelType)["buildTime"];
    result["timestamp"] = GlobalUtil::curTime() + buildTime / 2;
    conn::scheduler.addMessage(result);
}

void Logic::MessageGen::sendDestroyMessage(int *port, int channelId) {
    Json result = Const::EMPTY_MESSAGE;
    result["callType"] = Const::CALL_TYPE_CHANNEL_DESTROY;
    result["channelId"] = channelId;
    result["state"] = Const::STATE_NOTICE;

    result["targetId"] = port[0];

    conn::send(result);
    result["targetId"] = port[1];
    conn::send(result);
}
