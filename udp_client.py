import socket
import time
import types

import numpy as np

from crccheck.crc import Crc16CcittFalse
from struct import *
from socket import *

host = '192.168.2.2'
port = 5000
addr = (host, port)


class DataReceive:
    sign_roll = np.uint8
    angle_roll = np.uint8
    minutes_roll = np.uint8

    sign_pitch = np.uint8
    angle_pitch = np.uint8
    minutes_pitch = np.uint8

    sign_yaw = np.uint8
    angle_yaw = np.uint8
    minutes_yaw = np.uint8

    sign_acc_x = np.uint8
    whole_acc_x = np.uint8
    fraction_acc_x = np.uint8

    sign_acc_y = np.uint8
    whole_acc_y = np.uint8
    fraction_acc_y = np.uint8

    sign_acc_z = np.uint8
    whole_acc_z = np.uint8
    fraction_acc_z = np.uint8

    sign_gyro_x = np.uint8
    angle_gyro_x = np.uint8
    minutes_gyro_x = np.uint8

    sign_gyro_y = np.uint8
    angle_gyro_y = np.uint8
    minutes_gyro_y = np.uint8

    sign_gyro_z = np.uint8
    angle_gyro_z = np.uint8
    minutes_gyro_z = np.uint8


udp_socket = socket(AF_INET, SOCK_DGRAM)
udp_socket.settimeout(1)
data = b'1'
ping = 10
# data = input('write to server: ')
# data = input("input: ")
# if not data:
#     udp_socket.close()
#     sys.exit(1)

# encode - перекодирует введенные данные в байты, decode - обратно
# data = str.encode(data)
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
            receive = udp_socket.recvfrom(200)
            if receive[0]:
                bytes_rec = receive[0]
                crc_rec_data = Crc16CcittFalse.calc(bytes_rec[0:-2])
                crc_rec_stm = unpack('<H', bytes_rec[-2:])[0]
                if crc_rec_data == crc_rec_stm:
                    data_rec.sign_roll = unpack('<B', bytes_rec[0:1])[0]
                    data_rec.angle_roll = unpack('<B', bytes_rec[1:2])[0]
                    data_rec.minutes_roll = unpack('<B', bytes_rec[2:3])[0]

                    data_rec.sign_pitch = unpack('<B', bytes_rec[3:4])[0]
                    data_rec.angle_pitch = unpack('<B', bytes_rec[4:5])[0]
                    data_rec.minutes_pitch = unpack('<B', bytes_rec[5:6])[0]

                    data_rec.sign_yaw = unpack('<B', bytes_rec[6:7])[0]
                    data_rec.angle_yaw = unpack('<B', bytes_rec[7:8])[0]
                    data_rec.minutes_yaw = unpack('<B', bytes_rec[8:9])[0]

                    data_rec.sign_acc_x = unpack('<B', bytes_rec[9:10])[0]
                    data_rec.whole_acc_x = unpack('<B', bytes_rec[10:11])[0]
                    data_rec.fraction_acc_x = unpack('<B', bytes_rec[11:12])[0]

                    data_rec.sign_acc_y = unpack('<B', bytes_rec[12:13])[0]
                    data_rec.whole_acc_y = unpack('<B', bytes_rec[13:14])[0]
                    data_rec.fraction_acc_y = unpack('<B', bytes_rec[14:15])[0]

                    data_rec.sign_acc_z = unpack('<B', bytes_rec[15:16])[0]
                    data_rec.whole_acc_z = unpack('<B', bytes_rec[16:17])[0]
                    data_rec.fraction_acc_z = unpack('<B', bytes_rec[17:18])[0]

                    data_rec.sign_gyro_x = unpack('<B', bytes_rec[18:19])[0]
                    data_rec.angle_gyro_x = unpack('<B', bytes_rec[19:20])[0]
                    data_rec.minutes_gyro_x = unpack('<B', bytes_rec[20:21])[0]

                    data_rec.sign_gyro_y = unpack('<B', bytes_rec[21:22])[0]
                    data_rec.angle_gyro_y = unpack('<B', bytes_rec[22:23])[0]
                    data_rec.minutes_gyro_y = unpack('<B', bytes_rec[23:24])[0]

                    data_rec.sign_gyro_z = unpack('<B', bytes_rec[24:25])[0]
                    data_rec.angle_gyro_z = unpack('<B', bytes_rec[25:26])[0]
                    data_rec.minutes_gyro_z = unpack('<B', bytes_rec[26:27])[0]

                    # for key, value in data_rec.__dict__.items():
                    #     if not isinstance(types.FunctionType, types.MethodType):
                    #         print(key, value)

                    # print("sign_roll", data_rec.sign_roll)
                    # print("angle_roll", data_rec.angle_roll)
                    # print("minutes_roll", data_rec.minutes_roll)
                    # print("")
                    print("sign_roll", data_rec.sign_pitch)
                    print("angle_roll", data_rec.angle_pitch)
                    print("minutes_roll", data_rec.minutes_pitch)
                    print("")
                    print("sign_pitch", data_rec.sign_yaw)
                    print("angle_pitch", data_rec.angle_yaw)
                    print("minutes_pitch", data_rec.minutes_yaw)
                    print("")
                    print("sign_yaw", data_rec.sign_acc_x)
                    print("angle_yaw", data_rec.whole_acc_x)
                    print("minutes_yaw", data_rec.fraction_acc_x)
                    # print("sign_acc_x", data_rec.sign_acc_x)
                    # print("whole_acc_x", data_rec.whole_acc_x)
                    # print("fraction_acc_x", data_rec.fraction_acc_x)
                    # print("")
                    # print("sign_acc_y", data_rec.sign_acc_y)
                    # print("whole_acc_y", data_rec.whole_acc_y)
                    # print("fraction_acc_y", data_rec.fraction_acc_y)
                    # print("")
                    # print("sign_acc_z", data_rec.sign_acc_z)
                    # print("whole_acc_z", data_rec.whole_acc_z)
                    # print("fraction_acc_z", data_rec.fraction_acc_z)
                    # print("")
                    # print("sign_gyro_x", data_rec.sign_gyro_x)
                    # print("angle_gyro_x", data_rec.angle_gyro_x)
                    # print("minutes_gyro_x", data_rec.minutes_gyro_x)
                    # print("")
                    # print("sign_gyro_y", data_rec.sign_gyro_y)
                    # print("angle_gyro_y", data_rec.angle_gyro_y)
                    # print("minutes_gyro_y", data_rec.minutes_gyro_y)
                    # print("")
                    # print("sign_gyro_z", data_rec.sign_gyro_z)
                    # print("angle_gyro_z", data_rec.angle_gyro_z)
                    # print("minutes_gyro_z", data_rec.minutes_gyro_z)
        except TimeoutError:
            pass
        # b = int.from_bytes(pack(receive[0] + receive[1]), byteorder='big', signed=False)
        # print(b)
        time.sleep(0.025)
except KeyboardInterrupt:
    udp_socket.close()
