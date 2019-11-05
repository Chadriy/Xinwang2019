#pragma once

#include "../../extra/socket/Socket.h"
#include "Channel.h"

class GeneralChannel : public Channel{
public:
    void initConfig(Json config);
    void send(Json message, int target);
    std::vector<Json> recv();
    int getId();
    Json getConfig(std::vector<std::string> &keys);

private:
    SocketClient *client;
    Json config;
};


