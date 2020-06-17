#include "camera.h"

Camera::Camera()
{

    last_path_status = 0xFF;
    flag_qr_detect_crossroad = false;
    flag_through_crossroad = true;
    flag_arrived_crossroad = false;
    flag_capture_grap = true;
    path_status_slow_flag = true;
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
    camera_number=0;
    this->open();
}

void Camera::run()
{

}

void Camera::show()
{
    qDebug() << "show cap";
    VideoCapture cap(camera_number);
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
    capture.open(camera_number);           //open the default camera
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
            qDebug() << "show cap";
            emit show_frame(image);
            timer = new QTimer(this);
            timer_turn = new QTimer(this);
            timer_crossroad = new QTimer(this);
            timer_crossroad_qr = new QTimer(this);
            timer_path_status_slow = new QTimer(this);
            timer_path_status_slow->setInterval(500);
            timer_crossroad_qr->setInterval(200);
            timer_crossroad->setInterval(200);// set timer: 200ms
            timer_turn->setInterval(200);    // set timer: 200ms

            timer->setInterval(1000/rate);   //set timer match with FPS
            connect(timer, SIGNAL(timeout()), this, SLOT(on_next_frame()));
            connect(timer_turn, SIGNAL(timeout()), this, SLOT(on_timer_turn_tail()));
            connect(timer_crossroad, SIGNAL(timeout()), this, SLOT(on_timer_through_crossroad()));
            connect(timer_crossroad_qr, SIGNAL(timeout()), this, SLOT(on_timer_crossroad_qr()));
            connect(timer_path_status_slow, SIGNAL(timeout()), this, SLOT(on_timer_path_status_slow()));
            //            timer->start();
        }
    }
}

void Camera::on_update_path()
{
}

bool Camera::first_detect_qr()
{
    if (first_step == true) {

        QRData_current_update.clear();

        if (flag_first_step_detect_qr == true) {
            flag_first_step_detect_qr = false;
            capture.grab();
            capture.grab();
            capture.grab();
            capture.grab();
        }

        capture >> frame;
        if (!frame.empty())
        {
            QDateTime current_date_time = QDateTime::currentDateTime();
            QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss.zzz");
            // gray
            cvtColor(frame,grayImage,CV_BGR2GRAY);
            //        image = image.mirrored(true, false);
            emit show_frame(Mat2QImage(frame));
            //        emit show_frame_2(Mat2QImage(frame));

            //QR
            zbar::ImageScanner scanner;
            scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
            int width 	= grayImage.cols;
            int height 	= grayImage.rows;

            //            Mat roi(grayImage, Rect(0,height,width,height));
            unsigned char *pdata = (unsigned char *)grayImage.data;
            zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
            int n = scanner.scan(imageZbar);

            if (n > 0){
                ros_speed_line = ROS_SPEED_HIGH;
                path_status_calc = 0;
                // extract results
                for (zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
                     symbol != imageZbar.symbol_end();
                     ++symbol) {
                    // do something useful with results
                    QRData_current_update = QString::fromStdString(symbol->get_data());
                    if (turltebot_go == false) {
                        qDebug() << "QR Code: " << QRData_current_update;
                    }
                }
                //                qDebug() << "QR Code: " << QRData_current_update;
                //                return false;
                if(QRData_stable != QRData_current_update){
                    QRData_stable.clear();
                    QRData_stable.append(QRData_current_update);
                    QRData_store.append(QRData_current_update + ',');
                    QRData_store_index ++;
                    turltebot_direction_judgement();
                    send_status_to_pc();
                    qDebug() << "新扫描的QR内容为：" << QRData_current_update;
                }
            }
        }
    }
    if((QRData_current_update.isEmpty() == false) && (first_step == true))
    {
        //检测到QR
        first_step = false;
        return true;
    } else if((QRData_current_update.isEmpty() == true) && (first_step == true)) {
        //没有检测到QR
        return false;
    } else if(first_step == false){
        // 第一步以后，一律执行此处语法
        return true;
    }
}

void Camera::on_timer_path_status_slow()
{
    //    QDateTime current_date_time2 = QDateTime::currentDateTime();
    //    QString current_date2 = current_date_time2.toString("yyyy-MM-dd hh:mm:ss.zzz");
    //    qDebug() <<  "in timer: " <<current_date2;
    //    qDebug() << "path_status_slow_flag: " << (path_status_slow_flag ? "true" : "false");
    path_status_slow_flag = true;
    timer_path_status_slow->stop();
}

