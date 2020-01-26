# -*- coding: utf-8 -*-

from socket import *
import threading
import time
import math
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

#udp_ser_sock = socket(AF_INET, SOCK_DGRAM)
#udp_ser_sock.bind(LOCAL_ADDR)

cap = cv2.VideoCapture(0)

time_data = "hello python"
data = 0
bar_code_data = 0
camera_data_change_flage = 0

def camera():
    global bar_code_data # 设定用为全局变量
    global camera_data_change_flage
    global time_data
    global data
    color = []
    while True:
        data = data + 1
        time_data = time.strftime("%Y-%m-%d %H:%M:%S",time.localtime());
        # print(time_data)
        # 获得图片
        #ret, frame = cap.read()
        frame = cv2.imread("10.jpg")
        # 转化为灰度图
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # 大津法二值化
        retval, dst = cv2.threshold(gray, 0, 255, cv2.THRESH_OTSU)

        #cv2.imshow("capture", dst)
        # 检测QR
        barcodes = pyzbar.decode(gray)
        file = open("data.txt", "w")
        file.write(str(data) + '\n')
        for barcode in barcodes:
            bar_code_data = barcode.data.decode("utf-8")
            camera_data_change_flage = 1 # 新数据产生标志位
            #print(time_data + " " + str(data) + " " + bar_code_data, end='')
            print(time_data + " " + "QR code: " + bar_code_data, end='')
            file.write(bar_code_data)
        file.write('\n')

        # 膨胀，白区域变大
        dst = cv2.dilate(dst, None, iterations=10)
        # # 腐蚀，白区域变小
        #dst = cv2.erode(dst, None, iterations=6)
        # 按Q键退出程序
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            # 存储图片
            cv2.imwrite("camera.jpg", dst)
            break
        # 单看第300行的像素值
        color = dst[100]
        color_line_len = len(dst[100])
        # 找到黑色的像素点个数
        black_count = np.sum(color == 0)
        # 找到黑色的像素点索引
        black_index = np.where(color == 0)
        # 防止black_count=0的报错
        if black_count > 30:
            # black_count = 1
            # 找到黑色像素的中心点位置
            black_center = int(black_index[0][0] + (black_index[0][len(black_index[0])-1] - black_index[0][0]) / 2)
            if black_center < int(color_line_len/2 - 10):  # 小车向轨道左边偏离，发指令让小车右转
                file.write("right")
            if black_center > int(color_line_len/2 + 10):  # 小车向轨道右边偏离，发指令让小车左转
                file.write("left ")
            
            if camera_data_change_flage == 1:
                camera_data_change_flage = 0
            else:
                print(time_data, end='')
            print(" 中心坐标为：", end='')
            print(black_center, end='')
            # 如何中心点的像素是白色，则说明遇到岔路口
            print(" 中心坐标的像素为：", end='')
            
            if(color[black_center]==255):
                print(color[black_center], end='')
                print(" 遇到岔路口")
                file.write(" stop")
            else:
                print(color[black_center])
        file.flush()
        file.close()
        # print("white pix count :\n", black_count)
        # print("white color cneter :\n", center)

        #time.sleep(0.5)
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
    # threads[0].start()
    # threads[1].start()
    threads[2].start()

    # 等待上述进程执行完毕
    # threads[0].join()
    # threads[1].join()
    threads[2].join()


if __name__ == '__main__':
    main()

