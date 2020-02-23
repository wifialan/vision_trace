#include "camera.h"

Camera::Camera()
{

    flag = 0;
    index = 0;
    turn_tail_counter = 0;
    on_turn_tail_counter = 0;
    black_center_old =0;
    timer_turn_flag = true;
    speed_angular_current = 0.0;
    check_qr_contains_cross_road_node_flag = 0;
    QRData_current = "FFFF";
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

void Camera::open()
{
    qDebug() << "start cap";
    if (capture.isOpened()){
        capture.release();     //decide if capture is already opened; if so,close it
    }
    capture.open(0);           //open the default camera
    qDebug() << "open cap";
    if (capture.isOpened())
    {
        qDebug() << "read cap";
        capture.set(CV_CAP_PROP_FPS, 30);
        rate= capture.get(CV_CAP_PROP_FPS);
        capture >> frame;
        if (!frame.empty())
        {
            qDebug() << "get cap";
            //            image = QImage Mat2QImage(frame);
            image = Mat2QImage(frame);
            //            emit show_frame(image);
            timer = new QTimer(this);
            timer_turn = new QTimer(this);
            timer_turn->setInterval(200);    // set timer: 200ms
            timer->setInterval(1000/rate);   //set timer match with FPS
            connect(timer, SIGNAL(timeout()), this, SLOT(on_next_frame()));
            connect(timer_turn, SIGNAL(timeout()), this, SLOT(on_turn_tail()));
            timer->start();
        }
    }
}

void Camera::on_update_path()
{
    qDebug() << path_plan_array.split(',').at(index++);
    qDebug() << path_plan_array.split(',').at(index++);
    flag = 0;
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


        //        qDebug() << grayImage.rows;
        //        qDebug() << height;
        // image.copy(x,y,w,h); // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
        // if beyond aera, fill 0(black)
        //        QImage image_qr = image.copy(1, 240, width, 240);
        //        imag_gray_qr = QImage2Mat(image_qr);
        //        qDebug() << "Mat size:" << imag_gray_qr.size;
        //QR
        zbar::ImageScanner scanner;
        scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
        pix_offset_qr = 50;
        int width 	= grayImage.cols;
        int height 	= grayImage.rows;

        //Rect(x,y,w,h)
        // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
        //Mat roi(grayImage, Rect(0,height - pix_offset_qr,width,height + pix_offset_qr));
        unsigned char *pdata = (unsigned char *)grayImage.data;
        zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
        int n = scanner.scan(imageZbar);
        QString QRData;
        if (n > 0){
            // extract results
            for (zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
                 symbol != imageZbar.symbol_end();
                 ++symbol) {
                // do something useful with results
                QRData = QString::fromStdString(symbol->get_data());
                //                qDebug() << "QR Code: " << QRData;
            }
            if(QRData_current != QRData){
                QRData_current.clear();
                QRData_current.append(QRData);
                // update path command
                if(QRData != (start_stop_node_array.split('\n').at(0)) && QRData != (start_stop_node_array.split('\n').at(1))){
                    index ++;
                    if (index != path_plan_array.split(',').length()) {
                        qDebug() << "准备更新当前路线指令...";
                        qDebug() << "当前路线指令为----------:" << path_plan_array.split(',').at(index);
                        qDebug() << "完成更新当前路线指令...OK";
                    }
                }
            }
        }

        threshold(grayImage,binaryImage,100,255,THRESH_BINARY);
        // dilate
        dilate(binaryImage, dilateImage, element);

        image = Mat2QImage(binaryImage);
        //        image = image.mirrored(true, false);
        emit show_frame(image);

        if(QRData == (start_stop_node_array.split('\n').at(1)) && arrived_flag == false){

            //Rect(x,y,w,h)
            // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
            Mat roi(grayImage, Rect(0,height/2,width,height/2));
            unsigned char *pdata2 = (unsigned char *)roi.data;
            zbar::Image imageZbar2(width, height/2, "Y800", pdata2, width * (height/2));
            int qr_data = scanner.scan(imageZbar2);
            if (qr_data > 0) {
                arrived_flag = true;
                qDebug() << "\nQR Code: " << QRData;
                qDebug() << "*****************************************";
                qDebug() << "  Arrived terminate, stop the turltebot";
                qDebug() << "*****************************************";

                if (stop == 1) {
                    emit show_tutlebot_status(TURLTEBOT_STOP);
                    lock_status(TURLTEBOT_STOP);
                    emit turltebot_stop();
                }
                return;
            }
        }
        if(arrived_flag == true){
            return;
        }

        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss.zzz");
        qDebug() << current_date;
        qDebug() << "QR Code: " << QRData;

        if (index < path_plan_array.split(',').length()) {
            qDebug() << "当前路线命令索引号:" <<index;
            qDebug() << "当前路线命令为----:" << path_plan_array.split(',').at(index);
            //            qDebug() << "准备更新当前路线指令...";
            //            qDebug() << "当前路线指令为    :" << path_plan_array.split(',').at(index);
            //            qDebug() << "完成更新当前路线指令...OK";
        } else {
            qDebug() << "\nWARNING: index is out of the lengh of path_plan_array";
            return;
        }

        if( turn_tail(index) == true ){
            return;
        }
        //return;
        path_plan();
    }
}