void Camera::path_status_slow()
{
    if (path_status == PATH_STATUS_SLOW && path_status_slow_flag == true) {
        path_status_slow_flag = false;
        path_status_calc ++;
        //        QDateTime current_date_time2 = QDateTime::currentDateTime();
        //        QString current_date2 = current_date_time2.toString("yyyy-MM-dd hh:mm:ss.zzz");
        //        qDebug() << "timer: " << current_date2;
        timer_path_status_slow->start(400);
    }
    qDebug() << "path_status_calc: " << path_status_calc;
    if (path_status_calc < 2) {
        qDebug() << "高速+++++++++++++++++++++++++++++++++++++++++++++++++";
        ros_speed_line = ROS_SPEED_HIGH;
        return;
    } else {
        // 减速
        qDebug() << "低速-------------------------------------------------";
        ros_speed_line = ROS_SPEED_LOW;
        return;
    }
}

void Camera::detect_qr(){

    if (flag_qr_detect_crossroad == true) {
        qDebug() << "转弯中，禁止摄像头开启...";
        return;
    }
    if (ros_speed_line == ROS_SPEED_LOW) {
        qDebug() << "准备识别二维码";
        //        if (flag_capture_grap == true) {
        //            flag_capture_grap = false;
        capture.grab();
        capture.grab();
        capture.grab();
        capture.grab();
        //        }

        capture >> frame;
        if (!frame.empty())
        {
            // gray
            cvtColor(frame,grayImage,CV_BGR2GRAY);

            emit show_frame(Mat2QImage(frame));

            //QR
            zbar::ImageScanner scanner;
            scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
            int width 	= grayImage.cols;
            int height 	= grayImage.rows;

            //            Mat roi(grayImage, Rect(0,height,width,height));
            unsigned char *pdata = (unsigned char *)grayImage.data;
            zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
            int n = scanner.scan(imageZbar);

            QDateTime current_date_time2 = QDateTime::currentDateTime();
            QString current_date2 = current_date_time2.toString("yyyy-MM-dd hh:mm:ss.zzz");
            qDebug() << current_date2;

            if (n > 0){
                flag_capture_grap = true;
                path_status_calc = 0;
                ros_speed_line = ROS_SPEED_HIGH;
                // extract results
                for (zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
                     symbol != imageZbar.symbol_end();
                     ++symbol) {
                    // do something useful with results
                    QRData_current_update = QString::fromStdString(symbol->get_data());
                    qDebug() << "QR Code: " << QRData_current_update;
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
                        qDebug() << "当前道路命令索引：" << index;
                        if (index < path_plan_array.split(',').length() && \
                                QRData_current_update != (start_stop_node_array.split('\n').at(1))) {
                            qDebug() << "准备更新当前路线指令...";
                            qDebug() << "当前路线指令为---------:" << path_plan_array.split(',').at(index);
                            qDebug() << "完成更新当前路线指令...OK";
                        }
                    }
                    qDebug() << "-----";
                    if (crossroad_number > 1) {
                        //有岔道口的时候才会执行下面
                        if ((QRData_stable == (crossraod_node_array.split(',').at(0)) || \
                             QRData_stable == crossraod_node_array.split(',').at(1)))
                        {
                            qDebug() << "重置标志位: flag_complete_crossroad";
                            qDebug() << "重置标志位: flag_arrived_crossroad";
                            flag_through_crossroad = false;
                            flag_arrived_crossroad = false;
                            flag_qr_detect_crossroad = true;
                        }
                    }
                }
            }
            qDebug() << "arrived_flag: " << (arrived_flag?"true":"false");
            if(QRData_current_update == (start_stop_node_array.split('\n').at(1)) && arrived_flag == false){
                qDebug() << "判断是否到达停止点";
                turltebot_go = false;
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
                start_stop_node_array.clear();
                start_stop_node_array.append(QRData_stable);
                start_stop_node_array.append('\n');
                start_stop_node_array.append(QRData_stable);
                send_status_to_pc();
                emit update_path_start_node(start_stop_node_array);
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
                    if (crossroad_number > 1) {

                        //如果小车终点的QR值为岔道口节点，那么记录下来小车经过的上一个节点
                        if (QRData_stable == crossraod_node_array.split(',').at(0) || \
                                QRData_stable == crossraod_node_array.split(',').at(1) ) {
                            QRData_near_cross = QRData_store.split(',').at(index - 1);
                            qDebug() << "小乌龟停到了岔道口处，上一个节点为：" << QRData_near_cross;
                        }
                    }
                }
                return;
                // 此处代码在重新检测二维码目的在于，让小车更加靠近二维码而停下来
                //Rect(x,y,w,h)
                // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
                int width 	= grayImage.cols;
                int height 	= grayImage.rows/2;
                Mat roi(grayImage, Rect(0,height,width,height));
                unsigned char *pdata2 = (unsigned char *)roi.data;
                zbar::Image imageZbar2(width, height, "Y800", pdata2, width * (height));
                int qr_data = scanner.scan(imageZbar2);
                if (qr_data > 0) {
                    turltebot_go = false;
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
                    start_stop_node_array.clear();
                    start_stop_node_array.append(QRData_stable);
                    start_stop_node_array.append('\n');
                    start_stop_node_array.append(QRData_stable);
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
        }
    }
}

