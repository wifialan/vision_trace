#include "ros.h"

Ros::Ros(){

    //    connect( this, SIGNAL(turltebot_up() ), this, SLOT(on_turltebot_up()));
    //    connect( this, SIGNAL(turltebot_down() ), this, SLOT(on_turltebot_down()));
    //    connect( this, SIGNAL(turltebot_right() ), this, SLOT(on_turltebot_right()));
    //    connect( this, SIGNAL(turltebot_left() ), this, SLOT(on_turltebot_left()));
    //    connect( this, SIGNAL(turltebot_turn() ), this, SLOT(on_turltebot_turn()));

}

Ros::~Ros(){

}

void Ros::move(float velocity, float angular){

    //qDebug() << "line velocity: " << velocity << "angular velocity" << angular;
    this->speed.linear.x = velocity; //
    this->speed.angular.z = angular; //
    this->pub_cmd_vel.publish(this->speed); //
}


void Ros::run(){
//    ros::Rate loopRate(10);
    //    while(1){
    //        loopRate.sleep();
    switch (this->move_mode) {
    case TURTLEBOT_UP:
        this->move(0.1, 0.0);
        break;
    case TURTLEBOT_DOWN:
        this->move(-0.1, 0.0);
        break;
    case TURTLEBOT_RIGHT:
        this->move(0.01, -0.3);
        break;
    case TURTLEBOT_LEFT:
        this->move(0.01, 0.3);
        break;
    case TURTLEBOT_TURN:
        this->move(0.0, 0.1);
        break;
    case TURTLEBOT_STOP:
        this->move(0.0, 0.0);
        break;
    default:
        break;
    }
    //    }

}

void Ros::on_turltebot_up(){

    this->move_mode = TURTLEBOT_UP;
    this->terminate();
    while(!this->wait());
    this->start();
}

void Ros::on_turltebot_down(){

    this->move_mode = TURTLEBOT_DOWN;
    this->terminate();
    while(!this->wait());
    this->start();

}

void Ros::on_turltebot_right(){

    this->move_mode = TURTLEBOT_RIGHT;
    this->terminate();
    while(!this->wait());
    this->start();

}

void Ros::on_turltebot_left(){

    this->move_mode = TURTLEBOT_LEFT;
    this->terminate();
    while(!this->wait());
    this->start();

}

void Ros::on_turltebot_turn(){

    this->move_mode = TURTLEBOT_TURN;
    this->terminate();
    while(!this->wait());
    this->start();

}

void Ros::on_turltebot_stop(){

    this->move_mode = TURTLEBOT_UP;
    this->terminate();
    while(!this->wait());
    this->speed.linear.x = 0; //
    this->speed.angular.z = 0; //
    this->pub_cmd_vel.publish(this->speed); //
}
