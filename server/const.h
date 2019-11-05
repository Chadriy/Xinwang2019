#pragma once
#include <string>

namespace Const{
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

    // 没有错误
    const int ERR_CODE_NONE = 0x0;
    // 通道不存在
    const int ERR_CODE_NO_SUCH_CHANNEL = 0x001;
    // call type 不存在
    const int ERR_CODE_NO_SUCH_CALL_TYPE = 0x002;
    // 通道对面不是这个节点
    const int ERR_CODE_PORT_NOT_VALID = 0x003;
    // 通道创建相关
    const int ERR_CODE_CHANNEL_BUILD_MASK = 0x0100;
    // 对方拒绝创建
    const int ERR_CODE_CHANNEL_BUILD_TARGET_REFUSE = 0x0101;
    // 对方连接数达到上限
    const int ERR_CODE_CHANNEL_BUILD_TARGET_LIMIT = 0x0102;
    // 总连接数达到上限
    const int ERR_CODE_CHANNEL_BUILD_TOTAL_LIMIT = 0x0103;
    // 自己节点数达到上限
    const int ERR_CODE_CHANNEL_BUILD_SOURCE_LIMIT = 0x0104;
    // 对面超时没响应
    const int ERR_CODE_CHANNEL_BUILD_TARGET_TIMEOUT = 0x0105;
    // 未知操作
    const int ERR_CODE_CHANNEL_BUILD_UNKNOWN_OPERATION = 0x106;
    // 发送时相关
    const int ERR_CODE_SEND_MASK = 0x200;
    // 通道上的消息数达到上限
    const int ERR_CODE_SEND_COUNT_LIMIT = 0x201;
    // extra字段大小达到上限
    const int ERR_CODE_SEND_SIZE_LIMIT = 0x202;


    const Json EMPTY_MESSAGE = R"(
        {
          "callType": "",
          "channelId": 0,
          "sysMessage": {
            "target": 0,
            "data": "",
            "delay": 0.0
          },
          "extMessage": {},
          "state": 0,
          "errCode": 0,
          "channelType": 0
        }
    )"_json;
    const float EXP = 1e-5;
}
