import time

from crccheck.crc import Crc16CcittFalse
from struct import pack
from socket import *
import sys

host = '192.168.2.2'
port = 5000
addr = (host, port)

udp_socket = socket(AF_INET, SOCK_DGRAM)
data = b'1'
# data = input('write to server: ')
# data = input("input: ")
# if not data:
#     udp_socket.close()
#     sys.exit(1)

# encode - перекодирует введенные данные в байты, decode - обратно
# data = str.encode(data)
crc_req = Crc16CcittFalse.calc(data)
data = data + pack('<H', crc_req)
while True:
    udp_socket.sendto(data, addr)
    receive = udp_socket.recvfrom(200)

    crc_rec = Crc16CcittFalse.calc(receive)
    print(receive)
    time.sleep(0.025)

udp_socket.close()
