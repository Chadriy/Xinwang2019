#pragma once

#include "conn/Channel.h"
#include "Action.h"

class Action;

class Scheduler {
public:
    Scheduler(Channel* channel);
    int getId();
    void onRecv(Json message);
    void onPrepare(Json message);
    void onSend(Json message);
    void onSys(Json message) {}
    void onChannelBuild(Json message);
    void onDestroy(Json message);
    void sendChannelBuild(int target, int state, int errCode, int channelType);
    void doSend(Json message, int target);
    Channel *getChannel();
private:
    Channel *channel;
    std::vector<Action*> action;
    std::map<int, int> channelMap;
};


