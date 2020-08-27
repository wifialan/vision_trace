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

#define FORWARD                 false
#define BACKWARD                true

#define START_PATH_PLAN         true
#define STOP_PATH_PLAN          false

#define PATH_STRAIGHT_ROAD_LEN       30
#define SPEED_LINE               0.05

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
    void path_plan_pre();
    void path_plan(QByteArray);
    bool check_contains_cross( qint16);
    void crossroad_plan();
    bool check_qr_contains_cross_road_node();
    void check_qr_contains_cross_road_node_code();
    bool turn_tail(qint16);
    void turn_tail_start();
    bool crossroad_check_black_center();
    void leave_crossroad();
    void turltebot_direction_judgement();

    void send_path_node_to_pc();
    void send_status_to_pc();
    void send_ping_to_pc();
    void send_ctrl_to_pc(quint8);

    void lock_status(qint8);
    void adjust_orentation();
    void check_direction();
    void get_next_frame();

    VideoCapture capture;
    VideoWriter vw;
    int threshold_img;
    QTimer *timer;
    QTimer *timer_turn;
    QTimer *timer_crossroad;
    QTimer *timer_crossroad_qr;

    Mat frame;
    double rate; //FPS
    QImage image;
    Mat imag_gray_qr;
    Mat grayImage;
    Mat binaryImage;
    Mat erodeImage;
    Mat dilateImage;
    Mat template_img;
    QString QRData_stable;
    QString QRData_old;
    QString QRData_store;
    qint16 QRData_store_index;
    QString QRData_current_update;

    QString QRData_near_cross;
    qint32 counter;
    QByteArray path_plan_array;
    QByteArray start_stop_node_array;
    QByteArray path_node_distribute_info;
    qint16 flag;
    qint16 index;
    qint16 black_center;
    qint16 black_center_old;
    double speed_angular_current;
    QByteArray crossraod_node_array_info;
    QByteArray crossraod_node_array;
    QByteArray crossraod_node_near_array;
    qint16 crossroad_number;
    qint16 check_qr_contains_cross_road_node_flag;
    qint16 check_qr_contains_cross_road_node_counter;
    qint16 check_qr_contains_cross_road_node_blank;
    bool flag_first_turn_tail;
    bool flag_turn_tail_start;
    bool flag_on_timer_turn_tail_over;
    bool flag_path_plan;
    bool flag_update_path_node;

    bool arrived_flag;
    qint16 pix_offset_qr;
    bool turltebot_direction;
//    qint16 check_qr_contains_cross_road_node_counter;
    qint16 check_qr_contains_cross_road_node_number;
    qint8 camera_number;
    double doubleSpinBox_line_speed;
    double doubleSpinBox_angular_speed;
    bool start_correct_straight_road;

    bool turltebot_go;
    bool update_path_node_flag;
    quint16 counter_check_next_stop_node;

private:
    double speed_line;
    double speed_angular;
    qint8 stop;
    qint8 up;
    qint8 left;
    qint8 right;
    qint8 down;
    qint8 turn;
    qint16 len_frame;
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
    qint16 on_timer_through_crossroad_counter;
    bool   timer_crossroad_start_flag;
    qint16 on_timer_crossroad_qr_counter;

    bool path_plan_pre_flag;
    bool flag_through_crossroad;
    quint16 road_boundary1;
    quint16 road_boundary2;

    void init_status();

private slots:
    void on_next_frame();
    void on_read_path_plan();
    void on_update_path();
    void on_timer_turn_tail();
    void on_timer_through_crossroad();
    void on_timer_crossroad_qr();
    void on_send_path_info_to_camera(QByteArray path_info);
    void on_send_road_boundary(quint16, quint16);

signals:
    void show_frame(QImage);
    void show_frame_2(QImage);
    void show_tutlebot_status(qint16);
    void turltebot_up(double, double);
    void turltebot_down(double, double);
    void turltebot_right(double, double);
    void turltebot_left(double, double);
    void turltebot_turn(double, double);
    void turltebot_turn_clockwise(double, double);
    void turltebot_turn_counterclockwise(double, double);
    void turltebot_stop();
    void lanuch_turltebot_go();
    void show_command(QByteArray);
    //    void path_plan();

    void send_info_to_pc(QByteArray);
    void update_path_node(QByteArray);
    void update_path_start_node(QByteArray);
    void update_trultebot_direction(bool);

protected:
    void run();
};




#endif // CAMERA_H
