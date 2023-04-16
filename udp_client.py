import time
from threading import Timer
import select

import numpy as np
import logging

from crccheck.crc import Crc16CcittFalse
from struct import *
import socket

host = '192.168.2.2'
port = 5000
addr = (host, port)


class DataReceive:
    time = np.ulonglong
    lin_enc = np.double


udp_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
# udp_socket.settimeout(1)
data = b'1'
ping = 10
roll_param = 3.1
pitch_param = 3.1
data_rec = DataReceive
crc_req = Crc16CcittFalse.calc(data)
data = data + pack('<H', crc_req)


def send_pkg():
    udp_socket.sendto(data, addr)
    rec_pkg()


def rec_pkg():
    ready = select.select([udp_socket], [], [], 1.0 / 40)
    if ready[0]:
        receive = udp_socket.recvfrom(300)
        bytes_rec = receive[0]
        crc_rec_data = Crc16CcittFalse.calc(bytes_rec[0:-2])
        crc_rec_stm = unpack('<H', bytes_rec[-2:])[0]
        if crc_rec_data == crc_rec_stm:
            data_rec.lin_enc = unpack('<d', bytes_rec[0:8])[0]
            data_rec.time = unpack('<Q', bytes_rec[8:16])[0]/1000000

            # for key, value in data_rec.__dict__.items():
            #     if not isinstance(types.FunctionType, types.MethodType):
            #         print(key, value)
            print("")
            print("lin_anc", data_rec.lin_enc)
            print("time", data_rec.time)
            safe_logs()
    else:
        print("aa")

def safe_logs():
    logging.basicConfig(level=logging.INFO, filename="py_log.log", filemode="w")
    logging.info(f'{data_rec.__dict__.items()}')


# timer_send = Timer(1.0 / 40, send_pkg)
# timer_logs = Timer(1/40, safe_logs)
# timer_logs.start()
class RepeatTimer(Timer):
    def run(self):
        while not self.finished.wait(self.interval):
            self.function(*self.args, **self.kwargs)


timer_send = RepeatTimer(1.0/40, send_pkg)
timer_send.run()


try:
    while True:
        send_pkg()
        # time.sleep(0.25)
except KeyboardInterrupt:
    timer_send.cancel()
    udp_socket.close()