///auto get next frame
void Camera::on_next_frame()
{

    if (turltebot_go == false) {
        //在没有启动寻迹时，禁止执行下述程序
        return;
    }
    if (first_detect_qr() == false)//只执行一次，目的在于获取地起点处的二维码
    {
        return;
    }
    path_status_slow(); // 经过二维码后，第二次检测到减速带，那么就开始减速行驶，遇到二维码后（若不是终点）则过切换为高速行驶
    detect_qr();
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd hh:mm:ss.zzz");
    qDebug() << current_date;
    qDebug() << "QRData_current_update: " << QRData_current_update;
    qDebug() << "QRData_stable        : " << QRData_stable;

    if (index < path_plan_array.split(',').length()) {
        qDebug() << "当前路线命令索引号:" <<index;
        qDebug() << "当前路线命令为---:" << path_plan_array.split(',').at(index);
    } else {
        qDebug() << "WARNING: index is out of the lengh of path_plan_array";
        return;
    }

    if (turltebot_direction == FORWARD) {
        qDebug() << "小车朝向为正向";
    } else {
        qDebug() << "小车朝向为反向";
    }
    if (ros_speed_line == ROS_SPEED_HIGH) {
        qDebug() << "[高速]";
    } else {
        qDebug() << "[低速]";
    }

    if (path_plan_pre_flag == true) {
        qDebug() << "检测是否需要调整路线";
        path_plan_pre();
        emit show_command(path_plan_array);
        path_plan_pre_flag = false;
    }

    path_plan();
}

void Camera::path_plan()
{
    if (turn_tail(index) == false) {
        qDebug() << "正在掉头...";
        return;
    }

    if (flag_qr_detect_crossroad == false) {
        qDebug() << "[直线]";
        if (path_status_calc != 2) {
            qDebug() << "高速";
            if (current_straight_path_ros_speed_line < ROS_SPEED_HIGH) {
                current_straight_path_ros_speed_line += 0.002;
            }
            ros_speed_line = current_straight_path_ros_speed_line;
            if (ros_speed_line < 0.1)
                ros_speed_angular = 8 * ros_speed_line;
            else if(ros_speed_line < 0.2)
                ros_speed_angular = 4 * ros_speed_line;
            else if(ros_speed_line < 0.35)
                ros_speed_angular = 2 * ros_speed_line;
            else
                ros_speed_angular = 1.5 * ros_speed_line;
        } else {
            qDebug() << "低速";
            if (ros_speed_line > ROS_SPEED_LOW) {
                ros_speed_line -= 0.01;
            }
            if (ros_speed_line < 0.1)
                ros_speed_angular = 5 * ros_speed_line;
            else if(ros_speed_line < 0.2)
                ros_speed_angular = 4 * ros_speed_line;
            else
                ros_speed_angular = 2 * ros_speed_line;
        }
        QDateTime current_date_time2 = QDateTime::currentDateTime();
        QString current_date2 = current_date_time2.toString("yyyy-MM-dd hh:mm:ss.zzz");
        qDebug() << current_date2;
        straightroad_plan();
    } else {
        current_straight_path_ros_speed_line = ROS_SPEED_LOW;
        qDebug() << "[岔道]";
        crossroad_plan();
    }
}

