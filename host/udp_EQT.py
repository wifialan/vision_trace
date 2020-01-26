# coding=utf-8

from socket import *
import time

LOCAL_HOST = ''
LOCAL_PORT = 8399
LOCAL_ADDR = (LOCAL_HOST, LOCAL_PORT)

DEST_HOST_PYTHON = '127.0.0.1'
DEST_PORT_PYTHON = 8877
DEST_ADDR_PYTHON = (DEST_HOST_PYTHON, DEST_PORT_PYTHON)

DEST_HOST_QT = '127.0.0.1'
DEST_PORT_QT = 8499
DEST_ADDR_QT = (DEST_HOST_QT, DEST_PORT_QT)

BUFSIZ = 5

udp_client_sock = socket(AF_INET, SOCK_DGRAM)
udp_client_sock.bind(LOCAL_ADDR)
#udp_client_sock.settimeout(1)

while True:
    #print("Local_port is: " + str(LOCAL_PORT) + "\nwaiting for udp message")
    data, addr = udp_client_sock.recvfrom(BUFSIZ)
    print(time.strftime("%H:%M:%S",time.localtime()),data)


udp_client_sock.close()
