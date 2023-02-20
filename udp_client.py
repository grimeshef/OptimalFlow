import socket
import time
import types

import numpy as np
import logging

from crccheck.crc import Crc16CcittFalse
from struct import *
from socket import *

host = '192.168.2.2'
port = 5000
addr = (host, port)


def rec_func():
    pass


class DataReceive:
    time = np.ulonglong
    first_roll = np.double
    first_pitch = np.double
    first_yaw = np.double

    first_acc_x = np.double
    first_acc_y = np.double
    first_acc_z = np.double

    first_gyro_x = np.double
    first_gyro_y = np.double
    first_gyro_z = np.double

    first_start_pitch = np.double
    first_abs_pitch = np.double

    second_roll = np.double
    second_pitch = np.double
    second_yaw = np.double

    second_acc_x = np.double
    second_acc_y = np.double
    second_acc_z = np.double

    second_gyro_x = np.double
    second_gyro_y = np.double
    second_gyro_z = np.double

    second_start_pitch = np.double
    second_abs_pitch = np.double

    third_roll = np.double
    third_pitch = np.double
    third_yaw = np.double

    third_acc_x = np.double
    third_acc_y = np.double
    third_acc_z = np.double

    third_gyro_x = np.double
    third_gyro_y = np.double
    third_gyro_z = np.double

    third_start_pitch = np.double
    third_abs_pitch = np.double


udp_socket = socket(AF_INET, SOCK_DGRAM)
udp_socket.settimeout(1)
data = b'1'
ping = 10

data_rec = DataReceive
crc_req = Crc16CcittFalse.calc(data)
data = data + pack('<H', crc_req)
try:
    while True:
        try:
            print("Try to connect")
            udp_socket.connect((host, port))
        except OSError:
            print("Communication problem. Check main PCB and Ethernet cable")
        udp_socket.sendto(data, addr)
        try:
            receive = udp_socket.recvfrom(300)
            if receive[0]:
                bytes_rec = receive[0]
                crc_rec_data = Crc16CcittFalse.calc(bytes_rec[0:-2])
                crc_rec_stm = unpack('<H', bytes_rec[-2:])[0]
                if crc_rec_data == crc_rec_stm:
                    data_rec.first_roll = unpack('<d', bytes_rec[0:8])[0]
                    data_rec.first_pitch = unpack('<d', bytes_rec[8:16])[0]
                    data_rec.first_yaw = unpack('<d', bytes_rec[16:24])[0]

                    data_rec.first_acc_x = unpack('<d', bytes_rec[24:32])[0]
                    data_rec.first_acc_y = unpack('<d', bytes_rec[32:40])[0]
                    data_rec.first_acc_z = unpack('<d', bytes_rec[40:48])[0]

                    data_rec.first_gyro_x = unpack('<d', bytes_rec[48:56])[0]
                    data_rec.first_gyro_y = unpack('<d', bytes_rec[56:64])[0]
                    data_rec.first_gyro_z = unpack('<d', bytes_rec[64:72])[0]

                    data_rec.first_start_pitch = unpack('<d', bytes_rec[72:80])[0]
                    data_rec.first_abs_pitch = unpack('<d', bytes_rec[80:88])[0]

                    data_rec.second_roll = unpack('<d', bytes_rec[88:96])[0]
                    data_rec.second_pitch = unpack('<d', bytes_rec[96:104])[0]
                    data_rec.second_yaw = unpack('<d', bytes_rec[104:112])[0]

                    data_rec.second_acc_x = unpack('<d', bytes_rec[112:120])[0]
                    data_rec.second_acc_y = unpack('<d', bytes_rec[120:128])[0]
                    data_rec.second_acc_z = unpack('<d', bytes_rec[128:136])[0]

                    data_rec.second_gyro_x = unpack('<d', bytes_rec[136:144])[0]
                    data_rec.second_gyro_y = unpack('<d', bytes_rec[144:152])[0]
                    data_rec.second_gyro_z = unpack('<d', bytes_rec[152:160])[0]

                    data_rec.second_start_pitch = unpack('<d', bytes_rec[160:168])[0]
                    data_rec.second_abs_pitch = unpack('<d', bytes_rec[168:176])[0]

                    data_rec.third_roll = unpack('<d', bytes_rec[176:184])[0]
                    data_rec.third_pitch = unpack('<d', bytes_rec[184:192])[0]
                    data_rec.third_yaw = unpack('<d', bytes_rec[192:200])[0]

                    data_rec.third_acc_x = unpack('<d', bytes_rec[200:208])[0]
                    data_rec.third_acc_y = unpack('<d', bytes_rec[208:216])[0]
                    data_rec.third_acc_z = unpack('<d', bytes_rec[216:224])[0]

                    data_rec.third_gyro_x = unpack('<d', bytes_rec[224:232])[0]
                    data_rec.third_gyro_y = unpack('<d', bytes_rec[232:240])[0]
                    data_rec.third_gyro_z = unpack('<d', bytes_rec[240:248])[0]

                    data_rec.third_start_pitch = unpack('<d', bytes_rec[248:256])[0]
                    data_rec.third_abs_pitch = unpack('<d', bytes_rec[256:264])[0]
                    data_rec.time = unpack('<Q', bytes_rec[264:272])[0]/1000000

                    # for key, value in data_rec.__dict__.items():
                    #     if not isinstance(types.FunctionType, types.MethodType):
                    #         print(key, value)
                    print("first_roll", data_rec.first_roll)
                    print("first_pitch", data_rec.first_pitch)
                    print("first_yaw", data_rec.first_yaw)
                    print("")
                    print("second_roll", data_rec.second_roll)
                    print("second_pitch", data_rec.second_pitch)
                    print("second_yaw", data_rec.second_yaw)
                    print("")
                    print("third_roll", data_rec.third_roll)
                    print("third_pitch", data_rec.third_pitch)
                    print("third_yaw", data_rec.third_yaw)
                    print("")
                    print("")
                    print("handle_pitch_angle_sub_to_error", data_rec.second_pitch + 3)
                    print("arrow_pitch_angle_relative_handle", -data_rec.second_pitch - 3 + data_rec.third_pitch)
                    print("time", data_rec.time)
        except TimeoutError:
            pass
        logging.basicConfig(level=logging.INFO, filename="py_log.log", filemode="w")
        logging.info(f'{data_rec.__dict__.items()}')
        time.sleep(0.025)
except KeyboardInterrupt:
    udp_socket.close()
