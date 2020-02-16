#include "camera.h"

Camera::Camera()
{
    //    cap = new VideoCapture(0);
    qDebug() << "start cap";
    //    cap = cvCreateCameraCapture(0);//打开摄像头，从摄像头中获取视频
    //    VideoCapture cap(0);
}

void Camera::run()
{

}

void Camera::show()
{
    qDebug() << "show cap";
    VideoCapture cap(0);
    while(1){
        cap >> frame;
        if(!frame.data) //如果数据不为空
        {
            qDebug() << "No data";
        }
        namedWindow("[1]", CV_WINDOW_AUTOSIZE);
        imshow("openCV_image",frame);//在窗口中显示对象
        waitKey(30); //等待30ms


        //        frame = cvQueryFrame(cap);  // 从摄像头中抓取并返回每一帧
        //        qDebug() << "read cap";
        //        cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE );
        //        cvShowImage("camera", frame);
        //        //    QImage image = QImage((const uchar*)frame->imageData, frame->width, frame->height,QImage::Format_RGB888).rgbSwapped();
        //        qDebug() << "show cap...";
    }
    //    while(!cap->isOpened()){
    //        qDebug() << "Open cap...";
    //        cap->open(0);
    //    }
    //    cap->read(frame);
    //    namedWindow("[1]");
    //    imshow("camera", &image);

}

QImage Camera::Mat2QImage(cv::Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels()==3)                             //3 channels color image
    {

        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols, cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_RGB888);
    }
    else if(cvImg.channels()==1)                    //grayscale image
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols,cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_Indexed8);
    }
    else
    {
        qImg =QImage((const unsigned char*)(cvImg.data),
                     cvImg.cols,cvImg.rows,
                     cvImg.cols*cvImg.channels(),
                     QImage::Format_RGB888);
    }

    return qImg;

}

void Camera::open()
{
    qDebug() << "start cap";
    if (capture.isOpened())
        capture.release();     //decide if capture is already opened; if so,close it
    capture.open(0);           //open the default camera
    qDebug() << "open cap";
    if (capture.isOpened())
    {
        qDebug() << "read cap";
        rate= capture.get(CV_CAP_PROP_FPS);
        capture >> frame;
        if (!frame.empty())
        {
            qDebug() << "get cap";
            //            image = QImage Mat2QImage(frame);
            image = Mat2QImage(frame);
            emit show_frame(image);
            timer = new QTimer(this);
            timer->setInterval(1000/rate);   //set timer match with FPS
            connect(timer, SIGNAL(timeout()), this, SLOT(on_next_frame()));
            timer->start();
        }
    }
}


///auto get next frame
void Camera::on_next_frame()
{
    capture >> frame;
    if (!frame.empty())
    {
        qint16 dilate_threshold=10;
        Mat element = getStructuringElement(MORPH_RECT, Size(dilate_threshold, dilate_threshold));
        // gray
        cvtColor(frame,grayImage,CV_BGR2GRAY);
        // binaryzation
        threshold(grayImage,binaryImage,150,255,THRESH_BINARY);
        // dilate
        dilate(binaryImage, dilateImage, element);

        QZXing decoder;//(QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_CODE_128);

        image = Mat2QImage(frame);

        decoder.setDecoder(QZXing::DecoderFormat_QR_CODE);
        qrCode = decoder.decodeImage(image);
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss.zzz");

        qDebug().nospace() << current_date;

        if(!qrCode.isEmpty()){
            qDebug() << "QR Code: " << qrCode;
        }
        emit show_frame(image);
        //        ui->label_setPixmap(QPixmap::fromImage(image));
        //this->update();
    }

}
