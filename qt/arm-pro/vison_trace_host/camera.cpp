#include "camera.h"

Camera::Camera()
{

    timer_crossroad_start_flag = true;
    QRData_near_cross = "0";
    turltebot_direction = false;
    flag = 0;
    index = 0;
    turn_tail_counter = 0;
    on_turn_tail_counter = 0;
    black_center_old =0;
    timer_turn_flag = true;
    speed_angular_current = 0.0;
    check_qr_contains_cross_road_node_flag = 0;
    QRData_stable = "FFFF";
    QRData_old = "FFFF";
    init_status();
    this->counter = 0;
    this->open();
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
            timer_crossroad = new QTimer(this);
            timer_crossroad_qr = new QTimer(this);
            timer_crossroad_qr->setInterval(200);
            timer_crossroad->setInterval(200);// set timer: 200ms
            timer_turn->setInterval(200);    // set timer: 200ms
            timer->setInterval(1000/rate);   //set timer match with FPS
            connect(timer, SIGNAL(timeout()), this, SLOT(on_next_frame()));
            connect(timer_turn, SIGNAL(timeout()), this, SLOT(on_timer_turn_tail()));
            connect(timer_crossroad, SIGNAL(timeout()), this, SLOT(on_timer_through_crossroad()));
            connect(timer_crossroad_qr, SIGNAL(timeout()), this, SLOT(on_timer_crossroad_qr()));
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
        qint16 dilate_threshold_qr=30;
        qint16 erode_threshold=60;
        qint16 dilate_threshold =30 /*- dilate_threshold_qr*/;
        Mat element_dilateImage_qr = getStructuringElement(MORPH_RECT, Size(dilate_threshold_qr, dilate_threshold_qr));
        Mat element_dilateImage = getStructuringElement(MORPH_RECT, Size(dilate_threshold, dilate_threshold));
        Mat element_erodeImage = getStructuringElement(MORPH_RECT, Size(erode_threshold, erode_threshold));
        // gray
        cvtColor(frame,grayImage,CV_BGR2GRAY);
        threshold(grayImage,binaryImage,60,255,THRESH_BINARY);
        // dilate
        dilate(binaryImage, dilateImage, element_dilateImage_qr);
        // erode
        erode(dilateImage, erodeImage,element_erodeImage);
        //dilate
        dilate(erodeImage, dilateImage, element_dilateImage);
        //        Mat detected_edges,dst;
        //        qint16 lowThreshold = 70;
        //        blur( grayImage, detected_edges, Size(3,3) );
        //        Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*3, 3 );
        //        dst.create(grayImage.size(), grayImage.type());
        //        dst = Scalar::all(255);
        //        grayImage.copyTo(dst, detected_edges);

        image = Mat2QImage(dilateImage);
        //        image = image.mirrored(true, false);
        emit show_frame(image);
        emit show_frame_2(Mat2QImage(frame));

        //QR
        zbar::ImageScanner scanner;
        scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
        pix_offset_qr = 50;
        int width 	= grayImage.cols;
        int height 	= grayImage.rows/2;

        //Rect(x,y,w,h)
        // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
        Mat roi(grayImage, Rect(0,height,width,height));
        unsigned char *pdata = (unsigned char *)roi.data;
        zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
        int n = scanner.scan(imageZbar);
        //        QString QRData;
        QRData_current_update.clear();

        if (n > 0){
            // extract results
            for (zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
                 symbol != imageZbar.symbol_end();
                 ++symbol) {
                // do something useful with results
                QRData_current_update = QString::fromStdString(symbol->get_data());
                if (turltebot_go == false) {
                    qDebug() << "QR Code: " << QRData_current_update;
                    return;
                }
            }
            if(QRData_stable != QRData_current_update){
                QRData_stable.clear();
                QRData_stable.append(QRData_current_update);
                QRData_store.append(QRData_current_update + ',');
                QRData_store_index ++;
                turltebot_direction_judgement();
                send_status_to_pc();
                qDebug() << "新扫描的QR内容为：" << QRData_current_update;
                // update path command
                if(QRData_current_update != start_stop_node_array.split('\n').at(0) && \
                        flag_path_plan == true){
                    index ++;
                    if (index != path_plan_array.split(',').length() && \
                            QRData_current_update != (start_stop_node_array.split('\n').at(1))) {
                        qDebug() << "准备更新当前路线指令...";
                        qDebug() << "当前路线指令为---------:" << path_plan_array.split(',').at(index);
                        qDebug() << "完成更新当前路线指令...OK";
                    }
                }
            }
        }

        if (turltebot_go == false) {
            return;
        }

        if(QRData_current_update == (start_stop_node_array.split('\n').at(1)) && arrived_flag == false){

            // 此处代码在重新检测二维码目的在于，让小车更加靠近二维码而停下来
            //Rect(x,y,w,h)
            // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
            Mat roi(grayImage, Rect(0,height,width,height));
            unsigned char *pdata2 = (unsigned char *)roi.data;
            zbar::Image imageZbar2(width, height, "Y800", pdata2, width * (height));
            int qr_data = scanner.scan(imageZbar2);
            if (qr_data > 0) {
                arrived_flag = true;
                flag_path_plan = false;
                qDebug() << "\nQRData_current_update  :" << QRData_current_update;
                qDebug() << "经过的QR点为：" << QRData_store;
                if (turltebot_direction == FORWARD) {
                    qDebug() << "小车朝向为正向";
                } else {
                    qDebug() << "小车朝向为反向";
                }
                qDebug() << "*****************************************";
                qDebug() << "  Arrived terminate, stop the turltebot";
                qDebug() << "*****************************************";
                start_stop_node_array.append(QRData_stable);
                start_stop_node_array.append('\n');
                start_stop_node_array.append(" ");
                send_status_to_pc();
                timer_crossroad->stop();
                timer_crossroad_start_flag = true;
                turltebot_go = false;
                //qDebug() << "on_timer_through_crossroad_counter is over:" << on_timer_through_crossroad_counter*200 << "ms" ;
                on_timer_through_crossroad_counter = 0;
                check_qr_contains_cross_road_node_counter = 0;
                check_qr_contains_cross_road_node_number = 0;

                if (stop == 1) {
                    emit show_tutlebot_status(TURLTEBOT_STOP);
                    lock_status(TURLTEBOT_STOP);
                    emit turltebot_stop();
                    //如果小车终点的QR值为岔道口节点，那么记录下来小车经过的上一个节点
                    if (QRData_stable == crossraod_node_array.split(',').at(0) || \
                            QRData_stable == crossraod_node_array.split(',').at(1) ) {
                        QRData_near_cross = QRData_store.split(',').at(index - 1);
                        qDebug() << "小乌龟停到了岔道口处，上一个节点为：" << QRData_near_cross;
                    }
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
        qDebug() << "QRData_current_update: " << QRData_current_update;
        qDebug() << "QRData_stable        : " << QRData_stable;

        if (index < path_plan_array.split(',').length()) {
            qDebug() << "当前路线命令索引号:" <<index;
            qDebug() << "当前路线命令为---:" << path_plan_array.split(',').at(index);
        } else {
            qDebug() << "\nWARNING: index is out of the lengh of path_plan_array";
            return;
        }

        if (turltebot_direction == FORWARD) {
            qDebug() << "小车朝向为正向";
        } else {
            qDebug() << "小车朝向为反向";
        }

        if (path_plan_pre_flag == true) {
            qDebug() << "检测是否需要调整路线";
            path_plan_pre();
            emit show_command(path_plan_array);
            path_plan_pre_flag = false;
        }
        if( turn_tail(index) == true ){
            return;
        }
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

    len_frame = cols;
    black_count_100 = 0;
    memset(black_index_100,0,len_frame);
    count_100 = 0;
    black_count_300 = 0;
    memset(black_index_300,0,len_frame);
    count_300 = 0;
    black_count_500 = 0;
    memset(black_index_500,0,len_frame);
    count_500 = 0;

    for (int i = 1; i < len_frame - 1; ++i) {
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
        // 判断是否需要加速修正轨道
        if(black_center < len_frame / 2 - 20 || black_center > len_frame / 2 + 20){
            this->speed_angular += 1 * this->speed_line;
            if(black_center < len_frame / 2 - 40 || black_center > len_frame / 2 + 40){
                this->speed_angular += 1 * this->speed_line;
                if(black_center < len_frame / 2 - 55 || black_center > len_frame / 2 + 55){
                    this->speed_angular += 1 * this->speed_line;
                }
            }
        }
    } else {
        qDebug() << "此处为曲线";
        this->speed_line = SPEED_LINE;
        this->speed_angular = this->speed_line * 3.5;
    }
    if (black_index_100[0] < 100 || black_index_100[black_count_100 - 1] > len_frame - 100) {
        qDebug() << "加速修正偏移";
        this->speed_angular += this->speed_line * 2.5;
    }
    qDebug() << "第100行黑色像素共有:" << black_count_100;
    qDebug() << "其黑色像素的首尾坐标:" << black_index_100[0] << " " << black_index_100[black_count_100 - 1];

    if (black_count_100 > PATH_STRAIGHT_ROAD_LEN) {

        //find the center of the black pix
        black_center = (black_index_100[0] + black_index_100[black_count_100 - 1]) / 2;
        qDebug() << "黑色像素中心为：" << black_center;

        if (!check_contains_cross(black_count_100) && black_center < (len_frame / 2 - 10) \
                && flag_through_crossroad == true) {
            //小车向轨道右边偏离，发指令让小车左转
            qDebug() << "小车向轨道右边偏离，发指令让小车左转";
            if (left == 1) {
                emit show_tutlebot_status(TURLTEBOT_LEFT);
                lock_status(TURLTEBOT_LEFT);
            }

            qDebug() << "[左转] 线速度为：" << speed_line << "m/s " << "角速度为：" << speed_angular << "rad/s";
            emit turltebot_left(speed_line, speed_angular);
        }
        else if(!check_contains_cross(black_count_100) && black_center > (len_frame / 2 + 10) \
                && flag_through_crossroad == true){
            //小车向轨道边左偏离，发指令让小车右转
            qDebug() << "小车向轨道边左偏离，发指令让小车右转";
            if (right == 1) {
                emit show_tutlebot_status(TURLTEBOT_RIGHT);
                lock_status(TURLTEBOT_RIGHT);
            }
            qDebug() << "[右转] 线速度为：" << speed_line << "m/s " << "角速度为：" << speed_angular << "rad/s";
            emit turltebot_right(speed_line, speed_angular);

        }
        else if ((check_contains_cross(black_count_100) || flag_through_crossroad == false) && \
                 on_timer_crossroad_qr_counter == 0)
        {
            //          !!! WARING !!!
            // the turtlebot move status maybe change
            //          !!! WARING !!!
            crossroad_plan();
        }
        else if(!check_contains_cross(black_count_100) && flag_through_crossroad == true) {
            qDebug() << "路线无偏移，沿当前路线直行";
            if (up == 1) {
                emit show_tutlebot_status(TURLTEBOT_UP);
                lock_status(TURLTEBOT_UP);
            }
            qDebug() << "[直行] 线速度为：" << speed_line << "m/s " << "角速度为：" << speed_angular << "rad/s";
            emit turltebot_up(speed_line, speed_angular);
        }
    }
}

void Camera::path_plan_pre() {
    // 如果起始坐标位于岔路入口点上，并且小车为反向，路线命令第一个为“TURN”，第二个为“UP”
    if (start_stop_node_array.split('\n').at(0) == crossraod_node_array.split(',').at(0) \
            && turltebot_direction == BACKWARD && path_plan_array.split(',').at(1) == "UP" \
            && path_plan_array.split(',').at(0) == "TURN") {
        qDebug() << "起始坐标位于岔路入口点上，并且小车为反向，路线命令第一个为“TURN”，第二个为“UP”";
        for (int i = 0; i < crossroad_number; ++i) {
            //当小车目的地恰好为岔道口时
            if (QRData_near_cross == crossraod_node_near_array.split('\n').at(i).split(',').at(0)) {
                qDebug() << "当前小车停靠在第" + QString(i + 1) + "条路线上";

                QByteArray path_plan_array2;
                int k = path_plan_array.split(',').length();

                if (i == 0) {
                    //小车在第一条路线上
                    // TURN,UP,...   --->   TURN,RIGHT,UP...
                    for (int j = 0; j < k-1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                        if (j == 0) {
                            path_plan_array2.append("RIGHT,");
                        }
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(k-1));
                } else if (i == 1 && crossroad_number == 2) {
                    // 小车在第二条路线上，一共有两条路线
                    // TURN,UP,...   --->   TURN,LEFT,UP...
                    for (int j = 0; j < k-1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                        if (j == 0) {
                            path_plan_array2.append("LEFT,");
                        }
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(k-1));
                } else if (i == 1 && crossroad_number == 3) {
                    // 小车在第二条路线上，一共有三条路线
                    qDebug() << "小车在第二条路线上，一共有三条路线，不需要更新路线命令";
                } else if (i == 2) {
                    // 小车在第三条路线上，一共有三条路线
                    // TURN,UP,...   --->   TURN,LEFT,UP...
                    for (int j = 0; j < k-1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                        if (j == 0) {
                            path_plan_array2.append("LEFT,");
                        }
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(k-1));
                }

                path_plan_array = path_plan_array2;
                qDebug() << "当前的路线命令为：" << path_plan_array;
                qDebug() << "小车期望朝向为反向，现在小车实际朝向就是反向，那么第一个TURN将跳过不再执行";
                this->index ++;
                qDebug() << "路线命令索引为：" << this->index;
                qDebug() << "待执行路线命令：" << path_plan_array.split(',').at(this->index);

                return;
            }
        }
    }
    // 如果起始坐标位于岔路入口点上，并且小车为反向，路线命令第一个为“LEFT”或者“RIGHT”，第二个为“UP”，那么需要把第一个LEFT命令替换掉
    else if (start_stop_node_array.split('\n').at(0) == crossraod_node_array.split(',').at(0) \
             && turltebot_direction == BACKWARD && path_plan_array.split(',').at(1) == "UP" \
             && (path_plan_array.split(',').at(0) == "LEFT" || path_plan_array.split(',').at(0) == "RIGHT")) {
        if (path_plan_array.split(',').at(0) == "LEFT") {
            qDebug() << "如果起始坐标位于岔路入口点上，并且小车为反向，路线命令第一个为“LEFT”，第二个为“UP”，那么需要把第一个LEFT命令替换掉";
        } else {
            qDebug() << "如果起始坐标位于岔路入口点上，并且小车为反向，路线命令第一个为“RIGHT”，第二个为“UP”，那么需要把第一个RIGHT命令替换掉";
        }
        for (int i = 0; i < crossroad_number; ++i) {
            if (QRData_near_cross == crossraod_node_near_array.split('\n').at(i).split(',').at(0)) {

                qDebug() << "当前小车停靠在第" + QString(i + 1) + "条路线上";
                QByteArray path_plan_array2;
                int len = path_plan_array.split(',').length();
                if (i == 0) {
                    // LEFT,UP ----> TURN,UP
                    // RIGHT,UP ----> LEFT,UP
                    if (path_plan_array.split(',').at(0) == "LEFT") {
                        path_plan_array2.append("TURN,");//目的是让小车姿态调整为正向
                    } else if (path_plan_array.split(',').at(0) == "RIGHT"){
                        path_plan_array2.append("LEFT,");
                    }
                    for (int j = 1; j < len - 1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(len-1));

                }else if(i == 1 && crossroad_number == 2) {
                    // 小车在第二条路线上，一共有两条路线
                    // LEFT,UP ----> RIGHT,UP
                    // RIGHT,UP ----> TURN,UP
                    if (path_plan_array.split(',').at(0) == "LEFT") {
                        path_plan_array2.append("RIGHT,");//目的是让小车姿态调整为正向
                    } else if (path_plan_array.split(',').at(0) == "RIGHT"){
                        path_plan_array2.append("TURN,");
                    }
                    for (int j = 1; j < len - 1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(len-1));

                }else if (i == 1 && crossroad_number == 3) {
                    // 小车在第二条路线上，一共有三条路线
                    // ############ 待修改程序 ##############
                    path_plan_array2.append("TURN,");//伪节点，目的是让小车姿态调整为正向
                    path_plan_array2.append("RIGHT,");
                    for (int j = 1; j < len; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(len-1));

                } else if (i == 2) {
                    // ############ 待修改程序 ##############
                    // 小车在第三条路线上，一共有三条路线
                    path_plan_array2.append("TURN,");//伪节点，目的是让小车姿态调整为正向
                    path_plan_array2.append("RIGHT,");
                    for (int j = 1; j < len; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(len-1));

                }
                path_plan_array = path_plan_array2;
                qDebug() << "当前的路线命令为：" << path_plan_array;
                qDebug() << "小车期望朝向为正向，现在小车实际朝向就是反向，那么需要更改第一个路线命令，如下：";
                qDebug() << "路线命令索引为：" << this->index;
                qDebug() << "待执行路线命令：" << path_plan_array.split(',').at(this->index);
                return;
            }
        }
    }
    // 如果起始坐标位于岔路出口点上，并且小车为反向，路线命令第一个为“TURN”，第二个为“LEFT”或者“RIGHT”，那么不需要此处程序对路线命令进行重新规划
    else if (start_stop_node_array.split('\n').at(0) == crossraod_node_array.split(',').at(1) \
             && turltebot_direction == BACKWARD && path_plan_array.split(',').at(0) == "TURN" \
             && (path_plan_array.split(',').at(1) == "LEFT" || path_plan_array.split(',').at(1) == "RIGHT")) {
        qDebug() << "起始坐标位于岔路出口点上，并且小车为反向，路线命令第一个为“TURN”，第二个为“LEFT”或者“RIGHT”，那么需要此处程序对路线命令进行重新规划";
        qDebug() << "小车期望朝向为反向，现在小车实际朝向就是反向，那么需要跳过第一个路线命令TURN";
        this->index ++;
        qDebug() << "路线命令索引为：" << this->index;
        qDebug() << "待执行路线命令：" << path_plan_array.split(',').at(this->index);
        return;
    }
    // 如果起始坐标位于岔路出口点上，并且小车为反向，路线命令第一个为“UP”，那么需要此处程序对路线命令进行重新规划
    else if (start_stop_node_array.split('\n').at(0) == crossraod_node_array.split(',').at(1) \
             && turltebot_direction == BACKWARD && path_plan_array.split(',').at(0) == "UP") {
        qDebug() << "起始坐标位于岔路出口点上，并且小车为反向，路线命令第一个为“UP”，那么需要此处程序对路线命令进行重新规划";
        qDebug() << "小车期望朝向为正向，现在小车实际朝向是反向，那么需要第一个路线命令应该为TURN";
        QByteArray path_plan_array2;
        int len = path_plan_array.split(',').length();
        path_plan_array2.append("TURN,");
        for (int j = 0; j < len - 1; ++j) {
            path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
        }
        path_plan_array2.append(path_plan_array.split(',').at(len-1));
        path_plan_array = path_plan_array2;
        qDebug() << "路线命令索引为：" << this->index;
        qDebug() << "待执行路线命令：" << path_plan_array.split(',').at(this->index);
        return;
    }

    // 如果起始坐标位于岔路出口点上，并且小车为正向，路线命令第一个为“TURN”，第二个为“LEFT”或者“RIGHT”，那么需要此处程序对路线命令进行重新规划
    else if (start_stop_node_array.split('\n').at(0) == crossraod_node_array.split(',').at(1) \
             && turltebot_direction == FORWARD && path_plan_array.split(',').at(0) == "TURN" \
             && (path_plan_array.split(',').at(1) == "LEFT" || path_plan_array.split(',').at(1) == "RIGHT")) {
        //        qDebug() << "如果起始坐标位于岔路出口点上，并且小车为正向，路线命令第一个为“TURN”，第二个为“LEFT”或者“RIGHT”，那么需要此处程序对路线命令进行重新规划";
        if (path_plan_array.split(',').at(0) == "LEFT") {
            qDebug() << "如果起始坐标位于岔路出口点上，并且小车为正向，路线命令第一个为“TURN”，第二个为“LEFT”，那么需要修正第二个参数";
        } else {
            qDebug() << "如果起始坐标位于岔路出口点上，并且小车为正向，路线命令第一个为“TURN”，第二个为“RIGHT”，那么需要修正第二个参数";
        }
        for (int i = 0; i < crossroad_number; ++i) {
            if (QRData_near_cross == crossraod_node_near_array.split('\n').at(i).split(',').at(1)) {
                qDebug() << "当前小车停靠在第" + QString(i + 1) + "条路线上";
                QByteArray path_plan_array2;
                int len = path_plan_array.split(',').length();
                if (i == 0) {
                    //小车在第一条路线上
                    // 当前小车朝向为正向，由于执行此次路线命令需要改变小车朝向，根据规则，抛弃第一个命令，将第二个命令做一下修正
                    if (path_plan_array.split(',').at(1) == "RIGHT") {
                        path_plan_array2.append("TURN,");
                    } else if (path_plan_array.split(',').at(1) == "LEFT"){
                        path_plan_array2.append("RIGHT,");
                    }
                    for (int j = 2; j < len - 1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(len-1));

                }else if(i == 1 && crossroad_number == 2) {
                    // 小车在第二条路线上，一共有两条路线
                    if (path_plan_array.split(',').at(1) == "RIGHT") {
                        path_plan_array2.append("LEFT,");
                    } else if (path_plan_array.split(',').at(1) == "LEFT"){
                        path_plan_array2.append("TURN,");
                    }
                    for (int j = 2; j < len - 1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(len-1));

                }else if (i == 1 && crossroad_number == 3) {
                    // 小车在第二条路线上，一共有三条路线
                    // ############ 待修改程序 ##############
                    path_plan_array2.append("TURN,");//伪节点，目的是让小车姿态调整为正向
                    path_plan_array2.append("RIGHT,");
                    for (int j = 1; j < len; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(len-1));

                } else if (i == 2) {
                    // ############ 待修改程序 ##############
                    // 小车在第三条路线上，一共有三条路线
                    path_plan_array2.append("TURN,");//伪节点，目的是让小车姿态调整为正向
                    path_plan_array2.append("RIGHT,");
                    for (int j = 1; j < len; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(len-1));

                }
                path_plan_array = path_plan_array2;
                qDebug() << "小车期望朝向为反向，现在小车实际朝向是正向，那么需要修改道路命令";
                qDebug() << "当前的路线命令为：" << path_plan_array;
                qDebug() << "路线命令索引为：" << this->index;
                qDebug() << "待执行路线命令：" << path_plan_array.split(',').at(this->index);
                return;
            }
        }
    }
    // 如果起始坐标位于岔路出口点上，并且小车为正向，路线命令第一个为“UP”
    else if(start_stop_node_array.split('\n').at(0) == crossraod_node_array.split(',').at(1) \
            && turltebot_direction == FORWARD && path_plan_array.split(',').at(0) == "UP"){
        qDebug() << "起始坐标位于岔路出口点上，并且小车为正向，路线命令第一个为“UP”";
        for (int i = 0; i < crossroad_number; ++i) {
            //当小车目的地恰好为岔道口时
            if (QRData_near_cross == crossraod_node_near_array.split('\n').at(i).split(',').at(1)) {

                qDebug() << "当前小车停靠在第" + QString(i + 1) + "条路线上";
                QByteArray path_plan_array2;
                int k = path_plan_array.split(',').length();

                if (i == 0) {
                    //小车在第一条路线上
                    // UP,...   --->   LEFT,UP...
                    path_plan_array2.append("LEFT,");
                    for (int j = 0; j < k-1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(k-1));
                } else if (i == 1 && crossroad_number == 2) {
                    // 小车在第二条路线上，一共有两条路线
                    // UP,...   --->   RIGHT,UP...
                    path_plan_array2.append("RIGHT,");
                    for (int j = 0; j < k-1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(k-1));
                } else if (i == 1 && crossroad_number == 3) {
                    // 小车在第二条路线上，一共有三条路线
                    qDebug() << "小车在第二条路线上，一共有三条路线，不需要更新路线命令";
                } else if (i == 2) {
                    // 小车在第三条路线上，一共有三条路线
                    // TURN,UP,...   --->   TURN,LEFT,UP...
                    for (int j = 0; j < k-1; ++j) {
                        path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
                        if (j == 0) {
                            path_plan_array2.append("LEFT,");
                        }
                    }
                    path_plan_array2.append(path_plan_array.split(',').at(k-1));
                }
                path_plan_array = path_plan_array2;
                qDebug() << "小车期望朝向为正向，现在小车实际朝向是正向，但小车目前在岔道口内，需要修改路线命令";
                qDebug() << "当前的路线命令为：" << path_plan_array;
                qDebug() << "路线命令索引为：" << this->index;
                qDebug() << "待执行路线命令：" << path_plan_array.split(',').at(this->index);
                return;
            }
        }
    }

    // 小车为反向，第一个命令为TURN
    else if(turltebot_direction == BACKWARD && path_plan_array.split(',').at(0) == "TURN"){
        qDebug() << "小车期望朝向为反向，现在小车实际朝向是反向，需要跳过第一个TURN命令";
        qDebug() << "当前的路线命令为：" << path_plan_array;
        this->index ++;
        qDebug() << "路线命令索引为：" << this->index;
        qDebug() << "待执行路线命令：" << path_plan_array.split(',').at(this->index);
        return;
    }
    // 小车为反向，第一个命令为UP
    else if(turltebot_direction == BACKWARD && path_plan_array.split(',').at(0) == "UP"){

        QByteArray path_plan_array2;
        int len = path_plan_array.split(',').length();
        path_plan_array2.append("TURN,");
        for (int j = 0; j < len - 1; ++j) {
            path_plan_array2.append(path_plan_array.split(',').at(j) + ',');
        }
        path_plan_array2.append(path_plan_array.split(',').at(len-1));
        path_plan_array = path_plan_array2;

        qDebug() << "小车期望朝向为正向，现在小车实际朝向是反向，需要在路线第一个命令处增加一个TURN命令";
        qDebug() << "当前的路线命令为：" << path_plan_array;
        qDebug() << "路线命令索引为：" << this->index;
        qDebug() << "待执行路线命令：" << path_plan_array.split(',').at(this->index);
        return;
    }

}

void Camera::turn_tail_start(){
    if (turn == 1) {
        emit show_tutlebot_status(TURLTEBOT_TURN);
        lock_status(TURLTEBOT_TURN);
    }
    if (timer_turn_flag == true) {
        timer_turn->start();
        timer_turn_flag = false;
    }
}

void Camera::on_timer_turn_tail(){

    emit turltebot_turn(0.0, 3.14159 / 3.0);

    on_turn_tail_counter ++;
    // Timer is 200ms, 15 * 200ms = 3s
    if (on_turn_tail_counter >= 20) {
        timer_turn->stop();
        timer_turn_flag = true;
        on_turn_tail_counter = 0;
        qDebug() << "Update path command";
        qDebug() << "The next path command is:" << path_plan_array.split(',').at(index);
        qDebug() << "小乌龟改变了行驶方向";
        flag_on_timer_turn_tail_over = true;
    }
}

bool Camera::turn_tail(qint16 index)
{
    /*
     * 掉头规则：
     * 在小车朝向为反向的条件下：(判断是否改变小车朝向)
     *      1. 若检测到第一个命令为“TURN”:(判断是否改变小车朝向)
     *          a. 若后续命令中仅有“UP”命令 +++
     *              A.this->index ++ B.不掉头 C.不改变小车朝向，小车朝向仍为反向
     *          b. 若检测到后续岔道口入口点命令为“LEFT”或“RIGHT”
     *              a) 若小车在第一行
     *                  1) 命令为“LEFT”，那么 A. 改变小车朝向，小车朝向为反向
     *                  2) 命令为“RIGHT”，那么 不做任何改动
     *              b) 若小车在第二行：
     *                  1) 命令为“LEFT”，那么 不做任何改动
     *                  2) 命令为“RIGHT”，那么 A. 改变小车朝向，小车朝向为反向
     *      2. 若检测到第一个命令不为“TURN”：(改变小车朝向)
     *          a. 若第一个命令为“UP”，那么 A.掉头 B.改变小车朝向，小车朝向为正向
     *          b. 若第一个命令为“LEFT”或“RIGHT”，那么 A. 改变小车朝向，小车朝向为正向
     * 在小车朝向为正向的条件下：
     *      1. 若检测到第一个命令为“TURN”：(判断是否改变小车朝向)
     *          a. 若后续命令中仅有“UP”命令 +++
     *              A.this->index ++ B.掉头 C.改变小车朝向，小车朝向为反向
     *          b. 若检测到后续岔道口入口点命令为“LEFT”或“RIGHT”
     *              a) 若小车在第一行，在岔道口出口右边(不包含岔道口)
     *                  1) 检测到第一个转向命令为“LEFT”
     *                      a> 第二个为“RIGHT”  A. 不改变小车朝向，朝向正向 B. 掉头 C. this->index ++
     *                      b> 第二个为“LEFT”   A. 改变小车朝向，朝向反向 B. 掉头 C. this->index ++
     *                  2) 检测到第一个转向命令为“RIGHT”
     *                      a> 第二个为“RIGHT”  A. 改变小车朝向，朝向反向 B. 掉头 C. this->index ++
     *                      b> 第二个为“LEFT”   A. 不改变小车朝向，朝向正向 B. 掉头 C. this->index ++
     *              b) 若小车在第二行：
     *                  1) 转向命令为“LEFT”     A. 改变小车朝向，朝向反向 B. 掉头 C. this->index ++
     *                  2) 转向命令为“RIGHT”    A. 改变小车朝向，小车朝向为正向 B. 掉头 C. this->index ++
     *      2. 若检测到第一个命令不为“TURN”：(判断是否改变小车朝向)
     *          a. 若检测到后续岔道口出口点命令为“LEFT”或“RIGHT”
     *              a) 若小车在第一行
     *                  1) 命令为“LEFT”，那么 不做任何改动
     *                  2) 命令为“RIGHT”，那么 A. 改变小车朝向，小车朝向为反向
     *              b) 若小车在第二行：
     *                  1) 命令为“LEFT”，那么 A. 改变小车朝向，小车朝向为反向
     *                  2) 命令为“RIGHT”，那么 不做任何改动
     */
    int i = 0;
    if(i != 0){
        if (turltebot_direction == BACKWARD) {
            qDebug() << "小车当前朝向为反向";
            if (path_plan_array.split(',').at(0) == "TURN" && flag_first_turn_tail == true) {
                qDebug() << "路线第一个命令为“TURN”";
                //第 1 行的节点为： "1,2,3,4,5,6,"
                //第 2 行的节点为： "7,10,9,"
                int crossroad_coord_start = path_node_distribute_info.split('\n').at(0).split(',').indexOf(\
                            crossraod_node_array.split(',').at(0));
                int crossroad_coord_end = path_node_distribute_info.split('\n').at(0).split(',').indexOf(\
                            crossraod_node_array.split(',').at(1));
                qDebug() << "岔道口入口坐标：" << crossroad_coord_start;
                qDebug() << "岔道口出口坐标：" << crossroad_coord_end;
                qDebug() << "第二行节点数量为：" << path_node_distribute_info.split('\n').at(1).split(',').length()-1;
                //判断起点位于岔道口入口和出口之间，且在第一行
                //遍历第一行节点：第 1 行的节点为： "1,2,3,4,5,6,"
                for (int i = crossroad_coord_start + 1; i < path_node_distribute_info.split('\n').at(0).split(',').length()-1; ++i) {
                    if (start_stop_node_array.split('\n').at(0) == \
                            path_node_distribute_info.split('\n').at(0).split(',').at(i)) {
                        qDebug() << "小车为反向，起点位于岔道口入口右边(不含岔道口)，且在第一行，起始点坐标为：" << i;
                        /*
                     * 小车为反向
                     *      1.路线第一个命令为“TURN”，小车期望朝向为反向，但实际朝向为反向，因此这里不需要掉头
                     *      (A.不需要执行掉头命令 B.this->index++(跳过TURN指令) C.接下来根据不同情况，调整小车朝向)
                     *          a.起始坐标在岔道口内(不含岔道口)
                     *              a)后续只有一个转向命令(LEFT或RIGHT)
                     *                  1)小车在第一行
                     *                      a>转向命令为LEFT     C.调整朝向，调为正向
                     *                      b>转向命令为RIGHT    C.不调整朝向，朝向反向
                     *                  2)小车在第二行
                     *                      a>转向命令为LEFT     C.不调整朝向，朝向反向
                     *                      b>转向命令为RIGHT    C.调整朝向，调为正向
                     *              b)后续有两个转向命令
                     *                  1)小车在第一行
                     *                      (a)第一个转向命令为LEFT
                     *                          a>第二个转向命令为LEFT     C.不调整朝向，朝向反向
                     *                          b>第二个转向命令为RIGHT    C.调整朝向，调为正向
                     *                      (b)第一个转向命令为RIGHT
                     *                          ------------------      C.不调整朝向，朝向反向
                     *                  2)小车在第二行
                     *                      (a)第一个转向命令为LEFT
                     *                          ------------------      C.不调整朝向，朝向反向
                     *                      (b)第一个转向命令为RIGHT
                     *                          a>第二个转向命令为LEFT     C.调整朝向，调为正向
                     *                          b>第二个转向命令为RIGHT    C.不调整朝向，朝向反向
                     *              c)后续没有转向命令
                     *                   --------------------------     C.调整朝向，调为正向
                     *
                     *           b.起始坐标不在在岔道口内(不含岔道口)，那么小车坐标肯定在第一行上面
                     *              a)后续只有一个转向命令(LEFT或RIGHT)
                     *                          ------------------      C.不调整朝向，朝向反向
                     *              b)后续有两个转向命令
                     *                  (a)第一个转向命令为LEFT
                     *                      a>第二个转向命令为LEFT     C.不调整朝向，朝向反向
                     *                      b>第二个转向命令为RIGHT    C.调整朝向，调为正向
                     *                  (b)第一个转向命令为RIGHT
                     *                      a>第二个转向命令为LEFT     C.调整朝向，调为正向
                     *                      b>第二个转向命令为RIGHT    C.不调整朝向，朝向反向
                     *
                     *
                     *      2.路线第一个命令不为“TURN”，小车期望朝向为正向，但实际朝向为反向，因此这里需要掉头
                     *      (A.需要执行掉头命令 B.不执行this->index++(没有TURN指令跳过) C.接下来根据不同情况，调整小车朝向)
                     *          a.起始坐标在岔道口内(不含岔道口)
                     *              a)后续只有一个转向命令(LEFT或RIGHT) --- 掉头完成后重置朝向 ---
                     *                  1)小车在第一行
                     *                      a>转向命令为LEFT     C.调整朝向，调为正向
                     *                      b>转向命令为RIGHT    C.不调整朝向，朝向反向
                     *                  2)小车在第二行
                     *                      a>转向命令为LEFT     C.不调整朝向，朝向反向
                     *                      b>转向命令为RIGHT    C.调整朝向，调为正向
                     *              b)后续有两个转向命令
                     *                  1)小车在第一行
                     *                      (a)第一个转向命令为LEFT
                     *                           ------------------      C.调整朝向，调为正向
                     *                      (b)第一个转向命令为RIGHT
                     *                          a>第二个转向命令为LEFT     C.不调整朝向，朝向反向
                     *                          b>第二个转向命令为RIGHT    C.调整朝向，调为正向
                     *                  2)小车在第二行
                     *                      (a)第一个转向命令为LEFT
                     *                          a>第二个转向命令为LEFT     C.调整朝向，调为正向
                     *                          b>第二个转向命令为RIGHT    C.不调整朝向，朝向反向
                     *                      (b)第一个转向命令为RIGHT
                     *                          ------------------      C.调整朝向，调为正向
                     *              c)后续没有转向命令
                     *                   --------------------------     C.调整朝向，调为正向
                     *
                     *           b.起始坐标不在在岔道口内(不含岔道口)，那么小车坐标肯定在第一行上面
                     *              a)后续只有一个转向命令(LEFT或RIGHT)
                     *                          ------------------      C.调整朝向，朝向正向
                     *              b)后续有两个转向命令
                     *                  (a)第一个转向命令为LEFT
                     *                      a>第二个转向命令为LEFT     C.调整朝向，调为正向
                     *                      b>第二个转向命令为RIGHT    C.不调整朝向，朝向反向
                     *                  (b)第一个转向命令为RIGHT
                     *                      a>第二个转向命令为LEFT     C.不调整朝向，朝向反向
                     *                      b>第二个转向命令为RIGHT    C.调整朝向，调为正向
                     *              c)后续没有转向命令
                     *                          ------------------      C.调整朝向，朝向正向
                     * 小车为正向
                     *      1.路线第一个命令为“TURN”，小车期望朝向为反向，但实际朝向为正向，因此这里需要掉头
                     *      (A.需要执行掉头命令 B.掉头完成后this->index++ C.接下来根据不同情况，调整小车朝向)
                     *          a.起始坐标在岔道口内(不含岔道口)         --- 掉头完成后重置朝向 ---
                     *              a)后续只有一个转向命令(LEFT或RIGHT)
                     *                  1)小车在第一行
                     *                      a>转向命令为LEFT     C.调整朝向，调为反向
                     *                      b>转向命令为RIGHT    C.不调整朝向，朝向正向
                     *                  2)小车在第二行
                     *                      a>转向命令为LEFT     C.调整朝向，调为反向
                     *                      b>转向命令为RIGHT    C.不调整朝向，朝向正向
                     *              b)后续有两个转向命令
                     *                  1)小车在第一行
                     *                      (a)第一个转向命令为LEFT
                     *                          ------------------      C.调整朝向，调为反向
                     *                      (b)第一个转向命令为RIGHT
                     *                          a>转向命令为LEFT     C.不调整朝向，朝向正向
                     *                          b>转向命令为RIGHT    C.调整朝向，调为反向
                     *                  2)小车在第二行
                     *                      (a)第一个转向命令为LEFT
                     *                          ------------------      C.不调整朝向，朝向反向
                     *                      (b)第一个转向命令为RIGHT
                     *                          a>第二个转向命令为LEFT     C.调整朝向，调为正向
                     *                          b>第二个转向命令为RIGHT    C.不调整朝向，朝向反向
                     *              c)后续没有转向命令
                     *                   --------------------------     C.调整朝向，调为正向
                     *
                     *           b.起始坐标不在在岔道口内(不含岔道口)，那么小车坐标肯定在第一行上面
                     *              a)后续只有一个转向命令(LEFT或RIGHT)
                     *                          ------------------      C.不调整朝向，朝向反向
                     *              b)后续有两个转向命令
                     *                  (a)第一个转向命令为LEFT
                     *                      a>第二个转向命令为LEFT     C.不调整朝向，朝向反向
                     *                      b>第二个转向命令为RIGHT    C.调整朝向，调为正向
                     *                  (b)第一个转向命令为RIGHT
                     *                      a>第二个转向命令为LEFT     C.调整朝向，调为正向
                     *                      b>第二个转向命令为RIGHT    C.不调整朝向，朝向反向
                     *
                     *
                     *      2.路线第一个命令不为“TURN”，小车期望朝向为正向，但实际朝向为反向，因此这里需要掉头
                     *      (A.需要执行掉头命令 B.不执行this->index++(没有TURN指令跳过) C.接下来根据不同情况，调整小车朝向)
                     *          a.起始坐标在岔道口内(不含岔道口)
                     *              a)后续只有一个转向命令(LEFT或RIGHT)
                     *                  1)小车在第一行
                     *                      a>转向命令为LEFT     C.调整朝向，调为正向
                     *                      b>转向命令为RIGHT    C.不调整朝向，朝向反向
                     *                  2)小车在第二行
                     *                      a>转向命令为LEFT     C.不调整朝向，朝向反向
                     *                      b>转向命令为RIGHT    C.调整朝向，调为正向
                     *              b)后续有两个转向命令
                     *                  1)小车在第一行
                     *                      (a)第一个转向命令为LEFT
                     *                           ------------------      C.调整朝向，调为正向
                     *                      (b)第一个转向命令为RIGHT
                     *                          a>第二个转向命令为LEFT     C.不调整朝向，朝向反向
                     *                          b>第二个转向命令为RIGHT    C.调整朝向，调为正向
                     *                  2)小车在第二行
                     *                      (a)第一个转向命令为LEFT
                     *                          a>第二个转向命令为LEFT     C.调整朝向，调为正向
                     *                          b>第二个转向命令为RIGHT    C.不调整朝向，朝向反向
                     *                      (b)第一个转向命令为RIGHT
                     *                          ------------------      C.调整朝向，调为正向
                     *              c)后续没有转向命令
                     *                   --------------------------     C.调整朝向，调为正向
                     *
                     *           b.起始坐标不在在岔道口内(不含岔道口)，那么小车坐标肯定在第一行上面
                     *              a)后续只有一个转向命令(LEFT或RIGHT)
                     *                          ------------------      C.调整朝向，朝向正向
                     *              b)后续有两个转向命令
                     *                  (a)第一个转向命令为LEFT
                     *                      a>第二个转向命令为LEFT     C.调整朝向，调为正向
                     *                      b>第二个转向命令为RIGHT    C.不调整朝向，朝向反向
                     *                  (b)第一个转向命令为RIGHT
                     *                      a>第二个转向命令为LEFT     C.不调整朝向，朝向反向
                     *                      b>第二个转向命令为RIGHT    C.调整朝向，调为正向
                     *              c)后续没有转向命令
                     *                          ------------------      C.调整朝向，朝向正向
                     */





                        if (i > crossroad_coord_end) {
                            /*
                         * 如果起始点在岔道口出口右边(不包含岔道口出口),在命令长度下检测到，路线命令中出现过两次“转向”命令
                         * 1. 第一个为“LEFT”(下面第二个就是岔道口入口处命令)
                         *      a. 第二个为“LEFT”   A. 不改变小车朝向，朝向为反向 B. this->index ++
                         *      b. 第二个为“RIGHT”  A. 改变小车朝向，朝向为正向 B. this->index ++
                         * 2. 第一个为“RIGHT”(下面第二个就是岔道口入口处命令)
                         *      a. 第二个为“LEFT”   A. 改变小车朝向，朝向为正向 B. this->index ++
                         *      b. 第二个为“RIGHT”  A. 不改变小车朝向，朝向为反向 B. this->index ++
                         */
                            qDebug() << "起点坐标位于岔道口出口右边";
                            bool left_x_command_flag = false;
                            bool right_x_command_flag = false;
                            //有两个转向命令
                            for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                                if (path_plan_array.split(',').at(i) == "LEFT") {
                                    if (left_x_command_flag == false) {
                                        left_x_command_flag = true;
                                        continue;
                                    }
                                    if (path_plan_array.split(',').at(i) == "LEFT") {
                                        qDebug() << "第一个为“LEFT”，第二个为“LEFT，不掉头，不改变朝向，仍为反向";
                                        turltebot_direction = BACKWARD;
                                        flag_first_turn_tail = false;
                                        this->index ++;
                                        return false;
                                    } else if (path_plan_array.split(',').at(i) == "RIGHT") {
                                        qDebug() << "第一个为“LEFT”，第二个为“RIGHT，不掉头，改变朝向，朝向正向";
                                        turltebot_direction = FORWARD;
                                        flag_first_turn_tail = false;
                                        this->index ++;
                                        return false;
                                    }
                                }
                            }
                            for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                                if (path_plan_array.split(',').at(i) == "LEFT") {
                                    if (right_x_command_flag == false) {
                                        right_x_command_flag = true;
                                        continue;
                                    }
                                    if (path_plan_array.split(',').at(i) == "LEFT") {
                                        qDebug() << "第一个为“RIGHT”，第二个为“LEFT”，不掉头，改变朝向，朝向正向";
                                        turltebot_direction = FORWARD;
                                        flag_first_turn_tail = false;
                                        this->index ++;
                                        return false;
                                    } else if (path_plan_array.split(',').at(i) == "RIGHT") {
                                        qDebug() << "第一个为“RIGHT”，第二个为“RIGHT”，不掉头，不改变朝向，朝向反向";
                                        turltebot_direction = BACKWARD;
                                        flag_first_turn_tail = false;
                                        this->index ++;
                                        return false;
                                    }
                                }
                            }
                            //仅有一个转向命令
                            for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                                if (path_plan_array.split(',').at(i) == "LEFT" || \
                                        path_plan_array.split(',').at(i) == "RIGHT") {
                                    qDebug() << "小车为反向，仅有一个转向命令，命令为“LEFT”或“RIGHT”，不掉头，不改变朝向，朝向反向";
                                    turltebot_direction = BACKWARD;
                                    flag_first_turn_tail = false;
                                    this->index ++;
                                    return false;
                                }
                            }
                            //没有转向命令
                            qDebug() << "小车为反向，没有转向命令，不掉头，不改变朝向，朝向反向";
                            turltebot_direction = BACKWARD;
                            flag_first_turn_tail = false;
                            return false;
                        }
                        else if(crossroad_coord_start < i && i < crossroad_coord_end){
                            qDebug() << "起点坐标位于岔道口内";
                            for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                                if (path_plan_array.split(',').at(j) == "LEFT") {
                                    qDebug() << "第一个为“LEFT”，不掉头，改变朝向，朝向正向";
                                    turltebot_direction = FORWARD;
                                    flag_first_turn_tail = false;
                                    this->index ++;
                                    return false;
                                }
                                else if (path_plan_array.split(',').at(j) == "RIGHT") {
                                    qDebug() << "第一个为“RIGHT”，不掉头，不改变朝向，朝向反向";
                                    turltebot_direction = BACKWARD;
                                    flag_first_turn_tail = false;
                                    this->index ++;
                                    return false;
                                }
                            }
                            //没有转向命令
                            qDebug() << "小车为反向，没有转向命令，不掉头，不改变朝向，朝向反向";
                            turltebot_direction = BACKWARD;
                            flag_first_turn_tail = false;
                            return false;
                        }
                    }
                }
                //遍历第二行节点：第 2 行的节点为： "7,10,9,"
                for (int i = 0; i < path_node_distribute_info.split('\n').at(1).split(',').length()-1; ++i) {
                    if (start_stop_node_array.split('\n').at(0) == \
                            path_node_distribute_info.split('\n').at(1).split(',').at(i)) {
                        qDebug() << "小车为反向，起点位于岔道口入口和出口之间(不含岔道口)，且在第二行";
                        for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                            if (path_plan_array.split(',').at(j) == "RIGHT") {
                                turltebot_direction = FORWARD;
                                flag_first_turn_tail = false;
                                this->index ++;
                                return false;
                            }
                            else if (path_plan_array.split(',').at(j) == "LEFT") {
                                turltebot_direction = BACKWARD;
                                flag_first_turn_tail = false;
                                this->index ++;
                                return false;
                            }
                        }
                        qDebug() << "无需调整3";
                        flag_first_turn_tail = false;
                        return false;
                    }
                }
                qDebug() << "无需调整";
                flag_first_turn_tail = false;
                return false;
            }
            else if (path_plan_array.split(',').at(0) != "TURN" && flag_first_turn_tail == true) {
                if (path_plan_array.split(',').at(0) == "UP") {
                    //A.掉头 B.改变小车朝向，小车朝向为正向
                    if (flag_turn_tail_start == false) {
                        qDebug() << "开始掉头";
                        flag_on_timer_turn_tail_over = false;// 掉头完成标志位
                        turn_tail_start();
                        flag_turn_tail_start = true; // 保证在一次掉头动作中，完成一次开启定时器
                    }
                    if (flag_on_timer_turn_tail_over == true) {
                        flag_turn_tail_start = false; //为下一次定时器开启作准备
                        flag_first_turn_tail = false; //当前路线命令下，不再执行此逻辑下代码
                        turltebot_direction = FORWARD;
                        flag_first_turn_tail = false;
                        qDebug() << "小车朝向为正向，掉头完成";
                        return false;//完成掉头
                    } else {
                        return true;//没有完成掉头
                    }
                }
                else if (path_plan_array.split(',').at(0) == "LEFT" || path_plan_array.split(',').at(0) == "RIGHT") {
                    //A. 改变小车朝向，小车朝向为正向
                    turltebot_direction = FORWARD;
                    flag_first_turn_tail = false;
                    return false;
                }
            }
        }
        else if (turltebot_direction == FORWARD) {
            qDebug() << "小车当前朝向为正向";
            if (path_plan_array.split(',').at(0) == "TURN" && flag_first_turn_tail == true) {
                //检验路线命令除第一个为TURN，后续是否都为UP
                int up_command_number = 0;
                for (int i = 0; i < path_plan_array.split(',').length(); ++i) {
                    if (path_plan_array.split(',').at(i) == "UP") {
                        up_command_number ++;
                    }
                }
                if (up_command_number == path_plan_array.split(',').length() - 1) {
                    qDebug() << "除第一个为“TURN”，后续命令中仅有“UP”命令";
                    ////a. 若后续命令中仅有“UP”命令 +++
                    //A.this->index ++ B.掉头 C.改变小车朝向，小车朝向为反向
                    //------------------ A. this->index ++ B.掉头 C.改变小车朝向，小车朝向为反向 -----------------
                    if (flag_turn_tail_start == false) {
                        qDebug() << "后续命令中仅有“UP”命令，开始掉头";
                        flag_on_timer_turn_tail_over = false;// 掉头完成标志位
                        turn_tail_start();
                        flag_turn_tail_start = true; // 保证在一次掉头动作中，完成一次开启定时器
                    }
                    if (flag_on_timer_turn_tail_over == true) {
                        this->index ++;
                        turltebot_direction = BACKWARD;
                        flag_turn_tail_start = false; //为下一次定时器开启作准备
                        flag_first_turn_tail = false;
                        qDebug() << "小车朝向为正向，掉头完成";
                        return false;//完成掉头
                    } else {
                        return true;//没有完成掉头
                    }
                    //------------------ A. this->index ++ B.掉头 C.改变小车朝向，小车朝向为反向 -----------------
                }
                ////b. 若检测到后续岔道口入口点命令为“LEFT”或“RIGHT”
                //第 1 行的节点为： "1,2,3,4,5,6,"
                //第 2 行的节点为： "7,10,9,"
                int crossroad_coord_start = path_node_distribute_info.split('\n').at(0).split(',').indexOf(\
                            crossraod_node_array.split(',').at(0));
                int crossroad_coord_end = path_node_distribute_info.split('\n').at(0).split(',').indexOf(\
                            crossraod_node_array.split(',').at(1));
                qDebug() << "岔道口入口坐标：" << crossroad_coord_start;
                qDebug() << "岔道口出口坐标：" << crossroad_coord_end;
                qDebug() << "第二行节点数量为：" << path_node_distribute_info.split('\n').at(1).split(',').length()-1;

                bool turltebot_direction_tmp;//临时存储小车的朝向，等掉头需求完成后更新到turltebot_direction
                //遍历第一行节点：第 1 行的节点为： "1,2,3,4,5,6,"
                for (int i = crossroad_coord_start + 1; i < path_node_distribute_info.split('\n').at(0).split(',').length()-1; ++i) {
                    if (start_stop_node_array.split('\n').at(0) == \
                            path_node_distribute_info.split('\n').at(0).split(',').at(i)) {
                        qDebug() << "小车为正向，起点位于岔道口入口右边(不含岔道口)，且在第一行，起始点坐标为：" << i;

                        if (i > crossroad_coord_end) {
                            /*
                         * 如果起始点在岔道口出口右边(不包含岔道口出口),在命令长度下检测到，路线命令中出现过两次“转向”命令
                         * 1. 第一个为“LEFT”(下面第二个就是岔道口入口处命令)
                         *      a. 第二个为“LEFT”   A. 改变小车朝向，朝向为反向 B. 掉头 C.this->index ++
                         *      b. 第二个为“RIGHT”  A. 不改变小车朝向，朝向为正向 B. 掉头 C.this->index ++
                         * 2. 第一个为“RIGHT”(下面第二个就是岔道口入口处命令)
                         *      a. 第二个为“LEFT”   A. 不改变小车朝向，朝向为正向 B. 掉头 C.this->index ++
                         *      b. 第二个为“RIGHT”  A. 改变小车朝向，朝向为反向 B. 掉头 C.this->index ++
                         */
                            qDebug() << "起点坐标位于岔道口出口右边";
                            bool left_x_command_flag = false;
                            bool right_x_command_flag = false;
                            // 命令中有两次转向
                            for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                                if (path_plan_array.split(',').at(i) == "LEFT") {
                                    if (left_x_command_flag == false) {
                                        left_x_command_flag = true;
                                        continue;
                                    }
                                    if (path_plan_array.split(',').at(i) == "LEFT") {
                                        //第一个为“LEFT”，第二个为“LEFT”，掉头，改变朝向，朝向反向";
                                        turltebot_direction_tmp = BACKWARD;
                                    } else if (path_plan_array.split(',').at(i) == "RIGHT") {
                                        //第一个为“LEFT”，第二个为“RIGHT”，掉头，不改变朝向，朝向正向";
                                        turltebot_direction_tmp = FORWARD;
                                    }
                                }
                            }
                            for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                                if (path_plan_array.split(',').at(i) == "RIGHT") {
                                    if (right_x_command_flag == false) {
                                        right_x_command_flag = true;
                                        continue;
                                    }
                                    if (path_plan_array.split(',').at(i) == "LEFT") {
                                        //第一个为“RIGHT”，第二个为“LEFT”，掉头，不改变朝向，朝向正向";
                                        turltebot_direction = FORWARD;
                                    } else if (path_plan_array.split(',').at(i) == "RIGHT") {
                                        //第一个为“RIGHT”，第二个为“RIGHT”，掉头，改变朝向，朝向反向";
                                        turltebot_direction = BACKWARD;
                                    }
                                }
                            }
                            // 命令中只有一次转向
                            for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                                if (path_plan_array.split(',').at(i) == "LEFT") {
                                    if (left_x_command_flag == false) {
                                        left_x_command_flag = true;
                                        continue;
                                    }
                                    if (path_plan_array.split(',').at(i) == "LEFT") {
                                        //第一个为“LEFT”，第二个为“LEFT”，掉头，改变朝向，朝向反向";
                                        turltebot_direction_tmp = BACKWARD;
                                    } else if (path_plan_array.split(',').at(i) == "RIGHT") {
                                        //第一个为“LEFT”，第二个为“RIGHT”，掉头，不改变朝向，朝向正向";
                                        turltebot_direction_tmp = FORWARD;
                                    }
                                }
                            }
                            for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                                if (path_plan_array.split(',').at(i) == "RIGHT") {
                                    if (right_x_command_flag == false) {
                                        right_x_command_flag = true;
                                        continue;
                                    }
                                    if (path_plan_array.split(',').at(i) == "LEFT") {
                                        //第一个为“RIGHT”，第二个为“LEFT”，掉头，不改变朝向，朝向正向";
                                        turltebot_direction = FORWARD;
                                    } else if (path_plan_array.split(',').at(i) == "RIGHT") {
                                        //第一个为“RIGHT”，第二个为“RIGHT”，掉头，改变朝向，朝向反向";
                                        turltebot_direction = BACKWARD;
                                    }
                                }
                            }
                        }
                        else if(crossroad_coord_start < i && i < crossroad_coord_end){
                            qDebug() << "起点坐标位于岔道口内";
                            for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                                if (path_plan_array.split(',').at(j) == "LEFT") {
                                    qDebug() << "转向命令为“LEFT”，掉头，不改变朝向，朝向正向";
                                    turltebot_direction_tmp = FORWARD;
                                }
                                else if (path_plan_array.split(',').at(j) == "RIGHT") {
                                    qDebug() << "转向命令为“RIGHT”，掉头，改变朝向，朝向反向";
                                    turltebot_direction_tmp = BACKWARD;
                                }
                            }
                        }
                    }
                }
                //遍历第二行节点：第 2 行的节点为： "7,10,9,"
                for (int i = 0; i < path_node_distribute_info.split('\n').at(1).split(',').length()-1; ++i) {
                    if (start_stop_node_array.split('\n').at(0) == \
                            path_node_distribute_info.split('\n').at(1).split(',').at(i)) {
                        qDebug() << "小车为正向，起点位于岔道口入口和出口之间(不含岔道口)，且在第二行";
                        for (int j = 0; j < path_plan_array.split(',').length(); ++j) {
                            if (path_plan_array.split(',').at(j) == "RIGHT") {
                                turltebot_direction_tmp = FORWARD;
                            }
                            else if (path_plan_array.split(',').at(j) == "LEFT") {
                                turltebot_direction_tmp = BACKWARD;
                            }
                        }
                    }
                }
                //------------------------ 掉头 + this->index ++ ----------------------------------
                if (flag_turn_tail_start == false) {
                    qDebug() << "后续命令中有转向命令，开始掉头";
                    flag_on_timer_turn_tail_over = false;// 掉头完成标志位
                    turn_tail_start();
                    flag_turn_tail_start = true; // 保证在一次掉头动作中，完成一次开启定时器
                }
                if (flag_on_timer_turn_tail_over == true) {
                    this->index ++;
                    turltebot_direction = turltebot_direction_tmp;
                    flag_turn_tail_start = false; //为下一次定时器开启作准备
                    flag_first_turn_tail = false;
                    qDebug() << "小车朝向为正向，掉头完成";
                    return false;//完成掉头
                } else {
                    qDebug() << "正在掉头...";
                    return true;//没有完成掉头
                }
                //------------------------ 掉头 + this->index ++ ----------------------------------
            }
            else if(path_plan_array.split(',').at(0) != "TURN" && flag_first_turn_tail == true){
                /*
             *路线命令第一个不是TURN，后续命令中行驶到岔道口处的命令恰好为“LEFT”或“RIGHT”，则需要考虑是否要调整朝向
             * e.g UP,UP,LEFT,UP
             * 1. 首先确定小车在第几行上面
             *      a. 若在第一行
             *          a) 判断起点位于岔道口入口左边(不含岔道口)
             *              不改变朝向
             *          b) 判断起点位于岔道口入口和出口之间(不含岔道口)
             *              1) 检测到岔道口出口处的命令为“RIGHT”
             *                  改变朝向，小车朝向为反向
             *              2) 检测到岔道口出口处的命令为“LEFT”
             *                  不改变朝向，小车朝向为正向
             *          c) 判断起点位于岔道口出口右边(不含岔道口)
             *              不改变朝向
             *      a. 若在第二行
             *          起点位于岔道口入口和出口之间(不含岔道口)
             *              1) 检测到岔道口出口处的命令为“RIGHT”
             *                  不改变朝向，小车朝向为正向
             *              2) 检测到岔道口出口处的命令为“LEFT”
             *                  改变朝向，小车朝向为反向s
             *
             */
                //第 1 行的节点为： "1,2,3,4,5,6,"
                //第 2 行的节点为： "7,10,9,"
                int crossroad_coord_start = path_node_distribute_info.split('\n').at(0).split(',').indexOf(\
                            crossraod_node_array.split(',').at(0));
                int crossroad_coord_end = path_node_distribute_info.split('\n').at(0).split(',').indexOf(\
                            crossraod_node_array.split(',').at(1));
                qDebug() << "岔道口入口坐标：" << crossroad_coord_start;
                qDebug() << "岔道口出口坐标：" << crossroad_coord_end;
                qDebug() << "第二行节点数量为：" << path_node_distribute_info.split('\n').at(1).split(',').length()-1;
                //判断起点位于岔道口入口和出口之间，且在第一行
                //遍历第一行节点：第 1 行的节点为： "1,2,3,4,5,6,"
                for (int i = crossroad_coord_start + 1; i < crossroad_coord_end; ++i) {
                    if (start_stop_node_array.split('\n').at(0) == \
                            path_node_distribute_info.split('\n').at(0).split(',').at(i)) {
                        qDebug() << "起点位于岔道口入口和出口之间(不含岔道口)，且在第一行";
                        for (int j = 0; j < path_plan_array.length(); ++j) {
                            if (path_plan_array.split(',').at(j) == "RIGHT") {
                                turltebot_direction = BACKWARD;
                                flag_first_turn_tail = false;
                                return false;
                            }
                            else if (path_plan_array.split(',').at(j) == "LEFT") {
                                turltebot_direction = FORWARD;
                                flag_first_turn_tail = false;
                                return false;
                            }
                        }
                        qDebug() << "无需调整33";
                        flag_first_turn_tail = false;
                        return false;
                    }
                }
                //遍历第二行节点：第 2 行的节点为： "7,10,9,"
                for (int i = 0; i < path_node_distribute_info.split('\n').at(1).split(',').length()-1; ++i) {
                    if (start_stop_node_array.split('\n').at(0) == \
                            path_node_distribute_info.split('\n').at(1).split(',').at(i)) {
                        qDebug() << "起点位于岔道口入口和出口之间(不含岔道口)，且在第二行";
                        qDebug() << "path_plan_array" << path_plan_array;
                        qDebug() << "path_plan_array len" << path_plan_array.split(',').length();
                        qint16 kkk;
                        for (kkk = 0; kkk < path_plan_array.split(',').length(); kkk++) {
                            qDebug() << "path_plan_array" << path_plan_array.split(',').at(kkk);
                            if (path_plan_array.split(',').at(kkk) == "RIGHT") {
                                turltebot_direction = FORWARD;
                                flag_first_turn_tail = false;
                                return false;
                            }
                            else if (path_plan_array.split(',').at(kkk) == "LEFT") {
                                turltebot_direction = BACKWARD;
                                flag_first_turn_tail = false;
                                return false;
                            }
                        }
                        qDebug() << "无需调整4";
                        flag_first_turn_tail = false;
                        return false;
                    }
                }
                qDebug() << "无需调整5";
                flag_first_turn_tail = false;
                return false;
            }
        }
    }
    // 除路线命令第一个以外，若再有TURN，则按照如下逻辑进行掉头，若没有则返回掉头完成命令
    if (path_plan_array.split(',').at(index) == "TURN" /*&& flag_first_turn_tail == false*/) {
        //        qDebug() << "除路线命令第一个以外，若再有TURN，则按照如下逻辑进行掉头";
        // 如果当前小车命令为TURN，执行三步：1. 掉头 2. this->index++
        if (flag_turn_tail_start == false) {
            flag_on_timer_turn_tail_over = false;// 掉头完成标志位
            turn_tail_start();
            flag_turn_tail_start = true; // 保证在一次掉头动作中，完成一次开启定时器
        }
        if (flag_on_timer_turn_tail_over == true) {
            flag_turn_tail_start = false; //为下一次定时器开启作准备
            this->index ++;
            //            turltebot_direction = !turltebot_direction; //改变小车朝向
            qDebug() << "完成掉头";
            return false;//完成掉头
        } else {
            qDebug() << "正在掉头...";
            return true;//没有完成掉头
        }
    } else {
        return false;
    }
}

