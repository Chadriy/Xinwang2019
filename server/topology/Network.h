#pragma once

#include <map>
#include <set>
#include <queue>
#include "Channel.h"
#include "PrepareChannel.h"

class Channel;

class Network {
private:
    std::map<int, Channel*> channelMap;
    //std::map<int, std::set<int> > preparing;
    std::priority_queue<float, std::vector<float>, std::less<float> > preparing[1001][1001];
    std::priority_queue<PrepareChannel*> preparingHeap;
public:
    void prepareToBuild(Json message);
    int recvPrepareMessage(Json message);
    void deletePrepareMessage(Json message);
    void deletePrepareMessage(int source, int target);
    int addChannel(Json message);
    void destroyChannel(Json message);
    void tick();
    void sendTimeoutMessage(PrepareChannel * channel);
    Channel* getChannel(int channelId);
};