void Camera::path_plan()
{

    qint16 cols = dilateImage.cols;
    // get 100th line pix data
    char *data_1 = dilateImage.ptr<char>(150);
    QByteArray color_line_100 = QByteArray(data_1,cols);
    char *data_2 = dilateImage.ptr<char>(300);
    // get 300th line pix data
    QByteArray color_line_300 = QByteArray(data_2,cols);
    // get 500th line pix data
    char *data_3 = dilateImage.ptr<char>(400);
    QByteArray color_line_500 = QByteArray(data_3,cols);

    len = cols;
    black_count_100 = 0;
    memset(black_index_100,0,len);
    count_100 = 0;
    black_count_300 = 0;
    memset(black_index_300,0,len);
    count_300 = 0;
    black_count_500 = 0;
    memset(black_index_500,0,len);
    count_500 = 0;

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
        qDebug() << "此处为直线";
        this->speed_line = SPEED_LINE;
        this->speed_angular = this->speed_line / 2 ;
        if(black_center < len / 2 - 15 || black_center > len / 2 + 15){
            this->speed_angular += 3 * this->speed_line;
            if(black_center < len / 2 - 30 || black_center > len / 2 + 30){
                this->speed_angular += 3 * this->speed_line;
                if(black_center < len / 2 - 50 || black_center > len / 2 + 60){
                    this->speed_angular += 4 * this->speed_line;
                }
            }
        }
    } else {
        qDebug() << "此处为曲线";
        this->speed_line = SPEED_LINE;
        this->speed_angular = this->speed_line * 3.5;
    }
    if (black_index_100[0] < 100 || black_index_100[black_count_100 - 1] > len - 100) {
        qDebug() << "加速修正偏移";
        this->speed_angular += this->speed_line * 2.5;
    }
    qDebug() << "第100行黑色像素共有:" << black_count_100;
    qDebug() << "其黑色像素的首尾坐标:" << black_index_100[0] << " " << black_index_100[black_count_100 - 1];

    if (black_count_100 > PATH_STRAIGHT_ROAD) {

        //find the center of the black pix
        black_center = (black_index_100[0] + black_index_100[black_count_100 - 1]) / 2;
        qDebug() << "黑色像素中心为：" << black_center;

        if (!check_contains_cross(black_count_100) && black_center < (len / 2 - 10)) {
            //小车向轨道右边偏离，发指令让小车左转
            qDebug() << "小车向轨道右边偏离，发指令让小车左转";
            if (left == 1) {
                emit show_tutlebot_status(TURLTEBOT_LEFT);
                lock_status(TURLTEBOT_LEFT);
            }

            // turltebot adjust agular speed
            //            if (black_center < black_center_old) {
            //                // Speed the turn left
            //                black_center_old = black_center;
            //                this->speed_angular += this->speed_angular + 2*this->speed_line;
            //            } else {
            //                // Slow the turn left
            //                black_center_old = black_center;
            //                this->speed_angular += this->speed_angular - 2*this->speed_line;
            //            }
            black_pix_sum_flag = false;
            qDebug() << "[左转] 线速度为：" << speed_line << "m/s " << "角速度为：" << speed_angular << "rad/s";
            emit turltebot_left(speed_line, speed_angular);
        }
        else if(!check_contains_cross(black_count_100) && black_center > (len / 2 + 10)){
            //小车向轨道边左偏离，发指令让小车右转
            qDebug() << "小车向轨道边左偏离，发指令让小车右转";
            if (right == 1) {
                emit show_tutlebot_status(TURLTEBOT_RIGHT);
                lock_status(TURLTEBOT_RIGHT);
            }
            // turltebot adjust agular speed
            //            if (black_center < black_center_old) {
            //                // Speed the turn right
            //                black_center_old = black_center;
            //                this->speed_angular += this->speed_angular + 2*this->speed_line;
            //            } else {
            //                // Slow the turn left
            //                black_center_old = black_center;
            //                this->speed_angular += this->speed_angular - 2*this->speed_line;
            //            }
            black_pix_sum_flag = false;
            qDebug() << "[右转] 线速度为：" << speed_line << "m/s " << "角速度为：" << speed_angular << "rad/s";
            emit turltebot_right(speed_line, speed_angular);

        }
        else if (check_contains_cross(black_count_100)) {
            //          !!! WARING !!!
            // the turtlebot move status maybe change
            //          !!! WARING !!!
            crossroad_plan();
        }
        else {
            qDebug() << "路线无偏移，沿当前路线直行";
            if (up == 1) {
                emit show_tutlebot_status(TURLTEBOT_UP);
                lock_status(TURLTEBOT_UP);
            }
            // turltebot adjust agular speed
            //            if (black_center < black_center_old) {
            //                // Speed the turn right
            //                black_center_old = black_center;
            //                this->speed_angular += this->speed_angular + 2*this->speed_line;
            //            } else {
            //                // Slow the turn left
            //                black_center_old = black_center;
            //                this->speed_angular += this->speed_angular - 2*this->speed_line;
            //            }///
            qDebug() << "[直行] 线速度为：" << speed_line << "m/s " << "角速度为：" << speed_angular << "rad/s";
            emit turltebot_up(speed_line, speed_angular);
        }
    }
}

