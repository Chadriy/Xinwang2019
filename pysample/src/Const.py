import json
import copy
import time

main_config = "mainConfig"
master_port = "masterPort"
master_ip = "masterIp"
node_count = "nodeCount"
timeout = "timeOut"
max_channel_conn = "maxChannelConn"
index = "index"

channel_config = "channelConfig"
high_speed = "highSpeed"
normal_speed = "normalSpeed"
lag = "lag"

target_id = "targetId"
recv_time = "recvTime"

call_type = "callType"
channel_id = "channelId"
sys_message = "sysMessage"
target = "target"
data = "data"
delay = "delay"
state = "state"
err_code = "errCode"
channel_type = "channelType"

CALL_TYPE_PREPARE = "prepare"
CALL_TYPE_SEND = "send"
CALL_TYPE_SYS = "sys"
CALL_TYPE_CHANNEL_BUILD = "channel_build"
CALL_TYPE_CHANNEL_DESTROY = "channel_destroy"

STATE_REQUEST = 0
STATE_ACCEPT = 1
STATE_REFUSE = 2
STATE_NOTICE = -1

CHANNEL_TYPE_ERROR = -1
CHANNEL_TYPE_NORMAL = 0
CHANNEL_TYPE_FAST = 1

ERR_CODE_NONE = 0x0
ERR_CODE_NO_SUCH_CHANNEL = 0x001
ERR_CODE_NO_SUCH_CALL_TYPE = 0x002
ERR_CODE_CHANNEL_BUILD_MASK = 0x0100
ERR_CODE_CHANNEL_BUILD_TARGET_REFUSE = 0x0101
ERR_CODE_CHANNEL_BUILD_TARGET_LIMIT = 0x0102
ERR_CODE_CHANNEL_BUILD_TOTAL_LIMIT = 0x0103
ERR_CODE_CHANNEL_BUILD_SOURCE_LIMIT = 0x0104
ERR_CODE_CHANNEL_BUILD_TARGET_TIMEOUT = 0x0105
ERR_CODE_CHANNEL_BUILD_UNKNOWN_OPERATION = 0x106
ERR_CODE_SEND_MASK = 0x200
ERR_CODE_SEND_COUNT_LIMIT = 0x201
ERR_CODE_SEND_SIZE_LIMIT = 0x202

EXP = 1e-5

CHANNEL_STATE_NONE = 0
CHANNEL_STATE_REQUEST = 1
CHANNEL_STATE_ACCEPT = 2
CHANNEL_STATE_SUCCESS = 3

empty_message = json.loads("""
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
""")


def get_empty_message():
    global empty_message
    return copy.deepcopy(empty_message)


def cur_time():
    return time.time()


