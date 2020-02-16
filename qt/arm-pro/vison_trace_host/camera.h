#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QZXing.h>

using namespace cv;

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


private:
    VideoCapture capture;
    QTimer *timer;
    Mat frame;
    double rate; //FPS
    QImage image;
    Mat grayImage;
    Mat binaryImage;
    Mat dilateImage;
    QString qrCode;

private slots:
    void on_next_frame();

signals:
    void show_frame(QImage);


protected:
    void run();
};

#endif // CAMERA_H