void Camera::crossroad_plan()
{

    qDebug() << "[此处为岔道口]";
    this->speed_line = SPEED_LINE;
    this->speed_angular = this->speed_line * 2;

    //    qDebug() << "path plan: " << path_plan_array.split(',').at(index);

    //    if (black_count_100 > 1.8 * PATH_STRAIGHT_ROAD) {
    //        qDebug() << "Entered Cross Road......";
    //        black_pix_sum_flag = true;
    //    }

    //    if (black_pix_sum_flag == true) {
    //        if (black_count_300 < 1.3 * PATH_STRAIGHT_ROAD) {
    if( !check_qr_contains_cross_road_node() && black_count_100 < 1.4 * PATH_STRAIGHT_ROAD){
        //        check_qr_contains_cross_road_node_flag ++;
        //        qDebug() << "check_qr_contains_cross_road_node_flag: " << check_qr_contains_cross_road_node_flag;
        //        if (check_qr_contains_cross_road_node_flag >= 10) {
        //            check_qr_contains_cross_road_node_flag = 0;
        qDebug() << "\n...............................................................";
        qDebug() << "cross road over\n...............................................................\n";
        qDebug() << "Update current path command";
        index ++; // Update current path command;
        qDebug() << "364:index ++";
        QRData_current.clear();
        qDebug() << "Next path command: " << path_plan_array.split(',').at(index);
        //            QThread::sleep(10);
        return;
        //        }

    }
    //    }

    // judge the black center location
    if (black_center < (len / 2 - 10) || black_center > (len / 2 + 10)) {
        // turltebot adjust agular speed
        if (black_center < black_center_old) {
            // Speed the turn left
            qDebug() << "Speed the turn left";
            black_center_old = black_center;
            this->speed_angular += this->speed_angular + 2*this->speed_line;
        } else {
            // Slow the turn left
            qDebug() << "Slow the turn left";
            black_center_old = black_center;
            this->speed_angular += this->speed_angular - 2*this->speed_line;
        }
    }
    if (path_plan_array.split(',').at(index) == "LEFT") {
        if (left == 1) {
            emit show_tutlebot_status(TURLTEBOT_LEFT);
            lock_status(TURLTEBOT_LEFT);
        }
        qDebug() << "Cross road: LEFT";
        emit turltebot_left(speed_line, speed_angular);
    }
    else if (path_plan_array.split(',').at(index) == "RIGHT") {
        if (right == 1) {
            emit show_tutlebot_status(TURLTEBOT_RIGHT);
            lock_status(TURLTEBOT_RIGHT);
        }
        qDebug() << "Cross road: RIGHT";
        emit turltebot_right(speed_line, speed_angular);
    }
    else if (path_plan_array.split(',').at(index) == "UP") {
        if (up == 1) {
            emit show_tutlebot_status(TURLTEBOT_UP);
            lock_status(TURLTEBOT_UP);
        }
        qDebug() << "Cross road: UP";
        emit turltebot_up(speed_line, speed_angular);
    }
}

