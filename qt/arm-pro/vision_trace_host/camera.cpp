#include "camera.h"

Camera::Camera()
{

    camera_number=0;
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
    timer = new QTimer(this);
    timer_turn = new QTimer(this);
    timer_crossroad = new QTimer(this);
    timer_crossroad_qr = new QTimer(this);
    timer_crossroad_qr->setInterval(200);
    timer_crossroad->setInterval(200);// set timer: 200ms
    timer_turn->setInterval(100);    // set timer: 100ms
    timer->setInterval(1000/30);   //set timer match with FPS
    connect(timer, SIGNAL(timeout()), this, SLOT(on_next_frame()));
    connect(timer_turn, SIGNAL(timeout()), this, SLOT(on_timer_turn_tail()));
    connect(timer_crossroad, SIGNAL(timeout()), this, SLOT(on_timer_through_crossroad()));
    connect(timer_crossroad_qr, SIGNAL(timeout()), this, SLOT(on_timer_crossroad_qr()));

    //img = imread("template.png", IMREAD_GRAYSCALE);

    //    path = new Pathplan();
}

void Camera::run()
{

}

QString Camera::get_current_location()
{

//    qDebug() << "start cap";
//    if (capture.isOpened()){
//        capture.release();     //decide if capture is already opened; if so,close it

//    }
//    capture.open(camera_number);           //open the default camera
//    qDebug() << "open cap";

    QString current_location = "0";
    quint8 counter = 0;
    capture.grab();
    capture.grab();
    capture.grab();
    capture.grab();
    capture.grab();
    capture.grab();
    capture.grab();
    capture >> frame;
    while (!frame.empty())
    {
        emit show_frame_2(Mat2QImage(frame));
        counter++;
        // gray
        cvtColor(frame,grayImage,CV_BGR2GRAY);
        //QR
        zbar::ImageScanner scanner;
        scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
        pix_offset_qr = 50;
        int width 	= grayImage.cols;
        int height 	= grayImage.rows;

        //Rect(x,y,w,h)
        // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
        Mat roi(grayImage, Rect(0,grayImage.rows - height,width,height));

        unsigned char *pdata = (unsigned char *)roi.data;
        zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
        int n = scanner.scan(imageZbar);
        if (n > 0){
            // extract results
            for (zbar::Image::SymbolIterator symbol = imageZbar.symbol_begin();
                 symbol != imageZbar.symbol_end();
                 ++symbol) {
                // do something useful with results
                current_location = QString::fromStdString(symbol->get_data());
            }
            return current_location;
        }
        if(counter >= 20){
            current_location = "0";
            return current_location;
        }
    }
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
    qDebug() << "start cap...";
    if (capture.isOpened()){
        capture.release();     //decide if capture is already opened; if so,close it
    }
    capture.open(this->camera_number);           //open the default camera
    qDebug() << "open cap";
    if (capture.isOpened())
    {
        qDebug() << "read cap";
        VideoWriter vwr;

        //int fourcc = vw.fourcc('M','J','P','G');
        qDebug() << "open cap success";
        //capture.set(CAP_PROP_FOURCC,fourcc);
        capture.set(CV_CAP_PROP_FPS, 30);
        rate= capture.get(CV_CAP_PROP_FPS);
        capture >> frame;
        if (!frame.empty())
        {
            qDebug() << "get cap";
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
        qint16 erode_threshold=60;//60
        qint16 dilate_threshold =30 /*- dilate_threshold_qr*/;//30
        Mat element_dilateImage_qr = getStructuringElement(MORPH_RECT, Size(dilate_threshold_qr, dilate_threshold_qr));
        Mat element_dilateImage = getStructuringElement(MORPH_RECT, Size(dilate_threshold, dilate_threshold));
        Mat element_erodeImage = getStructuringElement(MORPH_RECT, Size(erode_threshold, erode_threshold));
        // gray
        cvtColor(frame,grayImage,CV_BGR2GRAY);
        threshold(grayImage,binaryImage,threshold_img,255,THRESH_BINARY);
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
        //image = Mat2QImage(frame);
        //        image = image.mirrored(true, false);
        emit show_frame(image);
        emit show_frame_2(Mat2QImage(frame));

        if(turltebot_go == false)
        {
            return;
        }

        //QR
        zbar::ImageScanner scanner;
        scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
        pix_offset_qr = 50;
        int width 	= grayImage.cols;
        int height 	= grayImage.rows / 3;

        //Rect(x,y,w,h)
        // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
        Mat roi(grayImage, Rect(0,grayImage.rows - height,width,height));

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
                qDebug() << "QR Code: " << QRData_current_update;
                if(!path_node_distribute_info.contains( QRData_current_update.toUtf8() )){
                    qDebug() << "rescan picture to get QR code!";
                    return;
                }
            }
            if(QRData_stable != QRData_current_update){
                QRData_stable.clear();
                QRData_stable.append(QRData_current_update);
                QRData_store.append(QRData_current_update + ',');
                QRData_store_index ++;
                qDebug() << "新扫描的QR内容为：" << QRData_current_update;

                //turltebot_direction_judgement();
                counter_check_next_stop_node = 0;
                flag_check_stay_close_qr = false;
                // update path command

                if(QRData_current_update != start_stop_node_array.split('\n').at(0) && \
                        QRData_current_update != start_stop_node_array.split('\n').at(1) && \
                        flag_path_plan == true){
                    index ++;
                    if (index != path_plan_array.split(',').length() && \
                            QRData_current_update != (start_stop_node_array.split('\n').at(1))) {
                        qDebug() << "准备更新当前路线指令...";
                        qDebug() << "当前路线指令为---------:" << path_plan_array.split(',').at(index);
                        qDebug() << "完成更新当前路线指令...OK";
                    }
                }
                //                qDebug() << "path_plan_array.split(',').at(index)" << path_plan_array.split(',').at(index);
            }
        }

        if(flag_check_go_through_next_node == true){
            if(QRData_current_update.isEmpty()){
                counter_check_next_stop_node ++;
                if(counter_check_next_stop_node > 100){
                    flag_check_go_through_next_node2 = true;
                    qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
                    counter_check_next_stop_node = 0;
                }
            }
        }

        if(QRData_current_update == (start_stop_node_array.split('\n').at(1)) && arrived_flag == false){
            flag_check_go_through_next_node = true;
            qDebug() << "----------*******************%%%%%%%%%%%%*************------------------";
            qDebug() << QRData_current_update;
            //            counter_check_next_stop_node ++;

            if(flag_check_go_through_next_node2 == true) //设置大于等于10的目的在于，越过第一个目标节点，在这里实现下车到第二个stop节点停下
                //            if(counter_check_next_stop_node > 20)
            {
                qDebug() << "********************************************";
                counter_check_next_stop_node = 0;
                // 此处代码在重新检测二维码目的在于，让小车更加靠近二维码而停下来
                //Rect(x,y,w,h)
                // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
                int width 	= grayImage.cols;
                int height 	= grayImage.rows / 2;
                qDebug() << "width: " << width;
                qDebug() << "height: " << height;

                Mat roi(grayImage, Rect(0,grayImage.rows - height,width,height));
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
                    //                    if (check_direction() == true){
                    //                        return;
                    //                    }
                    qDebug() << "*****************************************";
                    qDebug() << "  Arrived terminate, stop the turltebot";
                    qDebug() << "*****************************************";
                    flag_check_direction = false;
                    flag_check_stay_close_qr = true;
                    start_stop_node_array.clear();
                    start_stop_node_array.append(QRData_stable);
                    start_stop_node_array.append('\n');
                    start_stop_node_array.append(QRData_stable);
                    send_status_to_pc();
                    emit update_path_start_node(start_stop_node_array);
                    //timer_crossroad->stop();
                    timer_crossroad_start_flag = true;
                    //qDebug() << "on_timer_through_crossroad_counter is over:" << on_timer_through_crossroad_counter*200 << "ms" ;
                    on_timer_through_crossroad_counter = 0;
                    check_qr_contains_cross_road_node_counter = 0;
                    check_qr_contains_cross_road_node_number = 0;


                    if (stop == 1) {
                        emit show_tutlebot_status(TURLTEBOT_STOP);
                        lock_status(TURLTEBOT_STOP);
                        emit turltebot_stop();
                        //如果小车终点的QR值为岔道口节点，那么记录下来小车经过的上一个节点
                        //                    if (QRData_stable == crossraod_node_array.split(',').at(0) || \
                        //                            QRData_stable == crossraod_node_array.split(',').at(1) ) {
                        //                        QRData_near_cross = QRData_store.split(',').at(index - 1);
                        //                        qDebug() << "小乌龟停到了岔道口处，上一个节点为：" << QRData_near_cross;
                        //                    }
                    }
                    return;
                }
            }
        }

        if (arrived_flag == true && check_direction() == true){
            qDebug() << "TRUN TAIL...";
            return;
        }

        // check if the robot stay in QR line
        if (arrived_flag == true && flag_check_stay_close_qr == true){
            qDebug() << "Stay QR more close...";

            int width 	= grayImage.cols;
            int height 	= grayImage.rows * 2 / 3;

            //Rect(x,y,w,h)
            // x,y is coordinate, w,h calc banse x and y, final pic is (x,x+w),(y,y+h)
            Mat roi(grayImage, Rect(0, grayImage.rows - height,width,height));
            unsigned char *pdata = (unsigned char *)roi.data;
            zbar::Image imageZbar(width, height, "Y800", pdata, width * height);
            int n = scanner.scan(imageZbar);
            if( n > 0 ){
                flag_check_stay_close_qr = false;
                qDebug() << "stay QR close enough";
            } else {
                path_plan();
                return;
            }
        }

        if(arrived_flag == true){
            // xiuzheng
            adjust_orentation();
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
            qDebug() << "path_plan_array" << path_plan_array;
            emit show_command(path_plan_array);
            path_plan_pre_flag = false;
        }
        if( turn_tail(index) == true ){
            return;
        }
        path_plan();
    }
}

