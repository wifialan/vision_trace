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

#define FORWARD                 0
#define BACKWARD                1

#define START_PATH_PLAN         true
#define STOP_PATH_PLAN          false

#define PATH_STATUS_SLOW            0x00
#define PATH_STATUS_RIGHT           0x0B
#define PATH_STATUS_LEFT            0x0D //1101
#define PATH_STATUS_UP              0x09
#define PATH_STATUS_LED_LEFT        0x01
#define PATH_STATUS_LED_RIGHT       0x08
#define PATH_STATUS_BLANK           0x0F



#define PATH_STRAIGHT_ROAD_LEN       30
#define SPEED_LINE               0.05
#define ROS_SPEED_HIGH              0.4
#define ROS_SPEED_LOW               0.03

typedef struct{

} status;

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
    void straightroad_plan();
    bool check_qr_contains_cross_road_node();
    void check_qr_contains_cross_road_node_code();
    bool turn_tail(qint16);
    void turn_tail_start();
    bool crossroad_check_black_center();
    void leave_crossroad();
    void turltebot_direction_judgement();
    bool first_detect_qr();
    void path_status_slow();
    void detect_qr();

    void send_path_node_to_pc();
    void send_status_to_pc();
    void send_ping_to_pc();
    void send_ctrl_to_pc(quint8);

    VideoCapture capture;
    QTimer *timer;
    QTimer *timer_turn;
    QTimer *timer_crossroad;
    QTimer *timer_crossroad_qr;
    QTimer *timer_path_status_slow;

    quint8 path_status;
    quint8 last_path_status;
    quint8 path_status_calc;
    double ros_speed_line;
    double ros_speed_angular;
    double current_straight_path_ros_speed_line;
    bool first_step;
    Mat frame;
    double rate; //FPS
    QImage image;
    Mat imag_gray_qr;
    Mat grayImage;
    Mat binaryImage;
    Mat erodeImage;
    Mat dilateImage;
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
    bool flag_capture_grap;
    bool flag_arrived_crossroad;
    bool flag_through_crossroad;
    bool flag_qr_detect_crossroad;
    bool flag_path_status_blank;
    bool flag_truned_crossroad_then_up;
    bool flag_no_detect_route_keep_up;
    bool flag_first_step_detect_qr;
    bool path_plan_pre_flag;
//    bool flag_through_crossroad;
    bool turltebot_go;

    bool arrived_flag;
    qint16 pix_offset_qr;
    bool turltebot_direction;
    bool update_path_node_flag;
//    qint16 check_qr_contains_cross_road_node_counter;
    qint16 check_qr_contains_cross_road_node_number;




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
    bool path_status_slow_flag;


    void init_status();
    void lock_status(qint8);

private slots:
    void on_next_frame();
    void on_read_path_plan();
    void on_update_path();
    void on_timer_turn_tail();
    void on_timer_through_crossroad();
    void on_timer_crossroad_qr();
    void on_timer_path_status_slow();
    void on_send_path_info_to_camera(QByteArray);

signals:
    void show_frame(QImage);
    void show_frame_2(QImage);
    void show_tutlebot_status(qint8);
    void turltebot_up(double, double);
    void turltebot_down(double, double);
    void turltebot_right(double, double);
    void turltebot_left(double, double);
    void turltebot_turn(double, double);
    void turltebot_stop();
    void show_command(QByteArray);
    void send_info_to_pc(QByteArray);
    void update_path_node(QByteArray);
    void update_path_start_node(QByteArray);
    //    void path_plan();


protected:
    void run();
};




#endif // CAMERA_H