bool Camera::check_qr_contains_cross_road_node()
{
    //QR
    //    zbar::ImageScanner scanner;
    //    int width 	= grayImage.cols;
    //    int height 	= grayImage.rows/2;
    //    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    //    unsigned char *pdata = (unsigned char *)imag_gray_qr.data;
    //    zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
    //    int n = scanner.scan(imageZbar);
    check_qr_contains_cross_road_node_counter ++;
    if (check_qr_contains_cross_road_node_counter >= 750 / (SPEED_LINE * 100) ) {
        zbar::ImageScanner scanner;
        scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
        int width 	= grayImage.cols;
        int height 	= grayImage.rows / 2;

        //Rect(x,y,w,h)
        // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
        Mat roi(grayImage, Rect(0,height - pix_offset_qr,width,height + pix_offset_qr));
        unsigned char *pdata = (unsigned char *)roi.data;
        zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
        int n = scanner.scan(imageZbar);
        if (n > 0){

            // indicate contains QR
            qDebug() << "还在岔道里面";
            return true;
        } else {
            check_qr_contains_cross_road_node_counter = 0;
            qDebug() << "离开岔道";
            return false;
        }
    } else {
        qDebug() << "还在岔道里面";
        return true;
    }
}

bool Camera::check_contains_cross(qint16 len)
{

    if(QRData_current == crossraod_node_array.split(',').at(0) || \
            QRData_current == crossraod_node_array.split(',').at(1)     )
    {
        return true;
    }
    else {
        return false;
    }
}

void Camera::on_read_path_plan()
{
    path_plan_array.clear();
    QFile file("path_plan.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    path_plan_array = file.readLine();
    file.close();
    qDebug() << "path plan: " << path_plan_array;

    crossraod_node_array.clear();
    QFile crossraod_node_file("crossraod_node.txt");
    crossraod_node_file.open(QIODevice::ReadOnly | QIODevice::Text);
    crossraod_node_array = crossraod_node_file.readLine();
    crossraod_node_file.close();
    qDebug() << "crossraod node: " << crossraod_node_array;

    start_stop_node_array.clear();
    QByteArray tmp;
    QFile file_stop_node("node.txt");
    file_stop_node.open(QIODevice::ReadOnly | QIODevice::Text);
    tmp = file_stop_node.readLine();
    //    tmp = tmp.mid(0,tmp.length()-1);
    start_stop_node_array.append(tmp);
    //    stop_node.append(
    start_stop_node_array.append(file_stop_node.readLine());
    file_stop_node.close();

    qDebug() << "start stop node: " << start_stop_node_array;

    arrived_flag = false;
    index = 0;

    this->open();

}

bool Camera::turn_tail(qint16 index)
{
    if (path_plan_array.split(',').at(index) == "TURN") {
        qDebug() << "TURN TAIL";
        if (timer_turn_flag == true) {
            timer_turn->start();
            timer_turn_flag = false;
        }
        return true;
    }
    return false;
}

void Camera::on_turn_tail(){

    emit turltebot_turn(0.0, 3.14159 / 3.0);

    on_turn_tail_counter ++;
    // Timer is 200ms, 15 * 200ms = 3s
    if (on_turn_tail_counter >= 20) {
        timer_turn->stop();
        timer_turn_flag = true;
        on_turn_tail_counter = 0;
        qDebug() << "Update path command";
        index ++;
        qDebug() << "The next path command is:" << path_plan_array.split(',').at(index);
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

Mat Camera::QImage2Mat(const QImage& qimage)
{
    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
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