void Camera::turltebot_direction_judgement(){
    /*
     * 通过检测当前QR值和上一个QR值来实时监控小车朝向
     * 1.如果检测到相邻QR值所在坐标都在第一行
     *      a.当前QR坐标在上一次QR坐标右边，那么，判定小车一定是正向
     *      b.当前QR坐标是第一行的第一个，上一次QR坐标为第一行的最后一个，那么，判定小车一定是正向
     *      c.当前QR坐标在上一次QR坐标左边，那么，判定小车一定是反向
     *      d.当前QR坐标是第一行的最后一个，上一次QR坐标为第一行的第一个，那么，判定小车一定是反向
     * 2.如果检测到相邻QR值所在坐标都在第二行
     *      a.当前QR坐标在上一次QR坐标右边，那么，判定小车一定是正向
     *      b.当前QR坐标在上一次QR坐标左边，那么，判定小车一定是反向
     * 3.如果检测到相邻QR值所在坐标在不同行
     *      a.当前QR坐标在第二行，上一次QR坐标在第一行
     *          a>上一次QR值为岔道口入口节点，那么判定小车为正向
     *          b>上一次QR值为岔道口出口节点，那么判定小车为反向
     *      b.如果当前QR坐标在第一行，上一次QR坐标在第二行
     *          a>当前QR值为岔道口入口节点，那么判定小车为反向
     *          b>当前QR值为岔道口出口节点，那么判定小车为正向
     *
    */
    if (QRData_store_index > 0) {
        //QRData_store_index > 0 时才能保证已经经过两个节点了，可以判断小车当前朝向
        QString QRData_current_str = QRData_store.split(',').at(QRData_store_index);
        QString QRData_last_str= QRData_store.split(',').at(QRData_store_index - 1);
        QByteArray QRData_current = QRData_current_str.toUtf8();//QString 转 QByterArray
        QByteArray QRData_last = QRData_last_str.toUtf8();
        qint16 path_node_1st_line_max_index = path_node_distribute_info.split('\n').at(0).length() - 2;
        qint16 path_node_2nd_line_max_index = path_node_distribute_info.split('\n').at(1).length() - 2;
        //1.如果检测到相邻QR值所在坐标都在第一行
        if (path_node_distribute_info.split('\n').at(0).split(',').contains(QRData_current) == true &&\
                path_node_distribute_info.split('\n').at(0).split(',').contains(QRData_last) == true) {

            if (path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_current) == 0 &&\
                    path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_last) == path_node_1st_line_max_index) {
                qDebug() << "当前QR坐标是第一行的第一个，上一次QR坐标为第一行的最后一个，那么，判定小车一定是正向";
                turltebot_direction = FORWARD;
                return;
            }
            else if (path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_current) == path_node_1st_line_max_index &&\
                     path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_last) == 0) {
                qDebug() << "当前QR坐标是第一行的最后一个，上一次QR坐标为第一行的第一个，那么，判定小车一定是反向";
                turltebot_direction = BACKWARD;
                return;
            }
            else if (path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_current) >\
                     path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_last)) {
                qDebug() << "当前QR坐标在上一次QR坐标右边，那么，判定小车一定是正向";
                turltebot_direction = FORWARD;
                return;
            }
            else if (path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_current) <\
                     path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_last)) {
                qDebug() << "当前QR坐标在上一次QR坐标左边，那么，判定小车一定是反向";
                turltebot_direction = BACKWARD;
                return;
            }

        }

        //2.如果检测到相邻QR值所在坐标都在第二行
        else if (path_node_distribute_info.split('\n').at(1).split(',').contains(QRData_current) == true &&\
                 path_node_distribute_info.split('\n').at(1).split(',').contains(QRData_last) == true) {

            if (path_node_distribute_info.split('\n').at(1).split(',').indexOf(QRData_current) >\
                    path_node_distribute_info.split('\n').at(1).split(',').indexOf(QRData_last)) {
                qDebug() << "都在第二行:当前QR坐标在上一次QR坐标右边，那么，判定小车一定是正向";
                turltebot_direction = FORWARD;
                return;
            }
            else if (path_node_distribute_info.split('\n').at(1).split(',').indexOf(QRData_current) <\
                     path_node_distribute_info.split('\n').at(1).split(',').indexOf(QRData_last)) {
                qDebug() << "都在第二行:当前QR坐标在上一次QR坐标左边，那么，判定小车一定是反向";
                turltebot_direction = BACKWARD;
                return;
            }
        }

        //3.如果检测到相邻QR值所在坐标在不同行
        else {

            //a.当前QR坐标在第二行，上一次QR坐标在第一行
            if (path_node_distribute_info.split('\n').at(1).split(',').contains(QRData_current) == true && \
                    path_node_distribute_info.split('\n').at(0).split(',').contains(QRData_last) == true) {
                qDebug() << "当前QR坐标在第二行，上一次QR坐标在第一行";
                if (QRData_last == crossraod_node_array.split(',').at(0)) {
                    qDebug() << "上一次QR值为岔道口入口节点，那么判定小车为正向";
                    turltebot_direction = FORWARD;
                    return;
                }
                else if(QRData_last == crossraod_node_array.split(',').at(1)){
                    qDebug() << "上一次QR值为岔道口出口节点，那么判定小车为反向";
                    turltebot_direction = BACKWARD;
                    return;
                }
            }
            //b.当前QR坐标在第一行，上一次QR坐标在第二行
            else if (path_node_distribute_info.split('\n').at(0).split(',').contains(QRData_current) == true && \
                     path_node_distribute_info.split('\n').at(1).split(',').contains(QRData_last) == true) {
                qDebug() << "当前QR坐标在第一行，上一次QR坐标在第二行";
                if (QRData_current == crossraod_node_array.split(',').at(0)) {
                    qDebug() << "当前QR值为岔道口入口节点，那么判定小车为反向";
                    turltebot_direction = BACKWARD;
                    return;
                }
                else if(QRData_current == crossraod_node_array.split(',').at(1)){
                    qDebug() << "当前QR值为岔道口出口节点，那么判定小车为正向";
                    turltebot_direction = FORWARD;
                    return;
                }
            }
        }
    }
}

