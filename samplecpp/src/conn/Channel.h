#pragma once

#include <vector>
#include "../../extra/json.hpp"
using Json = nlohmann::json;

class Channel {
public:
    /**
    * 初始化管道
    * @param config
    */
    virtual void initConfig(Json config) = 0;

    /**
     * 将消息发送给指定目标
     * @param message
     * @param targetId
     */
    virtual void send(Json message, int targetId) = 0;

    /**
     * 获取缓存中的所有消息
     * @return 消息列表
     */
    virtual std::vector<Json> recv() = 0;

    /**
     * 获取当前节点编号
     * @return 获取当前节点编号
     */
    virtual int getId() = 0;

    /**
     * 获取节点参数
     * @param args
     * @return
     */
    virtual Json getConfig(std::vector<std::string> &args) = 0;
};