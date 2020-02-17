#include "camera.h"

Camera::Camera()
{
    init_status();
    this->counter = 0;
    //    path = new Pathplan();
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
    }

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
    capture.open(1);           //open the default camera
    qDebug() << "open cap";
    if (capture.isOpened())
    {
        qDebug() << "read cap";
        capture.set(CV_CAP_PROP_FPS, 60);
        rate= capture.get(CV_CAP_PROP_FPS);
        capture >> frame;
        if (!frame.empty())
        {
            qDebug() << "get cap";
            //            image = QImage Mat2QImage(frame);
            image = Mat2QImage(frame);
            //            emit show_frame(image);
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

        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss.zzz");

        //        qDebug() << current_date << " " << counter++;
        //        QDebug("%s","sss");

        qint16 dilate_threshold=10;
        Mat element = getStructuringElement(MORPH_RECT, Size(dilate_threshold, dilate_threshold));
        // gray
        cvtColor(frame,grayImage,CV_BGR2GRAY);
        // binaryzation
        threshold(grayImage,binaryImage,90,255,THRESH_BINARY);
        // dilate
        dilate(binaryImage, dilateImage, element);

        image = Mat2QImage(dilateImage);
        //        image = image.mirrored(true, false);

        //QR
        zbar::ImageScanner scanner;
        int width 	= grayImage.cols;
        int height 	= grayImage.rows;
        scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
        unsigned char *pdata = (unsigned char *)grayImage.data;
        zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
        int n = scanner.scan(imageZbar);
        if (n > 0){
            // extract results
            for (zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
                 symbol != imageZbar.symbol_end();
                 ++symbol) {
                // do something useful with results
                QString symbolData = QString::fromStdString(symbol->get_data());
                qDebug() << "QR Code: " << symbolData;
            }
        }

        path_plan(dilateImage);
        emit show_frame(image);

#if 0
        QZXing decoder;//(QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_CODE_128);
        decoder.setDecoder(QZXing::DecoderFormat_QR_CODE);
        qrCode = decoder.decodeImage(image);
        if(!qrCode.isEmpty()){
            qDebug() << "QR Code: " << qrCode;
        }
#endif

    }

}

void Camera::path_plan(Mat image)
{

    qint16 cols = dilateImage.cols;
    // get 100th line pix data
    char *data_1 = dilateImage.ptr<char>(200);
    QByteArray color_line_100 = QByteArray(data_1,cols);
    char *data_2 = dilateImage.ptr<char>(300);
    // get 300th line pix data
    QByteArray color_line_300 = QByteArray(data_2,cols);
    // get 500th line pix data
    char *data_3 = dilateImage.ptr<char>(400);
    QByteArray color_line_500 = QByteArray(data_3,cols);

    qint16 len = cols;
    qint16 black_count_100 = 0;
    qint16 black_index_100[len] = {0};
    qint16 count_100 = 0;
    qint16 black_count_300 = 0;
    qint16 black_index_300[len] = {0};
    qint16 count_300 = 0;
    qint16 black_count_500 = 0;
    qint16 black_index_500[len] = {0};
    qint16 count_500 = 0;

    for (int i = 1; i < len - 1; ++i) {
        if (color_line_100[i] == 0) {
            black_count_100 ++;
            black_index_100[count_100 ++] = i;
        }
        if (color_line_300[i] == 0) {
            black_count_300 ++;
            black_index_300[count_300 ++] = i;
        }
        if (color_line_500[i] == 0) {
            black_count_500 ++;
            black_index_500[count_500 ++] = i;
        }

    }
    if (qAbs( qAbs(black_index_100[0] - black_index_300[0]) - \
              qAbs(black_index_300[0] - black_index_500[0]) < 5)
            )
    {
        qDebug() << "Straight Line";
        this->speed_line = 0.2;
        this->speed_angular = 0.05;
    } else {
        qDebug() << "*********************************\nCurve Line";
        this->speed_line = 0.2;
        this->speed_angular = this->speed_line * 3.5;
    }
    if (black_index_100[0] < 100 || black_index_100[black_count_100 - 1] > len - 100) {
        this->speed_angular += this->speed_line * 1.5;
    }
    qDebug() << "Black pix sum  : " << black_count_100;
    qDebug() << "Black pix coord:" << black_index_100[0] << " " << black_index_100[black_count_100 - 1];
    if (black_count_100 > PATH_STRAIGHT_ROAD) {

        //find the center of the black pix
        qint16 black_center = (black_index_100[0] + black_index_100[black_count_100 - 1]) / 2;
        qDebug() << "black_center" << black_center;

        if (!check_contains_cross(black_count_100) && black_center < (len / 2 - 10)) {
            //小车向轨道右边偏离，发指令让小车左转
            qDebug() << "LEFT";
            if (left == 1) {
                emit show_tutlebot_status(TURLTEBOT_LEFT);
                lock_status(TURLTEBOT_LEFT);
            }

            emit turltebot_left(speed_line, speed_angular);
        }
        else if(!check_contains_cross(black_count_100) && black_center > (len / 2 + 10)){
            //小车向轨道边左偏离，发指令让小车右转
            qDebug() << "RIGHT";
            if (right == 1) {
                emit show_tutlebot_status(TURLTEBOT_RIGHT);
                lock_status(TURLTEBOT_RIGHT);
            }
            emit turltebot_right(speed_line, speed_angular);

        }
        else if (check_contains_cross(black_count_100)) {
            qDebug() << "STOP";
            if (stop == 1) {
                emit show_tutlebot_status(TURLTEBOT_STOP);
                lock_status(TURLTEBOT_STOP);
            }
            emit turltebot_stop(speed_line, speed_angular);
        }
        else {
            qDebug() << "UP";
            if (up == 1) {
                emit show_tutlebot_status(TURLTEBOT_UP);
                lock_status(TURLTEBOT_UP);
            }
            emit turltebot_up(speed_line, speed_angular);
        }
    }
}

bool Camera::check_contains_cross(qint16 len)
{

    if(len > 2.2 * PATH_STRAIGHT_ROAD){
        return true;
    }
    else {
        return false;
    }
}

void Camera::lock_status(qint8 sta)
{
    switch (sta) {
    case TURLTEBOT_UP:
        up      = 0;
        left    = 1;
        right   = 1;
        down    = 1;
        stop    = 1;
        turn    = 1;
        break;
    case TURLTEBOT_LEFT:
        up      = 1;
        left    = 0;
        right   = 1;
        down    = 1;
        stop    = 1;
        turn    = 1;
        break;
    case TURLTEBOT_RIGHT:
        up      = 1;
        left    = 1;
        right   = 0;
        down    = 1;
        stop    = 1;
        turn    = 1;
        break;
    case TURLTEBOT_DOWN:
        up      = 1;
        left    = 1;
        right   = 1;
        down    = 0;
        stop    = 1;
        turn    = 1;
        break;
    case TURLTEBOT_STOP:
        up      = 1;
        left    = 1;
        right   = 1;
        down    = 1;
        stop    = 0;
        turn    = 1;
        break;
    case TURLTEBOT_TURN:
        up      = 1;
        left    = 1;
        right   = 1;
        down    = 1;
        stop    = 1;
        turn    = 0;
        break;
    default:
        break;
    }
}

void Camera::init_status()
{
    up      = 1;
    left    = 1;
    right   = 1;
    down    = 1;
    stop    = 1;
    turn    = 1;
}
