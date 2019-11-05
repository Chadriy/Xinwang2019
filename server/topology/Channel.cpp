
#include "Channel.h"
#include "PrepareChannel.h"
#include "../const.h"
#include "../conn/Connection.h"

Channel::Channel(Json buildMessage) {
    id = GetNextId();
    port[0] = buildMessage["sourceId"];
    port[1] = buildMessage["sysMessage"]["target"];
    type = buildMessage["channelType"];
}

Json Channel::getConfig(int type) {
    const Json ERR_CONFIG = R"({
          "lag": 0.0,
          "maxCount": 0,
          "buildTime": 0.0,
          "maxMessageCount": 0
    })";
    if (type == Const::CHANNEL_TYPE_ERROR) return ERR_CONFIG;
    std::string typeName = type == Const::CHANNEL_TYPE_NORMAL ? "normalSpeed" : "highSpeed";
    return conn::Config["channelConfig"][typeName];
}

Json Channel::getConfig() {
    return getConfig(type);
}

PrepareChannel::PrepareChannel(int source, int target, float timeout) {
    this->source = source;
    this->target = target;
    this->timeout = GlobalUtil::curTime() + timeout;
}

int PrepareChannel::getHash() const {
    return source * 10000 + target;
}

bool PrepareChannel::operator < (const PrepareChannel &channel) const {
    return timeout < channel.timeout || timeout == channel.timeout && getHash() < channel.getHash();
}

bool PrepareChannel::isTimeout() {
    return this->timeout <= GlobalUtil::curTime();
}

int PrepareChannel::getSource() { return source; }

int PrepareChannel::getTarget() { return target; }

float PrepareChannel::getTimeout() { return timeout; }