void Camera::straightroad_plan()
{
    //    ros_speed_angular = ros_speed_line * 2;

    if (last_path_status == PATH_STATUS_LEFT && (path_status != PATH_STATUS_UP)) {
        //上一次状态为左转，并且当前状态不为直行，那么继续左转，直到当前状态为直行为止
        if(path_status == 0x0E){
            //            ros_speed_line = ROS_SPEED_HIGH / 2;
            ros_speed_angular = 3 * ros_speed_line;
            qDebug() << "加速转弯";
        } else if (path_status == 0x0F) {
            qDebug() << "超级加速转弯";
            ros_speed_angular = 6 * ros_speed_line;
        }
        if (path_status == 0x07) {
            if (right == 1) {
                emit show_tutlebot_status(TURLTEBOT_RIGHT);
                lock_status(TURLTEBOT_RIGHT);
            }
            last_path_status = PATH_STATUS_RIGHT;
            qDebug() << "小车左转过头了，开始右转";
            qDebug() << "[右转] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";
            emit turltebot_right(ros_speed_line, ros_speed_angular);
            return;
        }

        if (left == 1) {
            emit show_tutlebot_status(TURLTEBOT_LEFT);
            lock_status(TURLTEBOT_LEFT);
        }
        qDebug() << "继续发指令让小车左转";
        qDebug() << "[左转] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";
        emit turltebot_left(ros_speed_line, ros_speed_angular);
        return;
    } else if (last_path_status == PATH_STATUS_RIGHT && (path_status != PATH_STATUS_UP)) {
        //        ros_speed_angular = 1.5 * ros_speed_angular;
        if(path_status == 0x07){
            //            ros_speed_line = ROS_SPEED_HIGH / 2;
            ros_speed_angular = 3 * ros_speed_line;
            qDebug() << "加速转弯";
        } else if (path_status == 0x0F) {
            qDebug() << "超级加速转弯";
            ros_speed_angular = 6 * ros_speed_line;
        }
        if (path_status == 0x0E) {
            //左转
            if (left == 1) {
                emit show_tutlebot_status(TURLTEBOT_LEFT);
                lock_status(TURLTEBOT_LEFT);
            }
            last_path_status = PATH_STATUS_LEFT;
            qDebug() << "小车右转过头了，开始左转";
            qDebug() << "[左转] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";
            emit turltebot_left(ros_speed_line, ros_speed_angular);
            return;
        }
        //上一次状态为右转，并且当前状态不为直行，那么继续右转，直到当前状态为直行为止
        if (right == 1) {
            emit show_tutlebot_status(TURLTEBOT_RIGHT);
            lock_status(TURLTEBOT_RIGHT);
        }
        qDebug() << "继续发指令让小车右转";
        qDebug() << "[右转] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";
        emit turltebot_right(ros_speed_line, ros_speed_angular);
        return;
    }

    if (path_status == PATH_STATUS_LEFT || path_status == 0x0E || path_status == 0x0D) {
        // 下面的二进制高位在右边
        //0x0E(0111) 最左边的LED亮，向左转
        //0x0D(1011) 次左边LED亮，向左转
        last_path_status = PATH_STATUS_LEFT;
        //小车向轨道右边偏离，发指令让小车左转
        qDebug() << "小车向轨道右边偏离，发指令让小车左转";
        if (left == 1) {
            emit show_tutlebot_status(TURLTEBOT_LEFT);
            lock_status(TURLTEBOT_LEFT);
        }

        qDebug() << "[左转] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";
        emit turltebot_left(ros_speed_line, ros_speed_angular);
    }
    else if(path_status == PATH_STATUS_RIGHT || path_status == 0x07 || path_status == 0x0B){
        // 下面的二进制高位在右边
        //0x07(1110) 最右边的LED亮，向右转
        //0x0B(1101) 次右边LED亮，向右转
        last_path_status = PATH_STATUS_RIGHT;
        //小车向轨道边左偏离，发指令让小车右转
        qDebug() << "小车向轨道边左偏离，发指令让小车右转";
        if (right == 1) {
            emit show_tutlebot_status(TURLTEBOT_RIGHT);
            lock_status(TURLTEBOT_RIGHT);
        }
        qDebug() << "[右转] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";
        emit turltebot_right(ros_speed_line, ros_speed_angular);

    }
    else if(path_status == PATH_STATUS_UP) {
        last_path_status = PATH_STATUS_UP;
        qDebug() << "路线无偏移，沿当前路线直行";
        if (up == 1) {
            emit show_tutlebot_status(TURLTEBOT_UP);
            lock_status(TURLTEBOT_UP);
        }
        qDebug() << "[直行] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";
        emit turltebot_up(ros_speed_line, ros_speed_angular);
    }
    else if(path_status == PATH_STATUS_SLOW) {
        emit turltebot_up(ros_speed_line, ros_speed_angular);
        if (up == 1) {
            emit show_tutlebot_status(TURLTEBOT_UP);
            lock_status(TURLTEBOT_UP);
        }
        last_path_status = 0xFF;
        qDebug() << "减缓移动";
        return;
        //        if (stop == 1) {
        //            emit show_tutlebot_status(TURLTEBOT_STOP);
        //            lock_status(TURLTEBOT_STOP);
        //            ros_speed_line = ROS_SPEED_LOW;
        //        }
    }

    if (path_status == PATH_STATUS_BLANK || flag_no_detect_route_keep_up == true) {
        flag_no_detect_route_keep_up = true;
        emit turltebot_up(ros_speed_line, ros_speed_angular);
        if (up == 1) {
            emit show_tutlebot_status(TURLTEBOT_UP);
            lock_status(TURLTEBOT_UP);
        }
        qDebug() << "灰度传感器没有检测到轨道，继续直行移动";
        if (path_status == 0x07 || path_status == 0x0B) {
            qDebug() << "向右转";
            // 下面的二进制高位在右边
            //0x07(1110) 最右边的LED亮，向右转
            //0x0B(1101) 次右边LED亮，向右转
            last_path_status = PATH_STATUS_RIGHT;
            flag_no_detect_route_keep_up = false;
        } else if(path_status == 0x0E || path_status == 0x0D) {
            qDebug() << "向左转";
            // 下面的二进制高位在右边
            //0x0E(0111) 最左边的LED亮，向左转
            //0x0D(1011) 次左边LED亮，向左转
            last_path_status = PATH_STATUS_LEFT;
            flag_no_detect_route_keep_up = false;
        }
    }
}

