#include <iostream>
#include <fstream>

#include "src/Scheduler.h"
#include "src/conn/GeneralChannel.h"
#include "src/Const.h"

int main() {
    std::ifstream fcfg("../client.json");
    Json config; fcfg>>config;
    fcfg.close();
    GeneralChannel *channel = new GeneralChannel();
    channel->initConfig(config);
    Scheduler scheduler(channel);
    while (true) {
        std::vector<Json> message = channel->recv();
        for (Json msg:message) {
            scheduler.onRecv(msg);
        }
        Sleep(50);
    }
    return 0;
}