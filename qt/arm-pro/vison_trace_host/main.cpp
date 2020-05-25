#include "mainwindow.h"
#include <QApplication>
#include "ros/ros.h"

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"talker");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    w.showMaximized();
//    w.showFullScreen();

    return a.exec();
}
