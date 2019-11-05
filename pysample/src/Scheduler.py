import Const
import Action


class Scheduler(object):

    def __init__(self, channel):
        self.channel = channel
        n = channel.get_config((Const.main_config, Const.node_count))
        self.action = [None] + [Action.Action(i + 1, self) for i in xrange(n)]
        self.channel_map = {}

    def get_id(self):
        return self.channel.get_id()

    def on_recv(self, message):
        if message[Const.err_code] != Const.ERR_CODE_NONE:
            return
        {
            Const.CALL_TYPE_PREPARE: self.on_prepare,
            Const.CALL_TYPE_SEND: self.on_send,
            Const.CALL_TYPE_SYS: self.on_sys,
            Const.CALL_TYPE_CHANNEL_BUILD: self.on_channel_build,
            Const.CALL_TYPE_CHANNEL_DESTROY: self.on_destroy
        }[message[Const.call_type]](message)

    def on_prepare(self, message):
        self.action[message[Const.sys_message][Const.target]].on_prepare(message)

    def on_send(self, message):
        self.action[message[Const.sys_message][Const.target]].on_send(message)

    def on_sys(self, message):
        pass

    def on_channel_build(self, message):
        target = message[Const.sys_message][Const.target]
        if message[Const.channel_id] != 0:
            self.action[target].on_succ(message)
            self.channel_map[message[Const.channel_id]] = target
        else:
            state = message[Const.state]
            if state == Const.STATE_NOTICE:
                self.action[target].on_request(message)
            elif state == Const.STATE_REFUSE:
                self.action[target].on_refuse(message)

    def on_destroy(self, message):
        channel_id = message[Const.channel_id]
        target = self.channel_map.get(channel_id, 0)
        if target != 0:
            self.action[target].on_destroy(message)
            del self.channel_map[channel_id]

    def send_channel_build(self, target, state, err_code, channel_type):
        self.do_send({
            Const.call_type: Const.CALL_TYPE_CHANNEL_BUILD,
            Const.state: state,
            Const.sys_message: {
                Const.target: target,
                Const.data: "",
                Const.delay: 0
            },
            Const.err_code: err_code,
            Const.channel_type: channel_type
        }, 0)

    def do_send(self, message, target):
        msg = Const.get_empty_message()
        msg.update(message)
        self.channel.send(msg, target)