bool Camera::check_direction()
{
    /*
     * 1. 如果小车朝向正向，但小车在垂直路线上面，那么需要掉头
     * 2. 如果小车朝向反向，但小车不在垂直路线上面，那么需要掉头
     */
    //    if(flag_check_direction == true)
    //        return;
    //    flag_check_direction = true;

    volatile int data = start_stop_node_array.split('\n').at(1).toInt();
    qDebug() << data;
    qDebug() << this->road_boundary1;
    if(turltebot_direction == FORWARD) {
        qDebug() << "FORWARD";
    } else {
        qDebug() << "BACKWARD";
    }

    if(turltebot_direction == FORWARD)
    {
        qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";

        if( data <=  road_boundary1)
        {
            qDebug() << data;
            qDebug() << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";
            if (flag_turn_tail_start == false) {
                flag_on_timer_turn_tail_over = false;// 掉头完成标志位
                turn_tail_start();
                flag_turn_tail_start = true; // 保证在一次掉头动作中，完成一次开启定时器
            }
            if (flag_on_timer_turn_tail_over == true) {
                flag_turn_tail_start = false; //为下一次定时器开启作准备
                //            turltebot_direction = !turltebot_direction; //改变小车朝向
                qDebug() << "完成掉头";
                return false;//完成掉头
            } else {
                qDebug() << "正在掉头...";
                return true;//没有完成掉头
            }
        }
    } else {
        if(data >= this->road_boundary2)
        {
            qDebug() << "BACKWARD";
            if (flag_turn_tail_start == false) {
                flag_on_timer_turn_tail_over = false;// 掉头完成标志位
                turn_tail_start();
                flag_turn_tail_start = true; // 保证在一次掉头动作中，完成一次开启定时器
            }
            if (flag_on_timer_turn_tail_over == true) {
                flag_turn_tail_start = false; //为下一次定时器开启作准备
                //            turltebot_direction = !turltebot_direction; //改变小车朝向
                qDebug() << "完成掉头";
                return false;//完成掉头
            } else {
                qDebug() << "正在掉头...";
                return true;//没有完成掉头
            }
        }
    }
    return false;
}

