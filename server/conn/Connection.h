#pragma once

#include <string>
#include "../extra/json.hpp"
#include "../scheduler/Judger.h"
#include "../scheduler/MessageScheduler.h"

class MessageScheduler;
class Judger;
using Json = nlohmann::json;

namespace conn {
    extern Json Config;
    extern MessageScheduler scheduler;
    extern Judger *judger;
    void init(std::string initFilename);
    void waitConns();
    void mainloop();
    void send(Json message);
    void setLog(Json message);
}
