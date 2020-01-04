# coding=utf-8

from socket import *
import time

HOST = 'localhost'
LOCAL_PORT = 8399
DEST_PORT = 8877
BUFSIZ = 1024
LOCAL_ADDR = (HOST, LOCAL_PORT)
DEST_ADDR = (HOST,DEST_PORT)

udp_client_sock = socket(AF_INET, SOCK_DGRAM)
udp_client_sock.bind(LOCAL_ADDR)

i = 1;

while True:
    print("Local_port is: " + str(LOCAL_PORT) + "\nwaiting for udp message")
    data, ADDR = udp_client_sock.recvfrom(BUFSIZ)
    if not data:
        break
    print(time.strftime("%Y-%m-%d %H:%M:%S",time.localtime()),data)


udp_client_sock.close()
