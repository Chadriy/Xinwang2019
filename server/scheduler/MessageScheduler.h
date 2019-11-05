#pragma once
#include <queue>
#include "../extra/json.hpp"
#include "../topology/Network.h"
#include "../GlobalUtil.h"

class Network;
using Json = nlohmann::json;

class JsonTimeCmp {
public:
    bool operator()(Json a, Json b) {
        return ((float) a["timestamp"]) > ((float) b["timestamp"]);
    }
};


class MessageScheduler {
private:
    std::priority_queue<Json, std::vector<Json>, JsonTimeCmp> messageQueue;
    Network *network;
public:
    MessageScheduler();
    void addMessage(Json message);
    void onMessageSend(Json message, int targetId);
    void onMessageRecv(Json message, int sourceId);
    void tick();
    Network& getNetwork() { return *network; }
};

