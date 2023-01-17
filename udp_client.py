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
    gyro_one_x = np.uint32
    gyro_one_y = np.uint32
    gyro_one_z = np.uint32
    gyro_two_x = np.uint32
    gyro_two_y = np.uint32
    gyro_two_z = np.uint32
    gyro_three_x = np.uint32
    gyro_three_y = np.uint32
    gyro_three_z = np.uint32


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
                    data_rec.gyro_one_x = unpack('<I', bytes_rec[0:4])[0]
                    data_rec.gyro_one_y = unpack('<I', bytes_rec[4:8])[0]
                    data_rec.gyro_one_z = unpack('<I', bytes_rec[8:12])[0]
                    data_rec.gyro_two_x = unpack('<I', bytes_rec[12:16])[0]
                    data_rec.gyro_two_y = unpack('<I', bytes_rec[16:20])[0]
                    data_rec.gyro_two_z = unpack('<I', bytes_rec[20:24])[0]
                    data_rec.gyro_three_x = unpack('<I', bytes_rec[24:28])[0]
                    data_rec.gyro_three_y = unpack('<I', bytes_rec[28:32])[0]
                    data_rec.gyro_three_z = unpack('<I', bytes_rec[32:36])[0]
                    for key, value in data_rec.__dict__.items():
                        if not isinstance(types.FunctionType, types.MethodType):
                            print(key, value)

        except TimeoutError:
            pass
        # b = int.from_bytes(pack(receive[0] + receive[1]), byteorder='big', signed=False)
        # print(b)
        time.sleep(0.025)
except KeyboardInterrupt:
    udp_socket.close()
