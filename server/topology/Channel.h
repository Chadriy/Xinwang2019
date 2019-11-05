#pragma once

#include "../extra/json.hpp"
#include "../conn/Connection.h"

using Json = nlohmann::json;

class Channel {
private:
    static int GetNextId() {
        static int x = 0;
        return ++x;
    }

    int id;
    int port[2];
    int type;

public:
    Channel(Json buildMessage);
    int getId() { return id; }
    static Json getConfig(int type);
    Json getConfig();

    int getOtherPort(int thisPort) {
        return thisPort == port[0] ? port[1] : port[0];
    }

    int* getPort() { return port; }

    int getType() { return type; }
};

