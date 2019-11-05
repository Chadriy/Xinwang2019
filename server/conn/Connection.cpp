#include <iostream>
#include <fstream>
#include <algorithm>
#include <winsock2.h>
#include <windows.h>
#include <ctime>
#include "../extra/json.hpp"
#include "../extra/socket/Socket.h"
#include "../scheduler/MessageScheduler.h"
#include "../scheduler/Logic.h"
#include "../GlobalUtil.h"
#include "Connection.h"
#include "../const.h"

using Json = nlohmann::json;

namespace conn {
    Json Config;
    MessageScheduler scheduler;
    Judger *judger;
    SocketServer *server;
    Socket **conn;
    float endTime = 0;
    std::map<std::string, double> start_time;
    std::map<std::string, std::string> sys_message;
}

void conn::init(std::string initFilename) {
    std::ifstream confInit(initFilename);
    confInit >> Config;
    confInit.close();
    std::ifstream testFile(Config["mainConfig"]["testData"]);
    std::cout<<Config["mainConfig"]["testData"]<<"---"<<testFile.is_open()<<std::endl;
    Json testJson; testFile >> testJson;
    testFile.close();
    for (Json question : testJson) {
        scheduler.addMessage(question);
        endTime = std::max(endTime, (float) question["timestamp"]);
        start_time[question["sysMessage"]["data"]] = (double) question["timestamp"];
        sys_message[question["sysMessage"]["data"]] = question["sysMessage"].dump();
    }
    //Print("load test data finished");
    endTime += (float) Config["mainConfig"]["timeOut"] * 2;
    int N = Config["mainConfig"]["nodeCount"];
    server = new SocketServer(Config["mainConfig"]["masterPort"], N, NonBlockingSocket);
    conn = new Socket*[N+1];
    judger = new Judger(Config);

}
void conn::waitConns() {
    std::ofstream facclog("server_accept_log.log");
    int N = Config["mainConfig"]["nodeCount"];
    for (int i=1; i<=N;) {
        Socket *accept = server->Accept();
        if (accept == nullptr) {
            continue;
        }
        conn[i] = accept;
        Json nodeConf;
        nodeConf["index"] = i;
        nodeConf["maxChannelConn"] = Config["mainConfig"]["maxChannelCount"][i - 1];
        conn[i]->SendLine(nodeConf.dump());
        //Print("accept %d -> &(%d)", i, accept);
        facclog<<"accept:"<<i<<","<<time(NULL)<<std::endl;
        i++;
    }
}
void conn::mainloop() {
    int N = Config["mainConfig"]["nodeCount"];

    while (GlobalUtil::curTime() <= endTime) {
        ////Print("do recving... %f", endTime - GlobalUtil::curTime());
        for (int i=1; i<=N; i++) {
            std::string messageStr;
            while ((messageStr = conn[i]->ReceiveLine()).size() > 0 && GlobalUtil::curTime() <= endTime) {
                //Print("recv... %s", messageStr.data());
                Json messageRecv = Json::parse(messageStr);
                Logic::afterRecv(messageRecv);
                scheduler.onMessageRecv(messageRecv, i);
            }
        }
        ////Print("do scheduler tick");
        scheduler.tick();
        Sleep(10);
    }
    for (int i=1; i<=N; i++) {
        conn[i]->Close();
    }
}
void conn::send(Json message) {
    if (!judger->beforeSendingCheck(message)) {
        //Print("send refused by judger");
        return ;
    }

    setLog(message);

    int target = message["targetId"];
    //Print("sending... %s", message.dump().data());
    message = Logic::beforeSend(message);
    //Print("conn[%d]: %d", target, conn[target])
    conn[target]->SendLine(message.dump());
    //Print("send succ")
    scheduler.onMessageSend(message, target);
}

void conn::setLog(Json message) {
    std::string callType = message["callType"];
    if (callType == Const::CALL_TYPE_SEND)
    {
        if ((int) message["targetId"] == (int) message["sysMessage"]["target"]) {
            std::string key = message["sysMessage"]["data"];
            auto msg = sys_message.find(key);
            if (msg != sys_message.end() && msg->second == message["sysMessage"].dump()) {
                GlobalUtil::attach(message, start_time[message["sysMessage"]["data"]]);
            } else {
                //Print("sys not match!\n%s\n%s", sys_message[key].data(), message["sysMessage"].dump().data());
            }
        }
    }
}
