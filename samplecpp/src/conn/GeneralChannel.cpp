#include "GeneralChannel.h"
#include "../Const.h"

void GeneralChannel::initConfig(Json config) {
    this->config = config;

    client = new SocketClient(
            config[Const::main_config][Const::master_ip],
            (int) config[Const::main_config][Const::master_port],
            NonBlockingSocket);


    while (true) {
        std::string line = client->ReceiveLine();
        if (line.size()) {
            Print("id recved: %s", line.data());
            Json exConfig = Json::parse(line);
            this->config[Const::index] = exConfig[Const::index];
            this->config[Const::max_channel_conn] = exConfig[Const::max_channel_conn];
            break;
        } else {
            Sleep(50);
        }
    }
    Print("init finish.")
}

void GeneralChannel::send(Json message, int target) {
    message[Const::target_id] = target;
    Print("msg send: %s", message.dump().data());
    client->SendLine(message.dump());
}

std::vector<Json> GeneralChannel::recv() {
    std::vector<Json> result;
    while (true) {
        std::string line = client->ReceiveLine();
        if (line.size()) {
            Print("msg recv: %s", line.data());
            Json message = Json::parse(line);
            message[Const::recv_time] = Const::curTime();
            result.push_back(message);
        } else {
            break;
        }
    }
    return result;
}

int GeneralChannel::getId() {
    return config[Const::index];
}

Json GeneralChannel::getConfig(std::vector<std::string> &keys){
    Json ans = config;
    for (std::string key: keys) {
        ans = ans[key];
    }
    return std::move(ans);
}