void Camera::crossroad_plan()
{

    qDebug() << "[此处为岔道口]";
    this->speed_line = SPEED_LINE;
    this->speed_angular = this->speed_line * 4;
    flag_through_crossroad = false;
    qDebug() << "[岔道] 线速度为：" << speed_line << "m/s " << "角速度为：" << speed_angular << "rad/s";

    if(on_timer_through_crossroad_counter >= 20){
        if (timer_crossroad_start_flag == false) {
            timer_crossroad->stop();
            timer_crossroad_start_flag = true;
            on_timer_crossroad_qr_counter = 1;
            qDebug() << "on_timer_through_crossroad_counter is over:" << on_timer_through_crossroad_counter*200 << "ms" ;
        }

        qDebug() << "修正转弯路线.....................................................";
        if (path_plan_array.split(',').at(index) == "LEFT") {
            if ((len_frame / 2 - 30) < black_index_100[0]) {
                leave_crossroad();
                return;
            }
            if (left == 1) {
                emit show_tutlebot_status(TURLTEBOT_LEFT);
                lock_status(TURLTEBOT_LEFT);
            }
            emit turltebot_left(speed_line, speed_angular);
            qDebug() << "[保持左转...]";
        } else if (path_plan_array.split(',').at(index) == "RIGHT") {
            if ( black_index_100[black_count_100] < (len_frame / 2 + 30)) {
                leave_crossroad();
                return;
            }
            if (right == 1) {
                emit show_tutlebot_status(TURLTEBOT_RIGHT);
                lock_status(TURLTEBOT_RIGHT);
            }
            emit turltebot_right(speed_line, speed_angular);
            qDebug() << "[保持右转...]";
        } else if (path_plan_array.split(',').at(index) == "UP") {
            if ( black_index_100[black_count_100] - black_index_100[0] < 2 * PATH_STRAIGHT_ROAD_LEN) {
                leave_crossroad();
                return;
            }
            if (up == 1) {
                emit show_tutlebot_status(TURLTEBOT_UP);
                lock_status(TURLTEBOT_UP);
            }
            emit turltebot_up(speed_line, speed_angular);
            qDebug() << "[保持前进...]";
        }
    }

    if (timer_crossroad_start_flag == true && on_timer_crossroad_qr_counter == 0) {
        qDebug() << "开启岔道口定时器";
        timer_crossroad->start();
        timer_crossroad_start_flag = false;
    }
}

