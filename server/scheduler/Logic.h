#pragma once

#include "../extra/json.hpp"

using Json = nlohmann::json;

namespace Logic{
    Json beforeSend(Json message);
    Json afterRecv(Json message);
    Json toErrMessage(Json message, int errCode);
    void route(Json message);
    namespace ChannelOp{
        void build(Json message);
        void destroy(Json message);
    }
    namespace MessageGen{
        void sendBuildMessage(int channelId, int sourceId, int targetId,
                              int state, int errCode, int channelType);
        void sendDestroyMessage(int *port, int channelId);
    }
}
