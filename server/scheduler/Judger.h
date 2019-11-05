#pragma once

#include "../extra/json.hpp"
#include "../Exception/Exception.h"
#include "../topology/Channel.h"

class Channel;
using Json = nlohmann::json;

class Judger {
public:
    Judger(Json config);
    void onChannelPrepare(Json message);
    void onChannelBuild(Json message, Channel *channel);
    void onChannelDestroy(Channel *channel, Json message);
    void onMessageRecv(Json message);
    void onMessageSend(Json message);
    void onChannelUsing(Channel *channel, Json message);
    bool beforeSendingCheck(Json message);
    bool afterRecvingCheck(Json message);
private:
    int checkChannelCount(int channelType, int *port);
    int *channelCount, highSpeed, lowSpeed, highMessage, lowMessage;
    std::map<int, int> messageCount;
};