void Camera::on_timer_through_crossroad(){

    qDebug() << "转弯定时器";
    on_timer_through_crossroad_counter ++;

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

bool Camera::check_contains_cross(qint16 len)
{
    //    qDebug() << "on_timer_crossroad_qr_counter: " << on_timer_crossroad_qr_counter;
    //    qDebug() << "QRData_current_update        : " << QRData_current_update;
    //    qDebug() << "QRData_stable                : " << QRData_stable;
    if (on_timer_crossroad_qr_counter == 1 && \
            ( QRData_current_update == crossraod_node_array.split(',').at(0) || \
              QRData_current_update == crossraod_node_array.split(',').at(1) )) {
        //on_timer_crossroad_qr_counter只有在岔道口内转弯时置1，若该值1，则保持小车调整姿态状态
        return true;
    }
    if(QRData_current_update == crossraod_node_array.split(',').at(0) || \
            QRData_current_update == crossraod_node_array.split(',').at(1)     )
    {
        return true;
    } else {
        on_timer_crossroad_qr_counter = 0;
        return false;
    }
}

void Camera::leave_crossroad(){
    qDebug() << "\n...............................................................";
    qDebug() << "cross road over\n...............................................................\n";
    qDebug() << "Update current path command";
    index ++; // Update current path command;
    qDebug() << "leave_crossroad:index ++";
    QRData_stable.clear();
    qDebug() << "Next path command: " << path_plan_array.split(',').at(index);
    timer_crossroad->stop();
    timer_crossroad_start_flag = true;
    qDebug() << "on_timer_through_crossroad_counter is over:" << on_timer_through_crossroad_counter*200 << "ms" ;
    on_timer_through_crossroad_counter = 0;
    check_qr_contains_cross_road_node_counter = 0;
    check_qr_contains_cross_road_node_number = 0;
    flag_through_crossroad = true;
}

bool Camera::crossroad_check_black_center(){
    if ((len_frame / 2 - 20) < black_center && black_center < (len_frame / 2 + 20)){
        return true;
    } else {
        return false;
    }
}
bool Camera::check_qr_contains_cross_road_node()
{

#if 1
    //    check_qr_contains_cross_road_node_counter ++;
    //    if (check_qr_contains_cross_road_node_counter >= 750 / (SPEED_LINE * 100) ) {
    check_qr_contains_cross_road_node_counter ++;
    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    int width 	= grayImage.cols;
    int height 	= grayImage.rows/2;

    //Rect(x,y,w,h)
    // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
    Mat roi(grayImage, Rect(0,height,width,height));
    unsigned char *pdata = (unsigned char *)roi.data;
    zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
    int n = scanner.scan(imageZbar);
    if (n > 0){
        // indicate contains QR
        //        check_qr_contains_cross_road_node_number ++;
        qDebug() << "还在岔道里面";
        return true;
    } else {
        check_qr_contains_cross_road_node_blank ++;
        qDebug() << "离开岔道";
        return false;
    }
    if (check_qr_contains_cross_road_node_number > 0) {
        qDebug() << "还在岔道里面";
        return true;
    } else {
        qDebug() << "离开岔道";
        return false;
    }
#endif
}
void Camera::check_qr_contains_cross_road_node_code()
{
    //    check_qr_contains_cross_road_node_counter ++;
    //    if (check_qr_contains_cross_road_node_counter >= 750 / (SPEED_LINE * 100) ) {
    check_qr_contains_cross_road_node_counter ++;
    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    int width 	= grayImage.cols;
    int height 	= grayImage.rows/2;

    //Rect(x,y,w,h)
    // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
    Mat roi(grayImage, Rect(0,height,width,height));
    unsigned char *pdata = (unsigned char *)roi.data;
    zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
    int n = scanner.scan(imageZbar);
    if (n > 0){
        // indicate contains QR
        check_qr_contains_cross_road_node_number ++;
    }
}
void Camera::on_timer_crossroad_qr(){

}

void Camera::on_send_path_info_to_camera(QByteArray path_info)
{
    qDebug() << path_info;
    //"\x01,\x02,\x03,\x04,c,\x05,\x06,#\x00,\x00,\x07,\n,\t,\x00,\x00,#"
    //计算路线条数
    crossroad_number = path_info.split('#').length() - 1;
    qDebug() << "crossroad_number" << crossroad_number;
    //    qDebug() << path_info.split('#').at(1).split(',').length() - 1;
    //寻找岔道口坐标和岔道口内邻居坐标
    quint8 dat;
    if (crossroad_number > 1) {
        int crossroad_entry_index=0;
        int crossroad_exit_index=0;

        QByteArray arr_1;
        QByteArray arr_2;
        arr_1 = path_info.split('#').at(0);
        arr_2 = path_info.split('#').at(1);
        for (int i = 0; i < arr_2.length() - 1; i = i + 2) {
            dat = arr_2.at(i);
            if (dat != 0) {
                crossroad_entry_index = i-2;
                dat = arr_1.at(crossroad_entry_index);
                //                qDebug() << QString::number(dat,10);
                crossraod_node_array.append(QString::number(dat,10));
                break;
            }
        }
        crossraod_node_array.append(',');
        //\x00,\x00,\x07,\n,\t,\x00,\x00,
        for (int i = arr_2.length() - 2; i > 0; i = i - 2) {
            dat = arr_2.at(i);
            if (dat != 0) {
                crossroad_exit_index = i + 2;
                dat = arr_1.at(crossroad_exit_index);
                //                qDebug() << QString::number(dat,10);
                crossraod_node_array.append(QString::number(dat,10));
                break;
            }
        }
        qDebug() << "crossraod_node_array" << crossraod_node_array;
        //寻找岔路口内相邻坐标
        for (int i = 0; i < crossroad_number; ++i) {
            dat = path_info.split('#').at(i).at(crossroad_entry_index + 2);
            crossraod_node_near_array.append(QString::number(dat,10));
            crossraod_node_near_array.append(',');
            for (int j = crossroad_exit_index - 2; j >= 0; j = j - 2) {
                dat = path_info.split('#').at(i).at(j);
                if (dat == 99) {
                    continue;
                }
                crossraod_node_near_array.append(QString::number(dat,10));
                break;
            }
            crossraod_node_near_array.append('\n');
        }
        qDebug() << "crossraod_node_near_array" << crossraod_node_near_array;
    }

    //将所有节点都保存下来
    path_node_distribute_info.clear();
    for (int i = 0; i < crossroad_number; ++i) {
        //#\x00,\x00,\x07,\n,\t,\x00,\x00,#
        for (int j = 0; j < path_info.split('#').at(i).length(); j = j + 2) {
            dat = path_info.split('#').at(i).at(j);
            if (dat != 0 && dat != 99) {
                path_node_distribute_info.append(QString::number(dat,10));
                path_node_distribute_info.append(',');
            }
        }
        path_node_distribute_info.append('\n');
    }
    for (int i = 0; i < crossroad_number; ++i) {
        qDebug() << "第" << (i+1) << "行的节点为：" << path_node_distribute_info.split('\n').at(i);
    }

    // 从文件记录中获取小车朝向
    QFile file("direction.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (file.isOpen()) {
        qDebug() << "open direction.txt file";
        QString str = file.readLine();
        if (str == "0") {
            turltebot_direction = FORWARD;
            qDebug() << "小车朝向为正向";
        } else {
            turltebot_direction = BACKWARD;
            qDebug() << "小车朝向为反向";
        }
        file.close();
    }

    arrived_flag = true;
    turltebot_go = false;
    update_path_node_flag = true;

    send_path_node_to_pc();

}

void Camera::on_read_path_plan()
{
    QRData_store_index = -1;
    flag_path_plan = true;
    flag_on_timer_turn_tail_over = false;
    flag_turn_tail_start = false;
    flag_first_turn_tail = true;
    on_timer_crossroad_qr_counter = 0;
    check_qr_contains_cross_road_node_blank = 0;
    flag_through_crossroad = true;
    check_qr_contains_cross_road_node_number = 0;
    check_qr_contains_cross_road_node_counter = 0;
    path_plan_pre_flag = true;
    on_timer_through_crossroad_counter = 0;
    arrived_flag = false;
    index = 0;
    QRData_stable.clear();
    QRData_store.clear();
    path_plan_array.clear();
    QFile file("path_plan.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    path_plan_array = file.readLine();
    file.close();
    qDebug() << "path plan: " << path_plan_array;

    qDebug() << "start stop node: " << start_stop_node_array;
    /*
path plan:  "UP,LEFT,UP"
crossraod node info:  "2,5\n3,4\n7,9"
crossraod_node_array:  "2,5"
crossroad number is: 2
crossraod_node_near_array:  "3,4\n7,9\n"
start stop node:  "1\n3"
第 1 行的节点为： "1,2,3,4,5,6,"
第 2 行的节点为： "7,10,9,"
 */
    send_status_to_pc();
}

void Camera::send_status_to_pc()
{
    QByteArray info_1;

    // 信息类型
    info_1.append(0x01);
    // 小车朝向
    QFile file("direction.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream.seek(file.size());
    if (turltebot_direction == false) {
        // forward
        info_1.append(0x01);
        stream << "0";
    } else {
        info_1.append(0x02);
        stream << "1";
    }
    file.flush();
    file.close();
    // 起始节点
    if (!start_stop_node_array.isEmpty()) {
        QString str;
        str = start_stop_node_array.split('\n').at(0);
        info_1.append(str.toInt());
        // 终点节点
        str = start_stop_node_array.split('\n').at(1);
        info_1.append(str.toInt());
    } else {
        info_1.append(0xFF);
        info_1.append(0xFF);
    }
    // 当前节点
    if (QRData_stable.isEmpty()) {
        info_1.append(0xFF);
    } else {
        info_1.append(QRData_stable.toInt());
    }

    qDebug() << info_1;

    emit send_info_to_pc(info_1);
}

void Camera::send_ctrl_to_pc(quint8 command)
{
    QByteArray info_3;

    // 信息类型
    info_3.append(0x03);
    info_3.append(command);

    emit send_info_to_pc(info_3);
}

void Camera::send_path_node_to_pc()
{
    qDebug() << "path_node_distribute_info" << path_node_distribute_info;
    qint16 path_node_1st_line_number = path_node_distribute_info.split('\n').at(0).split(',').length() - 1;
    qint16 path_node_2nd_line_number = path_node_distribute_info.split('\n').at(1).split(',').length() - 1;
    qint16 path_node_number = path_node_1st_line_number + path_node_2nd_line_number;
    qDebug() << "path_node_number" << path_node_number;
    qDebug() << "crossroad_number" << crossroad_number;
    qDebug() << "path_node_distribute_info.split('\n').at(i).split(',').length()" << path_node_distribute_info.split('\n').at(0).split(',').length() - 1;
    qint16 path_node[path_node_number + 2];
    for (int i = 0; i < crossroad_number; ++i) {
            for (int j = 0; j < path_node_distribute_info.split('\n').at(i).split(',').length() - 1; ++j) {
                path_node[j + i * path_node_1st_line_number] = path_node_distribute_info.split('\n').at(i).split(',').at(j).toInt();
                qDebug() << "***" << path_node_distribute_info.split('\n').at(i).split(',').at(j).toInt();
        }
    }
    for (int i = 0; i < path_node_number; ++i) {
        qDebug() << "path_node" << path_node[i];
    }
    qint16 tmp;
    for (int i = 0; i < path_node_number - 1; ++i) {
        if (path_node[i+1] < path_node[i]) {
            tmp = path_node[i];
            path_node[i] = path_node[i+1];
            path_node[i+1] = tmp;
        }
    }


    QByteArray path_node_array;
    for (int i = 0; i < path_node_number; ++i) {
        path_node_array.append(QString(path_node[i]));
    }
    qDebug() << "path_node_array" << path_node_array;

    QByteArray info_4;
    info_4.append(0x04);
    info_4.append("#");
    info_4.append(path_node_array);
    info_4.append("#");
    qDebug() << info_4;
    emit send_info_to_pc(info_4);
    if (update_path_node_flag == true) {
        emit update_path_node(info_4);
        update_path_node_flag = false;
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
