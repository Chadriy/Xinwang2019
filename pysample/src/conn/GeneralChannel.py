import json
import time
import Channel
import SocketClient
from src import Const


class GeneralChannel(Channel.Channel):

    def __init__(self):
        self.client = None
        self.config = None

    def _init_main_config(self):
        self.client = SocketClient.SocketClient(self.config[Const.main_config][Const.master_ip],
                                                self.config[Const.main_config][Const.master_port])
        while True:
            line = self.client.read_line()
            if len(line):
                print "id recved : %s" % (line,)
                ex_config = json.loads(line)
                self.config[Const.index] = ex_config[Const.index]
                self.config[Const.max_channel_conn] = ex_config[Const.max_channel_conn]
                break
            else:
                time.sleep(0.05)

    def init_config(self, config):
        self.config = config
        self._init_main_config()

    def send(self, message, target_id):
        message[Const.target_id] = target_id
        print "msg send : %s" % (message, )
        self.client.println(json.dumps(message))

    def recv(self):
        result = []
        while True:
            line = self.client.read_line()
            if len(line):
                print "msg recv: %s" % (line, )
                message = json.loads(line)
                message[Const.recv_time] = time.time()
                result.append(message)
            else:
                break
        return result

    def get_id(self):
        return self.config[Const.index]

    def get_config(self, args):
        obj = self.config
        for arg in args:
            obj = obj[arg]
        return obj
