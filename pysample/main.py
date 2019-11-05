
import json
import time
from src.conn import GeneralChannel
from src import Scheduler


def main():
    channel = GeneralChannel.GeneralChannel()
    with open("client.json") as fin:
        config = json.load(fin)
        channel.init_config(config)
        mainloop(channel)


def mainloop(channel):
    scheduler = Scheduler.Scheduler(channel)
    while True:
        message = channel.recv()
        for msg in message:
            scheduler.on_recv(msg)
        time.sleep(0.1)


if __name__ == "__main__":
    main()
