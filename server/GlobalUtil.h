#pragma once

#include <stdio.h>
#include <windows.h>
#include "extra/json.hpp"

using Json = nlohmann::json;

#define Print(format, ...) {\
    printf("[%s:%d:%s:%.4f]", __FILE__, __LINE__, __FUNCTION__, GlobalUtil::curTime());\
    printf(format, ##__VA_ARGS__);\
    putchar('\n');\
}


namespace GlobalUtil {
    extern int startTime;

    void init();

    float curTime();

    void attach(Json message, double start_time);
}
