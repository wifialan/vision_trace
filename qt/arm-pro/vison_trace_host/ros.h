#ifndef ROS_H
#define ROS_H

#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>
#include <QObject>
#include <QDebug>
#include <QThread>

#define TURTLEBOT_UP        1
#define TURTLEBOT_DOWN      2
#define TURTLEBOT_RIGHT     3
#define TURTLEBOT_LEFT      4
#define TURTLEBOT_TURN      5
#define TURTLEBOT_STOP      6


class Ros : public QThread{

    Q_OBJECT

public:
    explicit Ros();
    ~Ros();

public:
    ros::Publisher pub_cmd_vel;
    ros::NodeHandle n;

    std::string pub_cmd_vel_topic;
    //    pub_cmd_vel_topic =  "/cmd_vel_mux/input/navi";
    //    pub_cmd_vel = n.advertise<geometry_msgs::Twist>(pub_cmd_vel_topic_, 1);
    //    ros::Rate loopRate(5);
    geometry_msgs::Twist speed; //Twist message
//    ros::Rate loopRate(int hz);
    float speed_x;
    float speed_y;
    float speed_z;
    quint8 flag;
    quint8 move_mode;

private:
    void move(float , float );

protected:
    void run();

private slots:
    void on_turltebot_up();
    void on_turltebot_down();
    void on_turltebot_right();
    void on_turltebot_left();
    void on_turltebot_turn();
    void on_turltebot_stop();


};


#endif // ROS_H

