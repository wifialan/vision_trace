#-------------------------------------------------
#
# Project created by QtCreator 2020-01-02T15:35:34
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vison_trace_host
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ros.cpp \
    camera.cpp

HEADERS += \
        mainwindow.h \
    ros.h \
    camera.h

FORMS += \
        mainwindow.ui

## OpenCv
INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv \
               /usr/local/include/opencv2

LIBS += -L/usr/local/lib/ -lopencv_highgui
LIBS += -L/usr/local/lib/ -lopencv_core
LIBS += -L/usr/local/lib/ -lopencv_imgproc
LIBS += -L/usr/local/lib/ -lopencv_imgcodecs
LIBS += -L/usr/local/lib/ -lopencv_ml
LIBS += -L/usr/local/lib/ -lopencv_videoio

## QR
#include(/home/alan/work/vision_trace/qt/QZXing/source/QZXing.pri)
include(../qzxing-master/src/QZXing.pri)
INCLUDEPATH += -I /usr/local/zbar/include
LIBS += -L/usr/local/zbar/lib/ -lzbar

## ROS

INCLUDEPATH += -I /opt/ros/kinetic/include
DEPENDPATH += /opt/ros/kinetic/include

#--add ros libs
#unix:!macx: LIBS += -L /opt/ros/kinetic/lib/ -lroscpp
LIBS += -L /opt/ros/kinetic/lib/ -lroscpp
LIBS += -L /opt/ros/kinetic/lib/ -lroslib
LIBS += -L /opt/ros/kinetic/lib/ -lpthread
LIBS += -L /opt/ros/kinetic/lib/ -lroscpp_serialization
LIBS += -L /opt/ros/kinetic/lib/ -lrostime
LIBS += -L /opt/ros/kinetic/lib/ -lrosconsole
LIBS += -L /opt/ros/kinetic/lib/ -lrosconsole_log4cxx
LIBS += -L /opt/ros/kinetic/lib/ -lrosconsole_backend_interface
LIBS += -L /opt/ros/kinetic/lib/ -lxmlrpcpp

