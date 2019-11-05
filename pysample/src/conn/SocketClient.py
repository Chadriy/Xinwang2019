
import socket


class SocketClient(object):

    def __init__(self, ip, port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect((ip, port))
        self.socket.setblocking(False)

    def println(self, message):
        message += "\n"
        self.socket.send(message.encode())

    def read_line(self):
        buf = ""
        while True:
            try:
                recv = self.socket.recv(1)
                if recv == '\n':
                    break
                buf += recv
            except socket.error:
                break
        return buf

    def close(self):
        if self.socket is not None:
            self.socket.close()
