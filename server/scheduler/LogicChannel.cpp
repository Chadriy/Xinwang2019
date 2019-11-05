#include "Logic.h"
#include "../conn/Connection.h"
#include "../topology/Network.h"
#include "../topology/Channel.h"
#include "../const.h"
#include "../Exception/Exception.h"

void Logic::ChannelOp::build(Json message) {
    Network &network = conn::scheduler.getNetwork();
    int channelId = 0, channelType = message["channelType"];
    int state = 0, err = 0;
    int target = message["sysMessage"]["target"], source = message["sourceId"];
    switch ((int) message["state"]) {
        case Const::STATE_REQUEST:
            conn::judger->onChannelPrepare(message);
            network.prepareToBuild(message);
            Logic::MessageGen::sendBuildMessage(
                    channelId,
                    source,
                    target,
                    Const::STATE_NOTICE,
                    Const::ERR_CODE_NONE,
                    channelType
            );
            break;
        case Const::STATE_ACCEPT:
            try {
                //Print("Accpet build channel start");
                channelId = network.recvPrepareMessage(message);
                state = Const::STATE_ACCEPT;
                err = Const::ERR_CODE_NONE;
                //Print("Accpet build channel fin %d <---> %d by %d", source, target, channelId);
            } catch (Exception *e) {
                channelId = 0;
                state = Const::STATE_REFUSE;
                err = e->getErrCode();
            }
            Logic::MessageGen::sendBuildMessage(
                    channelId,
                    source,
                    target,
                    state,
                    err,
                    channelType
            );
            Logic::MessageGen::sendBuildMessage(
                    channelId,
                    target,
                    source,
                    state,
                    err,
                    channelType
            );
            break;
        case Const::STATE_REFUSE:
            network.deletePrepareMessage(message);
            Logic::MessageGen::sendBuildMessage(
                    channelId,
                    source,
                    target,
                    Const::STATE_REFUSE,
                    Const::ERR_CODE_CHANNEL_BUILD_TARGET_REFUSE,
                    channelType
            );
            break;
        default:
            Logic::MessageGen::sendBuildMessage(
                    channelId,
                    target,
                    source,
                    Const::STATE_REFUSE,
                    Const::ERR_CODE_CHANNEL_BUILD_UNKNOWN_OPERATION,
                    -1
            );
            break;
    }
}

void Logic::ChannelOp::destroy(Json message) {
    int channelId = message["channelId"];
    Network &network = conn::scheduler.getNetwork();
    Channel *channel = network.getChannel(channelId);
    if (channel == nullptr) {
        throw new Exception(message, Const::ERR_CODE_NO_SUCH_CHANNEL);
    } else {
        Logic::MessageGen::sendDestroyMessage(channel->getPort(), channelId);
        channel = nullptr;
        network.destroyChannel(message);
    }
}
