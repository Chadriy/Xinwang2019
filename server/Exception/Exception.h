#pragma once

#include "../extra/json.hpp"

using Json = nlohmann::json;

class Exception {
public:
    Exception(Json message, int errCode) {
        this->message = message;
        this->errCode = errCode;
    }

    void setMessage(Json message);
    Json dump();
    int getErrCode() { return errCode; }

private:
    Json message;
    int errCode;
};