void Camera::get_next_frame()
{
    //qDebug() << "get_next_frame";
    capture >> frame;
    if (!frame.empty())
    {
        qint16 dilate_threshold_qr=30;
        qint16 erode_threshold=60;//60
        qint16 dilate_threshold =30 /*- dilate_threshold_qr*/;//30
        Mat element_dilateImage_qr = getStructuringElement(MORPH_RECT, Size(dilate_threshold_qr, dilate_threshold_qr));
        Mat element_dilateImage = getStructuringElement(MORPH_RECT, Size(dilate_threshold, dilate_threshold));
        Mat element_erodeImage = getStructuringElement(MORPH_RECT, Size(erode_threshold, erode_threshold));
        // gray
        cvtColor(frame,grayImage,CV_BGR2GRAY);
        threshold(grayImage,binaryImage,threshold_img,255,THRESH_BINARY);
        // dilate
        dilate(binaryImage, dilateImage, element_dilateImage_qr);
        // erode
        erode(dilateImage, erodeImage,element_erodeImage);
        //dilate
        dilate(erodeImage, dilateImage, element_dilateImage);
        qDebug() << "get_next_frame success";
    }
}

void Camera::adjust_orentation()
{
    qint16 x1=0,x2=0,y1=0,y2=0;
    qint16 x_offset=0,y_offset=0;
    qint16 cols = dilateImage.cols;
    // get 150th line pix data
    char *data_1 = dilateImage.ptr<char>(50);
    QByteArray color_line_100 = QByteArray(data_1,cols);
    // get 500th line pix data
    char *data_2 = dilateImage.ptr<char>(400);
    QByteArray color_line_500 = QByteArray(data_2,cols);

    len_frame = cols;
    black_count_100 = 0;
    memset(black_index_100,0,len_frame);
    count_100 = 0;
    black_count_500 = 0;
    memset(black_index_500,0,len_frame);
    count_500 = 0;

    for (int i = 1; i < len_frame - 1; ++i) {
        if (color_line_100[i] == 0) {
            black_count_100 ++;
            black_index_100[count_100 ++] = i;
        }
        if (color_line_500[i] == 0) {
            black_count_500 ++;
            black_index_500[count_500 ++] = i;
        }
    }

    //获取线速度和角速度
    this->speed_line = this->doubleSpinBox_line_speed;
    this->speed_angular = this->doubleSpinBox_angular_speed;

    /**
     * 修正方向共有两种情况：
     *
     * 1. 顺时针旋转机器人，使其姿态与轨道平行
     *
     * 2. 逆时针旋转机器人，使其姿态与轨道平行
     *
     */

    x1 = black_index_100[0];
    y1 = black_index_100[black_count_100 - 1];
    x2 = black_index_500[0];
    y2 = black_index_500[black_count_500 - 1];
    x_offset = x1 - x2;
    y_offset = y1 - y2;

    //如果机器人姿态已经和轨道在误差允许范围内视为平行，那么修正姿态完成
    if(qAbs(x_offset) <= 10 && qAbs(y_offset) <= 10)
    {
        qDebug() << "x1 - y1 : " << x1 << " " << y1 << " " << x_offset;
        qDebug() << "x2 - y2 : " << x2 << " " << y2 << " " << y_offset;
        arrived_terminate_counter ++;
        if(arrived_terminate_counter > 100){
            qDebug() << "机器人姿态调整完成";
            turltebot_go = false;
            arrived_terminate();
        }
        return;
    }

    qDebug() << "第100行黑色像素共有:" << black_count_100;
    qDebug() << "x1 - y1 : " << x1 << " " << y1 << " " << x_offset;
    qDebug() << "x2 - y2 : " << x2 << " " << y2 << " " << y_offset;

    arrived_terminate_counter = 0;
    if(x_offset < 0)
    {
        qDebug() << "逆时针调整机器人姿态";
        emit turltebot_turn_counterclockwise(0.0, 3.14159 / 30.0);
    } else {
        qDebug() << "顺时针调整机器人姿态";
        emit turltebot_turn_clockwise(0.0, 3.14159 / 30.0);
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

    //获取线速度和角速度
    this->speed_line = this->doubleSpinBox_line_speed;
    this->speed_angular = this->doubleSpinBox_angular_speed;

    //    if (qAbs( qAbs(black_index_100[0] - black_index_300[0]) - \
    //              qAbs(black_index_300[0] - black_index_500[0]) < 5)
    //            )
    //    {
    qDebug() << "此处为直线";

    // 判断是否需要加速修正轨道
    if(black_center < len_frame / 2 - 10 || black_center > len_frame / 2 + 10){
        this->speed_angular = this->doubleSpinBox_angular_speed;
        if(black_center < len_frame / 2 - 25 || black_center > len_frame / 2 + 25){
            this->speed_angular = 2 * this->doubleSpinBox_angular_speed;
            if(black_center < len_frame / 2 - 40 || black_center > len_frame / 2 + 40){
                this->speed_angular += 2 * this->doubleSpinBox_angular_speed;
                if(black_center < len_frame / 2 - 55 || black_center > len_frame / 2 + 55){
                    this->speed_angular += 2 * this->doubleSpinBox_angular_speed;
                    if(black_center < len_frame / 2 - 70 || black_center > len_frame / 2 + 70){
                        this->speed_angular += 2 * this->doubleSpinBox_angular_speed;
                        if(black_center < len_frame / 2 - 90 || black_center > len_frame / 2 + 90){
                            this->speed_angular += 2 * this->doubleSpinBox_angular_speed;
                        }
                    }
                }
            }
        }
    }

    // 判断是否需要加速调整
    if (black_index_100[0] < 100 || black_index_100[black_count_100 - 1] > len_frame - 100) {
        qDebug() << "加速修正偏移";
        this->speed_angular += this->speed_angular * 2.5;
    }
    qDebug() << "第100行黑色像素共有:" << black_count_100;
    qDebug() << "其黑色像素的首尾坐标:" << black_index_100[0] << " " << black_index_100[black_count_100 - 1];

    if (black_count_100 > PATH_STRAIGHT_ROAD_LEN) {

        //find the center of the black pix
        black_center = (black_index_100[0] + black_index_100[black_count_100 - 1]) / 2;
        qDebug() << "黑色像素中心为：" << black_center;

        if ( black_center < (len_frame / 2 - 10)) {
            //小车向轨道右边偏离，发指令让小车左转
            qDebug() << "小车向轨道右边偏离，发指令让小车左转";
            if (left == 1) {
                emit show_tutlebot_status(TURLTEBOT_LEFT);
                lock_status(TURLTEBOT_LEFT);
            }

            qDebug() << "[左转] 线速度为：" << speed_line << "m/s " << "角速度为：" << speed_angular << "rad/s";
            emit turltebot_left(speed_line, speed_angular);
        }
        else if(black_center > (len_frame / 2 + 10)){
            //小车向轨道边左偏离，发指令让小车右转
            qDebug() << "小车向轨道边左偏离，发指令让小车右转";
            if (right == 1) {
                emit show_tutlebot_status(TURLTEBOT_RIGHT);
                lock_status(TURLTEBOT_RIGHT);
            }
            qDebug() << "[右转] 线速度为：" << speed_line << "m/s " << "角速度为：" << speed_angular << "rad/s";
            emit turltebot_right(speed_line, speed_angular);

        }
        else {
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

void Camera::path_plan_pre()
{
    if( turltebot_direction == BACKWARD )
    {
        if(path_plan_array.split(',').at(0) == "TURN")
        {
            this->index ++; // skip the TURN command
            return;
        }
        if(path_plan_array.split(',').at(0) != "TURN")
        {
            QByteArray path_plan_array_tmp;
            path_plan_array_tmp.append("TURN,");
            path_plan_array_tmp.append(path_plan_array);
            path_plan_array.clear();
            path_plan_array.append(path_plan_array_tmp);
            return;
        }
    }
}

void Camera::turn_tail_start(){
    if (turn == 1) {
        emit show_tutlebot_status(TURLTEBOT_TURN);
        lock_status(TURLTEBOT_TURN);
    }
    if (timer_turn_flag == true) {
        qDebug() << "turn_tail_start";
        on_turn_tail_counter = 0;
        timer_turn->start(100);
        timer_turn_flag = false;
    }
}

void Camera::on_timer_turn_tail(){

    qDebug() << "on_timer_turn_tail";
    emit turltebot_turn(0.0, 3.14159 / 6.0);
    on_turn_tail_counter ++;
    // Timer is 100ms, 20 * 100ms = 2s
    if (on_turn_tail_counter >= 20) {

        get_next_frame();
        if (!frame.empty())
        {
            //cvtColor(frame,grayImage,CV_BGR2GRAY);
            qint16 cols = dilateImage.cols;
            // get 100th line pix data
            char *data_1 = dilateImage.ptr<char>(120);
            QByteArray color_line_100 = QByteArray(data_1,cols);

            len_frame = cols;
            black_count_100 = 0;
            memset(black_index_100,0,len_frame);
            count_100 = 0;

            for (int i = 1; i < len_frame - 1; ++i) {
                if (color_line_100[i] == 0) {
                    black_count_100 ++;
                    black_index_100[count_100 ++] = i;
                }
            }

            black_center = (black_index_100[0] + black_index_100[black_count_100 - 1]) / 2;

            //如果黑色带中心位于摄像头视野中间，那么判定转弯完成
            if( (black_center > len_frame / 2 - 220) && (black_center < len_frame / 2 + 200) )
            {
                timer_turn->stop();
                timer_turn_flag = true;
                on_turn_tail_counter = 0;
                qDebug() << "Update path command";
                qDebug() << "The next path command is:" << path_plan_array.split(',').at(index);
                qDebug() << "小乌龟改变了行驶方向";
                flag_on_timer_turn_tail_over = true;
                if(turltebot_direction == FORWARD)
                    turltebot_direction = BACKWARD;
                else
                    turltebot_direction = FORWARD;
                emit update_trultebot_direction(turltebot_direction);
            } else {

            }
        }
    }
}

bool Camera::turn_tail(qint16 index)
{
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
    crossroad_number = 0;
    QRData_stable.clear();
    QRData_store.clear();
    path_plan_array.clear();
    QFile file("path_plan.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)==true)
    {
        qDebug() << "open path_plan.txt success";
        path_plan_array = file.readLine();
        file.close();
    }

    qDebug() << "path plan: " << path_plan_array;

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

    path_node_distribute_info.clear();
    crossroad_number = 1;//只有一条线路
    QFile file_path_node_distribute_info("path_node_distribute_info.txt");
    if (file_path_node_distribute_info.open(QIODevice::ReadOnly | QIODevice::Text) == true)
    {
        qDebug() << "打开" << "path_node_distribute_info.txt" << "成功";
        qDebug() << "crossroad_number" << crossroad_number;
        for (int i = 0; i < crossroad_number; ++i) {
            path_node_distribute_info.append(file_path_node_distribute_info.readLine());
        }
        for (int i = 0; i < crossroad_number; ++i) {
            qDebug() << "第" << (i+1) << "行的节点为：" << path_node_distribute_info.split('\n').at(i);
        }
    } else {
        qDebug() << "打开" << "path_node_distribute_info.txt" << "失败";
    }

    arrived_flag = false;
    index = 0;

    this->open();

}

void Camera::arrived_terminate()
{
    QByteArray info_1;

    // 信息类型
    info_1.append(0x01);
    info_1.append(0x03);
    qDebug() << info_1;

    emit send_info_to_pc(info_1);
}

void Camera::send_status_to_pc()
{
    QByteArray info_1;

    // 信息类型
    info_1.append(0x01);
    // 小车朝向
    QSettings settings("info.ini",QSettings::IniFormat);
    if (turltebot_direction == FORWARD) {
        // forward
        info_1.append(0x01);
        settings.setValue("node/direction","forward");
    } else {
        info_1.append(0x02);
        settings.setValue("node/direction","backward");
    }
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
    qDebug() << "path_node_distribute_info length" << path_node_distribute_info.split('\n').at(0).split(',').length() - 1;
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
    if (flag_update_path_node == true) {
        emit update_path_node(info_4);
        flag_update_path_node = false;
    }

}

void Camera::on_send_road_boundary(quint16 road_boundary1, quint16 road_boundary2)
{
    this->road_boundary1 = road_boundary1;
    this->road_boundary2 = road_boundary2;
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
    QSettings settings("info.ini", QSettings::IniFormat);
    settings.value("node/direction").toString();
    if (settings.value("node/direction").toString() == "forward") {
        turltebot_direction = FORWARD;
        qDebug() << "小车朝向为正向";
    } else {
        turltebot_direction = BACKWARD;
        qDebug() << "小车朝向为反向";
    }

    arrived_flag = true;
    turltebot_go = false;
    flag_update_path_node = true;

    send_path_node_to_pc();

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
