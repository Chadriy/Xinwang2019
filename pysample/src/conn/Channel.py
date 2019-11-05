# -*- coding: utf-8 -*-

class Channel(object):

    def init_config(self, config):
        """
        初始化通道
        :param config:
        :return: None
        """
        pass

    def send(self, message, target_id):
        """
        将消息发送给指定目标
        :param message:
        :param target_id:
        :return: None
        """
        pass

    def recv(self):
        """
        获取缓存中的所有消息
        :return: [json]
        """
        pass

    def get_id(self):
        """
        获取当前节点编号
        :return: int
        """
        pass
