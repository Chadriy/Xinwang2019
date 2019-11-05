#include "MessageScheduler.h"
#include "../GlobalUtil.h"
#include "../const.h"
#include "Logic.h"
#include "../conn/Connection.h"
#include "../Exception/Exception.h"

MessageScheduler::MessageScheduler() {
    network = new Network();
}

void MessageScheduler::addMessage(Json message) {
    messageQueue.push(message);
    if (message["targetId"] == 0) {
        //Print("fuck!");
    }
}

void MessageScheduler::onMessageRecv(Json message, int sourceId) {
    message["sourceId"] = sourceId;

    if (!conn::judger->afterRecvingCheck(message)) {
        return ;
    }

    std::string callType = message["callType"];
    if (callType == Const::CALL_TYPE_CHANNEL_BUILD) {
        try{
            Logic::ChannelOp::build(message);
        }catch (Exception *e){
            conn::send(e->dump());
            delete e;
        }
    } else if (callType == Const::CALL_TYPE_CHANNEL_DESTROY) {
        try{
            //Print("on destroy start")
            Channel *channel = getNetwork().getChannel(message["channelId"]);
            conn::judger->onChannelDestroy(channel, message);
            //Print("deal judger succ")
            Logic::ChannelOp::destroy(message);
        }catch (Exception *e){
            conn::send(e->dump());
            delete e;
        }
    } else if ( callType == Const::CALL_TYPE_PREPARE ||
                callType == Const::CALL_TYPE_SEND ||
                callType == Const::CALL_TYPE_SYS) {
        try {
            Channel *channel = getNetwork().getChannel(message["channelId"]);
            conn::judger->onChannelUsing(channel, message);
            conn::judger->onMessageRecv(message);
            Logic::route(message);
        }catch (Exception *e){
            conn::send(e->dump());
            delete e;
        }
    } else {
        conn::send(Logic::toErrMessage(message, Const::ERR_CODE_NO_SUCH_CALL_TYPE));
    }
}

void MessageScheduler::onMessageSend(Json message, int targetId) {
    std::string callType = message["callType"];
    if ( callType == Const::CALL_TYPE_PREPARE ||
         callType == Const::CALL_TYPE_SEND ||
         callType == Const::CALL_TYPE_SYS) {
        conn::judger->onMessageSend(message);
    }
}

void MessageScheduler::tick() {
    while (!messageQueue.empty() && (float)messageQueue.top()["timestamp"] < GlobalUtil::curTime()) {
        ////Print("tick sending... %s", messageQueue.top().dump().data())
        conn::send(messageQueue.top());
        //Print("poping...")
        messageQueue.pop();
        ////Print("next msg: %s", messageQueue.top().dump().data());
    }
}