void Camera::crossroad_plan()
{

    if (!(QRData_stable == crossraod_node_array.split(',').at(0) || \
          QRData_stable == crossraod_node_array.split(',').at(1)) && \
            flag_through_crossroad == true)
    {
        return;
    }
    // 小车来到了岔路口附近，减速准备转向
    // the turtlebot move status maybe change

    qDebug() << "[此处为岔道口]";
    ros_speed_line = ROS_SPEED_LOW;
    ros_speed_angular = ros_speed_line * 4;
    //    qDebug() << "[岔道] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";

    if ((path_status & PATH_STATUS_LED_LEFT) == 0x00 || \
            (path_status & PATH_STATUS_LED_RIGHT) == 0x00   ) {
        //抵达岔道口
        qDebug() << "[抵达岔道口]";
        ros_speed_angular = ros_speed_angular * 5;
        flag_arrived_crossroad = true;
    }

    if (flag_arrived_crossroad == false) {
        ros_speed_angular = ros_speed_angular * 2;
        qDebug() << "[岔道口]：直行";
        straightroad_plan();
    } else {
        QString current_crossroad_com = path_plan_array.split(',').at(index);

        if (path_status == PATH_STATUS_UP || flag_truned_crossroad_then_up == true) {
            qDebug() << "开始纠正姿态";
            flag_truned_crossroad_then_up = true;
            straightroad_plan();
            if (path_status_calc == 1) {
                qDebug() << "转弯完成，开始高速行驶";
                last_path_status = 0xFF;
                this->index ++;//执行下一个命令
                ros_speed_line = ROS_SPEED_HIGH;
                flag_truned_crossroad_then_up = false;
                flag_arrived_crossroad = false; //检测红外探头是否抵达岔道口
                flag_through_crossroad = true; //检测红外探头是否转弯成功
                flag_qr_detect_crossroad = false;//检测是否需要进入岔道口程序中
            }
            //遇到岔道口，先执行下面的转向任务，然后在执行上面的判定姿态是否归位
        } else if (current_crossroad_com == "LEFT" && flag_truned_crossroad_then_up == false) {
            //            last_path_status = PATH_STATUS_LEFT;
            qDebug() << "[岔道口] 左转";
            if (left == 1) {
                emit show_tutlebot_status(TURLTEBOT_LEFT);
                lock_status(TURLTEBOT_LEFT);
            }
            qDebug() << "[左转] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";
            emit turltebot_left(ros_speed_line, ros_speed_angular);
        } else if(current_crossroad_com == "RIGHT" && flag_truned_crossroad_then_up == false) {
            //            last_path_status = PATH_STATUS_LEFT;
            qDebug() << "[岔道口] 右转";
            if (right == 1) {
                emit show_tutlebot_status(TURLTEBOT_RIGHT);
                lock_status(TURLTEBOT_RIGHT);
            }
            qDebug() << "[右转] 线速度为：" << ros_speed_line << "m/s " << "角速度为：" << ros_speed_angular << "rad/s";
            emit turltebot_right(ros_speed_line, ros_speed_angular);
        }
    }
}

