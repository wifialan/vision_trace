# coding=utf-8

from socket import *
import time

LOCAL_HOST = ''
LOCAL_PORT = 8499
LOCAL_ADDR = (LOCAL_HOST, LOCAL_PORT)

DEST_HOST_PYTHON = '127.0.0.1'
DEST_PORT_PYTHON = 8877
DEST_ADDR_PYTHON = (DEST_HOST_PYTHON, DEST_PORT_PYTHON)

DEST_HOST_QT = '127.0.0.1'
DEST_PORT_QT = 8399
DEST_ADDR_QT = (DEST_HOST_QT, DEST_PORT_QT)

i = 1;

while True:
    print("Local_port is: " + str(LOCAL_PORT) + "\nwaiting for udp message")
    data, addr = udp_client_sock.recvfrom(BUFSIZ)
    if not data:
        break
    print(time.strftime("%Y-%m-%d %H:%M:%S",time.localtime()),addr,data)


udp_client_sock.close()

