#include "ros.h"

Ros::Ros(){

}

Ros::~Ros(){

}

void Ros::move(){

    //qDebug() << "line velocity: " << velocity << "angular velocity" << angular;
//    this->speed.linear.x = linear; // m/s
//    this->speed.angular.z = angular; // rad/s
    this->pub_cmd_vel.publish(this->speed); //
}

void Ros::run(){

    this->move();

//    switch (this->move_mode) {
//    case TURLTEBOT_UP:
//        this->move(0.05, 0.0);
//        break;
//    case TURLTEBOT_DOWN:
//        this->move(-0.05, 0.0);
//        break;
//    case TURLTEBOT_RIGHT:
//        this->move(0.05, -0.08);
//        break;
//    case TURLTEBOT_LEFT:
//        this->move(0.05, 0.08);
//        break;
//    case TURLTEBOT_TURN:
//        this->move(0, 0.1);
//        break;
//    case TURLTEBOT_STOP:
//        this->move(0.0, 0.0);
//        break;
//    default:
//        break;
//    }

}

void Ros::on_turltebot_up(double linear, double angular){

    this->speed.linear.x = linear;
    this->speed.angular.z = 0;
    this->terminate();
    while(!this->wait());
    this->start();
}

void Ros::on_turltebot_down(double linear, double angular){

    this->speed.linear.x = -linear;
    this->speed.angular.z = 0;
    this->move_mode = TURLTEBOT_DOWN;
    this->terminate();
    while(!this->wait());
    this->start();

}

void Ros::on_turltebot_right(double linear, double angular){

    this->speed.linear.x = linear;
    this->speed.angular.z = -angular;
    this->move_mode = TURLTEBOT_RIGHT;
    this->terminate();
    while(!this->wait());
    this->start();

}

void Ros::on_turltebot_left(double linear, double angular){

    this->speed.linear.x = linear;
    this->speed.angular.z = angular;
    this->move_mode = TURLTEBOT_LEFT;
    this->terminate();
    while(!this->wait());
    this->start();

}

void Ros::on_turltebot_turn(double linear, double angular){

    this->speed.linear.x = 0;
    this->speed.angular.z = angular;
    this->move_mode = TURLTEBOT_TURN;
    this->terminate();
    while(!this->wait());
    this->start();

}

void Ros::on_turltebot_stop(double linear, double angular){

    this->move_mode = TURLTEBOT_UP;
    this->terminate();
    while(!this->wait());
    this->speed.linear.x = 0; //
    this->speed.angular.z = 0; //
    this->pub_cmd_vel.publish(this->speed); //
}