void Camera::path_plan_pre() {

    // 如果只有一条道路
    if (crossroad_number == 1) {
        qDebug() << "只有一条路线";
        if (turltebot_direction == FORWARD) {
            qDebug() << "无需调整";
            return;
        }
        else if(turltebot_direction == BACKWARD){
            if (path_plan_array.split(',').at(0) == "TURN") {
                this->index ++;
                return;
            }
            else if (path_plan_array.split(',').at(0) != "TURN") {
                QByteArray path_plan_array_tmp;
                path_plan_array_tmp.append("TURN,");
                qint8 len = path_plan_array.split(',').length();
                for (int i = 0; i < len - 1; ++i) {
                    path_plan_array_tmp.append(path_plan_array.split(',').at(i) + ',');
                }
                path_plan_array_tmp.append(path_plan_array.split(',').at(len - 1));
                path_plan_array = path_plan_array_tmp;
                qDebug() << "path_plan_array: " << path_plan_array;
                return;
            }
        }
    }

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

bool Camera::turn_tail(qint16 index)
{
    if (index != 0) {
        return true;
    }
    //如果当前朝向为正向，路线命令第一个为TURN，那么执行掉头
    //如果当前朝向为反向，路线命令第一个不为TURN，那么执行掉头
    if (path_plan_array.split(',').at(index) == "TURN") {
        // 开始掉头
        // 掉头第一阶段，让小车红外探头移出轨道
        if (path_status != PATH_STATUS_BLANK && flag_path_status_blank == false) {
            emit turltebot_turn(0.0, 3.14159 / 5.0);
            if (turn == 1) {
                emit show_tutlebot_status(TURLTEBOT_TURN);
                lock_status(TURLTEBOT_TURN);
            }
            return false;
        }
        // 掉头第二阶段，小车红外探头移出轨道后，若小车 path_status & 0x09 不等于 0x09, 那么让小车保持掉头命令
        else if(path_status != PATH_STATUS_UP){
            emit turltebot_turn(0.0, 3.14159 / 10.0);
            flag_path_status_blank = true;
            return false;
        }
        // 掉头第三阶段，若检测小车红外探头再次移动到轨道上，表明掉头结束
        else if(path_status == PATH_STATUS_UP ){
            flag_path_status_blank = false;
            qDebug() << "掉头成功";
            path_status_calc = 1;
            this->index ++;
            return true;
        }
    } else {
        return true;
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
        qint16 path_node_1st_line_max_index = path_node_distribute_info.split('\n').at(0).split(',').length() - 2;
        //        qint16 path_node_2nd_line_max_index = path_node_distribute_info.split('\n').at(1).split(',').length() - 2;
        qDebug() << "path_node_1st_line_max_index" << path_node_1st_line_max_index;
        //1.如果检测到相邻QR值所在坐标都在第一行
        if (path_node_distribute_info.split('\n').at(0).split(',').contains(QRData_current) == true &&\
                path_node_distribute_info.split('\n').at(0).split(',').contains(QRData_last) == true) {
            qDebug() << "检测到相邻QR值所在坐标都在第一行";
            if (path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_current) == 0 &&\
                    path_node_distribute_info.split('\n').at(0).split(',').indexOf(QRData_last) == path_node_1st_line_max_index) {

                qDebug() << "当前QR坐标是第一行的第一个，上一次QR坐标为第一行的最后一个，判定小车一定是正向";
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
        QByteArray dir = file.readLine();
        qDebug() << dir;
        //        QString str = file.readLine();
        if (dir.contains("0")) {
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
    current_straight_path_ros_speed_line = ROS_SPEED_LOW;
    flag_first_step_detect_qr = true;
    //last_path_status = 0xFF;
    path_status_calc = 0;
    flag_path_status_blank = false;
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

    QFile file_direction("direction.txt");
    file_direction.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray dir = file_direction.readLine();
    if (dir.contains("0")){
        turltebot_direction = FORWARD;
        qDebug() << "初始方向为正向";
    } else {
        turltebot_direction = BACKWARD;
        qDebug() << "初始方向为反向";
    }

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
    timer->start();
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
    if (turltebot_direction == FORWARD) {
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
            //            qDebug() << "***" << path_node_distribute_info.split('\n').at(i).split(',').at(j).toInt();
        }
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
