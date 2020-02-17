#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <qmath.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QZXing.h>
#include "zbar.h"
#include "ros.h"
using namespace cv;

#define PATH_STRAIGHT_ROAD       50

class Camera : public QThread
{
    Q_OBJECT
public:
    Camera();

public:
    void open();
    void show();
    void close();
    QImage Mat2QImage(cv::Mat);
    void path_plan(Mat);
    void path_plan(QByteArray);
    bool check_contains_cross( qint16);


    VideoCapture capture;
    QTimer *timer;
    Mat frame;
    double rate; //FPS
    QImage image;
    Mat grayImage;
    Mat binaryImage;
    Mat dilateImage;
    QString qrCode;
    qint32 counter;

private:
    double speed_line;
    double speed_angular;
    qint8 stop;
    qint8 up;
    qint8 left;
    qint8 right;
    qint8 down;
    qint8 turn;
    void init_status();
    void lock_status(qint8);

private slots:
    void on_next_frame();

signals:
    void show_frame(QImage);
    void show_tutlebot_status(qint16);
    void turltebot_up(double, double);
    void turltebot_down(double, double);
    void turltebot_right(double, double);
    void turltebot_left(double, double);
    void turltebot_turn(double, double);
    void turltebot_stop(double, double);
    //    void path_plan();


protected:
    void run();
};




#endif // CAMERA_H
