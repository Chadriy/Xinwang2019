#pragma once

#include <string>
#include "../extra/json.hpp"

using Json = nlohmann::json;

#define Print(format, ...) {\
    printf("[%s:%d:%s]", __FILE__, __LINE__, __FUNCTION__);\
    printf(format, ##__VA_ARGS__);\
    putchar('\n');\
}

namespace Const{

    const std::string main_config = "mainConfig";
    const std::string master_port = "masterPort";
    const std::string master_ip = "masterIp";
    const std::string node_count = "nodeCount";
    const std::string timeout = "timeOut";
    const std::string max_channel_conn = "maxChannelConn";
    const std::string index = "index";

    const std::string channel_config = "channelConfig";
    const std::string high_speed = "highSpeed";
    const std::string normal_speed = "normalSpeed";
    const std::string lag = "lag";

    const std::string target_id = "targetId";
    const std::string recv_time = "recvTime";

    const std::string call_type = "callType";
    const std::string channel_id = "channelId";
    const std::string sys_message = "sysMessage";
    const std::string target = "target";
    const std::string data = "data";
    const std::string delay = "delay";
    const std::string state = "state";
    const std::string err_code = "errCode";
    const std::string channel_type = "channelType";

    const std::string CALL_TYPE_PREPARE = "prepare";
    const std::string CALL_TYPE_SEND = "send";
    const std::string CALL_TYPE_SYS = "sys";
    const std::string CALL_TYPE_CHANNEL_BUILD = "channel_build";
    const std::string CALL_TYPE_CHANNEL_DESTROY = "channel_destroy";

    const int STATE_REQUEST = 0;
    const int STATE_ACCEPT = 1;
    const int STATE_REFUSE = 2;
    const int STATE_NOTICE = -1;

    const int CHANNEL_TYPE_ERROR = -1;
    const int CHANNEL_TYPE_NORMAL = 0;
    const int CHANNEL_TYPE_FAST = 1;

    const int ERR_CODE_NONE = 0x0;
    const int ERR_CODE_NO_SUCH_CHANNEL = 0x001;
    const int ERR_CODE_NO_SUCH_CALL_TYPE = 0x002;
    const int ERR_CODE_CHANNEL_BUILD_MASK = 0x0100;
    const int ERR_CODE_CHANNEL_BUILD_TARGET_REFUSE = 0x0101;
    const int ERR_CODE_CHANNEL_BUILD_TARGET_LIMIT = 0x0102;
    const int ERR_CODE_CHANNEL_BUILD_TOTAL_LIMIT = 0x0103;
    const int ERR_CODE_CHANNEL_BUILD_SOURCE_LIMIT = 0x0104;
    const int ERR_CODE_CHANNEL_BUILD_TARGET_TIMEOUT = 0x0105;
    const int ERR_CODE_CHANNEL_BUILD_UNKNOWN_OPERATION = 0x106;
    const int ERR_CODE_SEND_MASK = 0x200;
    const int ERR_CODE_SEND_COUNT_LIMIT = 0x201;
    const int ERR_CODE_SEND_SIZE_LIMIT = 0x202;

    const int EXP = 1e-5;

    const int CHANNEL_STATE_NONE = 0;
    const int CHANNEL_STATE_REQUEST = 1;
    const int CHANNEL_STATE_ACCEPT = 2;
    const int CHANNEL_STATE_SUCCESS = 3;

    const Json empty_message = R"(
    {
        "callType": "",
                "extMessage": {},
        "sysMessage": {
            "delay": 0.0,
                    "data": "",
                    "target": 0
        },
        "channelId": 0,
                "errCode": 0,
                "state": 0,
                "channelType": 0
    }
    )"_json;

    double curTime();
}
