from collections import deque
from src import Const


class Action(object):

    def __init__(self, target, scheduler):
        self.target = target
        self.scheduler = scheduler
        self.timeout = 0
        self.waiting_count = 0
        self.queue = deque()

        self.channel_type = None
        self.channel_state = None
        self.channel_id = None

        self._clear_channel_info()

    def _clear_channel_info(self):
        self.channel_type = Const.CHANNEL_TYPE_ERROR
        self.channel_state = Const.CHANNEL_STATE_NONE
        self.channel_id = 0

    def is_valid(self):
        return self.channel_type != Const.CHANNEL_TYPE_ERROR and self.channel_state == Const.CHANNEL_STATE_SUCCESS

    def get_other_type(self):
        return Const.CHANNEL_TYPE_FAST if self.channel_type == Const.CHANNEL_TYPE_NORMAL else Const.CHANNEL_TYPE_NORMAL

    def do_request(self, channel_type):
        if self.channel_type != Const.CHANNEL_TYPE_ERROR:
            return
        self.channel_type = channel_type
        self.channel_state = Const.STATE_REQUEST
        self.scheduler.send_channel_build(self.target, Const.STATE_REQUEST, Const.ERR_CODE_NONE, channel_type)

    def on_request(self, message):
        target = message[Const.sys_message][Const.target]
        channel_type = message[Const.channel_type]
        if self.channel_state != Const.ERR_CODE_NONE:
            if self.scheduler.get_id() < target:
                self.scheduler.send_channel_build(target,
                                                  Const.STATE_REFUSE,
                                                  Const.ERR_CODE_CHANNEL_BUILD_TARGET_REFUSE,
                                                  channel_type)
                return
        self.scheduler.send_channel_build(target, Const.STATE_ACCEPT, Const.ERR_CODE_NONE, channel_type)
        self.channel_type = channel_type
        self.channel_state = Const.CHANNEL_STATE_ACCEPT

    def on_succ(self, message):
        self.channel_type = message[Const.channel_type]
        self.channel_state = Const.CHANNEL_STATE_SUCCESS
        self.channel_id = message[Const.channel_id]
        self._filter_queue()
        for message in self.queue:
            self.do_send(message)
        self.queue.clear()

    def on_refuse(self, message):
        if self.channel_state != Const.CHANNEL_STATE_SUCCESS:
            self._clear_channel_info()
            self._filter_queue()
            if self.waiting_count > 0 or len(self.queue):
                self.do_request(self.get_other_type())

    def on_destroy(self, message):
        if self.channel_state == Const.CHANNEL_STATE_SUCCESS:
            self._clear_channel_info()
        self._filter_queue()
        if self.waiting_count > 0 or len(self.queue):
            self.do_request(self.get_other_type())

    def on_prepare(self, message):
        self.timeout = max(self.timeout, Const.cur_time() + self.scheduler.channel.get_config((Const.main_config,
                                                                                               Const.timeout)))
        self.waiting_count += 1
        if self.channel_state == Const.CHANNEL_STATE_NONE:
            self.do_request(self.get_other_type())

    def on_send(self, message):
        if self.scheduler.get_id() == message[Const.sys_message][Const.target]:
            print "succ received message: %s" % (message[Const.sys_message][Const.data])
            return
        self.waiting_count -= 1
        if self.is_valid():
            self.do_send(message)
        else:
            self.queue.append(message)

    def do_send(self, message):
        if message[Const.recv_time] >= self._get_time_limit():
            message[Const.channel_id] = self.channel_id
            self.scheduler.do_send(message, message[Const.sys_message][Const.target])


    def _filter_queue(self):
        limit = self._get_time_limit()
        self.queue = deque([message for message in self.queue if message[Const.recv_time] >= limit])

    def _get_time_limit(self):
        channel_config = self.get_config()
        lag = channel_config[Const.lag] if channel_config else 0
        timeout = self.scheduler.channel.get_config((Const.main_config, Const.timeout))
        return Const.cur_time() + lag - timeout

    def get_config(self):
        keys = None
        if self.channel_type == Const.CHANNEL_TYPE_FAST:
            keys = (Const.channel_config, Const.high_speed)
        elif self.channel_type == Const.CHANNEL_TYPE_NORMAL:
            keys = (Const.channel_config, Const.normal_speed)
        else:
            return None
        return self.scheduler.channel.get_config(keys)
