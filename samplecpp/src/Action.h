#pragma once

#include <deque>
#include "Scheduler.h"
#include "../extra/json.hpp"

class Scheduler;
using Json = nlohmann::json;

class Action {
public:
    Action(int target, Scheduler* scheduler);
    bool isValid();
    int getOtherType();
    void doRequest(int channelType);
    void onRequest(Json message);
    void onSucc(Json message);
    void onRefuse(Json message);
    void onDestroy(Json message);
    void onPrepare(Json message);
    void onSend(Json message);
    void doSend(Json message);

private:
    int target;
    Scheduler *scheduler;
    double timeout;
    int waiting_count;
    std::deque<Json> queue;

    int channelType, channelState, channelId;

    void clearChannelInfo();
    void filterQueue();
    double getTimeLimit();
    Json getConfig();
};


