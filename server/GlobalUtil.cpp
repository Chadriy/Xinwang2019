#include "GlobalUtil.h"
#include <fstream>
#include <iostream>

int GlobalUtil::startTime;

namespace GlobalUtil{
    std::fstream attach_log;
}

void GlobalUtil::init() {
    startTime = GetTickCount();
    // clear file
    attach_log.open("attach.log", std::ios::out);
    attach_log.close();
    //open
    attach_log.open("attach.log", std::ios::app);
}

float GlobalUtil::curTime() {
    return (GetTickCount() - startTime) / 1000.0;
}

void GlobalUtil::attach(Json message, double start_time) {
    //attach_log<<message["sysMessage"]["data"]<<","<<GlobalUtil::curTime()<<std::endl;
    attach_log<<message["sysMessage"]["data"]<<","<<GlobalUtil::curTime()<<","<<GlobalUtil::curTime() - start_time<<std::endl;
}
