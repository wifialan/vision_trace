# coding=utf-8

from socket import *
import time

LOCAL_HOST = ''
LOCAL_PORT = 8499
LOCAL_ADDR = (LOCAL_HOST, LOCAL_PORT)

DEST_HOST_PYTHON = '127.0.0.1'
DEST_PORT_PYTHON = 8877
DEST_ADDR_PYTHON = (DEST_HOST_PYTHON, DEST_PORT_PYTHON)

DEST_HOST_EQT = '127.0.0.1'
DEST_PORT_EQT = 8399
DEST_ADDR_EQT = (DEST_HOST_EQT, DEST_PORT_EQT)

udp_client_sock = socket(AF_INET, SOCK_DGRAM)
udp_client_sock.bind(LOCAL_ADDR)

BUFSIZ = 8

while True:
    #print("Local_port is: " + str(LOCAL_PORT) + "\nwaiting for udp message")
    data, addr = udp_client_sock.recvfrom(BUFSIZ)
    print(time.strftime("%H:%M:%S",time.localtime()),data)


udp_client_sock.close()

