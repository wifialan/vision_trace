#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <qmath.h>
#include <QFile>
#include <QPixmap>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "zbar.h"
#include "ros.h"
using namespace cv;

#define PATH_STRAIGHT_ROAD       55
#define SPEED_LINE               0.07

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
    Mat QImage2Mat(const QImage&);
    void path_plan();
    void path_plan(QByteArray);
    bool check_contains_cross( qint16);
    void crossroad_plan();
    bool check_qr_contains_cross_road_node();
    bool turn_tail(qint16);

    VideoCapture capture;
    QTimer *timer;
    QTimer *timer_turn;
    Mat frame;
    double rate; //FPS
    QImage image;
    Mat imag_gray_qr;
    Mat grayImage;
    Mat binaryImage;
    Mat dilateImage;
    QString QRData_current;
    qint32 counter;
    QByteArray path_plan_array;
    QByteArray start_stop_node_array;
    qint16 flag;
    qint16 index;
    qint16 black_center;
    qint16 black_center_old;
    double speed_angular_current;
    QByteArray crossraod_node_array;
    qint16 check_qr_contains_cross_road_node_flag;
    qint16 check_qr_contains_cross_road_node_counter;
    bool arrived_flag;
    int pix_offset_qr;




private:
    double speed_line;
    double speed_angular;
    qint8 stop;
    qint8 up;
    qint8 left;
    qint8 right;
    qint8 down;
    qint8 turn;
    qint16 len;
    qint16 black_count_100;
    qint16 black_index_100[640];
    qint16 count_100;
    qint16 black_count_300;
    qint16 black_index_300[640];
    qint16 count_300;
    qint16 black_count_500;
    qint16 black_index_500[640];
    qint16 count_500 = 0;
    bool   black_pix_sum_flag;
    qint16 turn_tail_counter;
    qint16 on_turn_tail_counter;
    bool   timer_turn_flag;
    void init_status();
    void lock_status(qint8);

private slots:
    void on_next_frame();
    void on_read_path_plan();
    void on_update_path();
    void on_turn_tail();

signals:
    void show_frame(QImage);
    void show_tutlebot_status(qint16);
    void turltebot_up(double, double);
    void turltebot_down(double, double);
    void turltebot_right(double, double);
    void turltebot_left(double, double);
    void turltebot_turn(double, double);
    void turltebot_stop();
    //    void path_plan();


protected:
    void run();
};




#endif // CAMERA_H
