# -*- coding: utf-8 -*-

from socket import *
import threading
import time
import cv2
import numpy as np
import pyzbar.pyzbar as pyzbar

BUFSIZ = 1024

LOCAL_HOST = ''
LOCAL_PORT = 8877
LOCAL_ADDR = (LOCAL_HOST, LOCAL_PORT)

DEST_HOST_EQT = '127.0.0.1'
DEST_PORT_EQT = 8399
DEST_ADDR_EQT = (DEST_HOST_EQT, DEST_PORT_EQT)

DEST_HOST_QT = '192.168.1.216'
DEST_PORT_QT = 8499
DEST_ADDR_QT = (DEST_HOST_QT, DEST_PORT_QT)

udp_ser_sock = socket(AF_INET, SOCK_DGRAM)
udp_ser_sock.bind(LOCAL_ADDR)

cap = cv2.VideoCapture(0)


data = 1
bar_code_data = 0
camera_data_change_flage = 0

def camera():
    global bar_code_data # 设定用为全局变量
    global camera_data_change_flage
    while True:
        # 获得图片
        ret, frame = cap.read()
        
        # 转化为灰度图
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # 大津法二值化
        retval, dst = cv2.threshold(gray, 0, 255, cv2.THRESH_OTSU)
        # cv2.imshow("capture", gray)
        # 检测QR
        barcodes = pyzbar.decode(gray)
        for barcode in barcodes:
            bar_code_data = barcode.data.decode("utf-8")
            camera_data_change_flage = 1 # 新数据产生标志位
            print(time.strftime("%Y-%m-%d %H:%M:%S",time.localtime()),bar_code_data)

        # 膨胀，白区域变大
        dst = cv2.dilate(dst, None, iterations=2)
        # # 腐蚀，白区域变小
        #dst = cv2.erode(dst, None, iterations=6)
        #  cv2.imshow("capture", frame)
        # 按Q键退出程序
        if cv2.waitKey(1) & 0xFF == ord('q'):
            # 存储图片
            cv2.imwrite("camera.jpg", dst)
            break
        # 单看第400行的像素值
        color = dst[400]
        # 找到白色的像素点个数
        white_count = np.sum(color == 255)
        # 找到白色的像素点索引
        white_index = np.where(color == 255)

    ##    time.sleep(0.02)

    cap.release()
    cv2.destroyAllWindows()


def udp_send():
    global bar_code_data # 设定用为全局变量
    global camera_data_change_flage
    global data
    global LOCAL_ADDR
    global DEST_ADDR_EQT
    global DEST_ADDR_QT
    while True:
        # print("waiting for udp message")
        if camera_data_change_flage == 1:
            udp_ser_sock.sendto(bar_code_data.encode(), DEST_ADDR_EQT)
            udp_ser_sock.sendto(bar_code_data.encode(), DEST_ADDR_QT)
            camera_data_change_flage = 0
            
    udp_ser_sock.close()
    
    
def udp_receive():
    global bar_code_data # 设定用为全局变量
    global camera_data_change_flage
    global data
    global LOCAL_ADDR
    global DEST_ADDR_EQT
    global DEST_ADDR_QT
    comm_data = [0xAA,0xBB,0xCC,0xDD]
    rx = []
    while True:
        
        print("waiting for udp message")
        data, addr = udp_ser_sock.recvfrom(BUFSIZ)
        print(addr)
        if data[4] == 0x01:
            udp_ser_sock.sendto(data, DEST_ADDR_EQT)
            print(time.strftime("%Y-%m-%d %H:%M:%S",time.localtime()), "Retrun Data   ",DEST_ADDR_EQT,data)
            data, addr = udp_ser_sock.recvfrom(BUFSIZ)
            udp_ser_sock.sendto(data, DEST_ADDR_QT)
            print(time.strftime("%Y-%m-%d %H:%M:%S",time.localtime()), "Send Data     ",DEST_ADDR_QT,data)

#        print(time.strftime("%Y-%m-%d %H:%M:%S",time.localtime()), "Received from ", addr, data)
        
#        udp_ser_sock.sendto(data, DEST_ADDR_EQT)
#        print(time.strftime("%Y-%m-%d %H:%M:%S",time.localtime()), "Retrun Data   ",DEST_ADDR_EQT,data)
    
    udp_ser_sock.close()


def main():
    threads = []
    # 申请一个线程给udp send服务
    t = threading.Thread(target=udp_send, args=())
    threads.append(t)
    # 申请一个线程给udp receive服务
    t = threading.Thread(target=udp_receive, args=())
    threads.append(t)
    # 申请一个线程给camera
    t = threading.Thread(target=camera, args=())
    threads.append(t)
    print("Apply three thread: udp send、udp receive and camera")

    # 此处并不会执行线程，而是将任务分发到每个线程，同步线程。等同步完成后再开始执行start方法
    threads[0].start()
    threads[1].start()
    threads[2].start()

    # 等待上述进程执行完毕
    threads[0].join()
    threads[1].join()
    threads[2].join()


if __name__ == '__main__':
    main()

