# coding=utf-8

from socket import *
import threading
from time import *

HOST = ''
PORT = 8877
BUFSIZ = 1024
ADDR = (HOST, PORT)

udp_ser_sock = socket(AF_INET, SOCK_DGRAM)
udp_ser_sock.bind(ADDR)

data = 1

def camera():
    while True:
        print(ctime(), "camera running")
        sleep(2)


def udp():
    while True:
        # print("waiting for udp message")
        data, addr = udp_ser_sock.recvfrom(BUFSIZ)
        print(ctime(), "received from ", addr, data)
        udp_ser_sock.sendto(data, addr)
        print(ctime(), " Retrun Data",addr,data)
    udp_ser_sock.close()


def main():
    threads = []
    # 申请一个线程给udp服务
    t = threading.Thread(target=udp, args=())
    threads.append(t)
    # 申请一个线程给camera
    t = threading.Thread(target=camera, args=())
    threads.append(t)
    print("Apply two thread to udp and camera")

    # 此处并不会执行线程，而是将任务分发到每个线程，同步线程。等同步完成后再开始执行start方法
    threads[0].start()
    threads[1].start()

    # 等待上述两个进程执行完毕
    threads[0].join()
    threads[1].join()


if __name__ == '__main__':
    main()
