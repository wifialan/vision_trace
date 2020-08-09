#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Python.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_cam->setScaledContents(true);
    ui->label_cam_2->setScaledContents(true);
    cam = new Camera();
    ros = new Ros();
    path = new Pathplan();
    serial = new QSerialPort();
    protocol = new Protocol();
    socket = new QUdpSocket();
    tcp_server = new QTcpServer();
    timer_serial = new QTimer();
    connect_state  =  false;
    this->port = ARM_PORT;
    this->ip = get_localhost_ip();

    timer_serial->setInterval(500);

    timer_range = new QTimer;
    timer_range->setInterval(50);

    stm32Range = new STM32Range();
    if(stm32Range->OpenRs232()){
        OpenSuccess = true;
        qDebug() << "打开红外传感器成功";
    }else{
        OpenSuccess = false;
        qDebug() << "打开红外传感器失败";
    }

    ros->pub_cmd_vel_topic =  "/cmd_vel_mux/input/navi";
    ros->pub_cmd_vel = ros->n.advertise<geometry_msgs::Twist>(ros->pub_cmd_vel_topic, 1);
    //    ros->loopRate(5);
    ros->speed_x = 0.1;
    ros->flag = 1;
    ros->move_mode = 0;
    connect( (QObject*)this->path, SIGNAL(read_path_plan()), (QObject*)this->cam, SLOT(on_read_path_plan()));
    connect( (QObject*)this->path, SIGNAL(send_path_info_to_camera(QByteArray)), (QObject*)this->cam, SLOT(on_send_path_info_to_camera(QByteArray)));
    connect( (QObject*)this->cam, SIGNAL(show_tutlebot_status(qint16)), this, SLOT(on_show_tutlebot_status(qint16)));
    connect( (QObject*)this->cam, SIGNAL(show_frame(QImage)), this, SLOT(on_show_frame(QImage)));
    connect( (QObject*)this->cam, SIGNAL(show_frame_2(QImage)), this, SLOT(on_show_frame_2(QImage)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_up(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_up(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_down(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_down(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_right(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_right(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_left(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_left(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_turn(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_turn(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_turn_clockwise(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_turn_clockwise(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_turn_counterclockwise(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_turn_counterclockwise(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_stop() ), (QObject*)this->ros, SLOT(on_turltebot_stop()));
    connect( (QObject*)this->cam, SIGNAL(show_command(QByteArray)), this, SLOT(on_show_command(QByteArray)));
    connect( (QObject*)this->cam, SIGNAL(send_info_to_pc(QByteArray)), this, SLOT(on_send_info_to_pc(QByteArray)));
    connect( (QObject*)this->cam, SIGNAL(update_path_node(QByteArray)), this, SLOT(on_update_path_node(QByteArray)));
    connect( (QObject*)this->cam, SIGNAL(update_path_start_node(QByteArray)), this, SLOT(on_update_path_start_node(QByteArray)));
    connect( (QObject*)this->cam, SIGNAL(update_trultebot_direction(bool)), this, SLOT(on_update_trultebot_direction(bool)));

    connect( this, SIGNAL(lanuch_turltebot_go()), this, SLOT(on_pushButton_go_clicked()));
    connect( timer_serial, SIGNAL(timeout()),SLOT(on_timer_serial()));
    connect( timer_range, SIGNAL(timeout()),SLOT(on_timer_range()));

    socket_listen();
    connect(socket, SIGNAL( readyRead() ), this, SLOT( on_read_network() ));
    connect(serial, SIGNAL( readyRead() ), this, SLOT( on_read_serial()  ));

    path->read_json_file();
//    timer_serial->start();
    timer_range->start();

    ui->doubleSpinBox_line_speed->setRange(0.01,0.5);
    ui->doubleSpinBox_line_speed->setSingleStep(0.01);
    ui->doubleSpinBox_line_speed->setDecimals(2);
    ui->doubleSpinBox_line_speed->setValue(0.15);
    ui->doubleSpinBox_angular_speed->setRange(0.01,0.5);
    ui->doubleSpinBox_angular_speed->setSingleStep(0.05);
    ui->doubleSpinBox_angular_speed->setDecimals(2);
    ui->doubleSpinBox_angular_speed->setValue(0.02);
    this->cam->doubleSpinBox_line_speed = ui->doubleSpinBox_line_speed->value();
    this->cam->doubleSpinBox_angular_speed = ui->doubleSpinBox_angular_speed->value();

    ui->horizontalSlider->setMinimum(1);
    ui->horizontalSlider->setMaximum(254);
    ui->horizontalSlider->setValue(35);
    cam->threshold_img = ui->horizontalSlider->value();
    ui->lineEdit->setText(QString::number(cam->threshold_img,10));

    ui->doubleSpinBox_lifter_height->setDecimals(2);
    ui->doubleSpinBox_lifter_height->setRange(2,200);
    ui->doubleSpinBox_lifter_height->setValue(10);
    ui->doubleSpinBox_lifter_height->setSingleStep(0.01);

    ui->spinBox_lifter_speed->setRange(1,100);
    ui->spinBox_lifter_speed->setValue(100);
    ui->spinBox_lifter_speed->setSingleStep(1);

    ui->pushButton_lifter_excute->setEnabled(false);
    ui->pushButton_lifter_set_height->setEnabled(false);
    ui->pushButton_lifter_set_speed->setEnabled(false);
    ui->pushButton_lifter_stop->setEnabled(false);
    ui->pushButton_lifter_zero->setEnabled(false);
    ui->doubleSpinBox_lifter_height->setEnabled(false);
    ui->spinBox_lifter_speed->setEnabled(false);

    // 从文件记录中获取小车朝向
    ui->comboBox_direction->addItem("正向");
    ui->comboBox_direction->addItem("反向");

    QFile file("direction.txt");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (file.isOpen()) {
        qDebug() << "open direction.txt file";
        QByteArray dir = file.readLine();
        qDebug() << dir;
        //        QString str = file.readLine();
        if (dir.contains("0")) {
            ui->comboBox_direction->setCurrentText("正向");
            qDebug() << "小车朝向为正向";
        } else {
            ui->comboBox_direction->setCurrentText("反向");
            qDebug() << "小车朝向为反向";
        }
        file.close();
    }

    on_timer_serial();

    on_pushButton_serial_connect_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_serial_connect_clicked()
{
    serial->close();
    serial->setBaudRate(QSerialPort::Baud19200);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::OddParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    QString portInfo = ui->comboBox_serial->currentText();
//    int number = ui->comboBox_serial->findText("USB-Serial Controller D",Qt::MatchContains);
//    qDebug() << "number" << number;
//    ui->comboBox_serial->setCurrentIndex(number);
//    portInfo = ui->comboBox_serial->currentText();


    QString portName = "/dev/";
    qDebug() << "portInfo" << portInfo;
    quint8 start = portInfo.indexOf("(");
    quint8 stop  = portInfo.indexOf(")");
    for (int i = start + 1; i < stop; ++i) {
        portName.append(portInfo.at(i));
    }

    //QString portName = "lifter";
    qDebug() << "portName" << portName;

    serial->setPortName(portName);
    //    serial->setPort(*infoList);
    if (!serial->open(QIODevice::ReadWrite)) {
        QMessageBox::warning(this,"Warning","Open serial port fail!\n Please see the the information window to solve problem.");
        qDebug() << tr("SYSTEM: The serial port failed to open,Please check as follows: ");
        qDebug() << tr("        1> if the serial port is occupied by other software? ");
        qDebug() << tr("        2> if the serial port connection is normal?");
        qDebug() << tr("        3> if the program is run at root user? You can use the cmd sudo ./(programname) and type your password to be done.");

        ui->pushButton_lifter_excute->setEnabled(false);
        ui->pushButton_lifter_set_height->setEnabled(false);
        ui->pushButton_lifter_set_speed->setEnabled(false);
        ui->pushButton_lifter_stop->setEnabled(false);
        ui->pushButton_lifter_zero->setEnabled(false);
        ui->doubleSpinBox_lifter_height->setEnabled(false);
        ui->spinBox_lifter_speed->setEnabled(false);


        ui->pushButton_serial_disconnect->setEnabled(false);
        ui->comboBox_serial->setEnabled(true);
        ui->pushButton_serial_connect->setEnabled(true);
        ui->statusBar->showMessage("Open:" + portInfo + "failed!" );
    } else {

        ui->pushButton_lifter_excute->setEnabled(true);
        ui->pushButton_lifter_set_height->setEnabled(true);
        ui->pushButton_lifter_set_speed->setEnabled(true);
        ui->pushButton_lifter_stop->setEnabled(true);
        ui->pushButton_lifter_zero->setEnabled(true);
        ui->doubleSpinBox_lifter_height->setEnabled(true);
        ui->spinBox_lifter_speed->setEnabled(true);


        ui->pushButton_serial_disconnect->setEnabled(true);
        ui->comboBox_serial->setEnabled(false);
        ui->pushButton_serial_connect->setEnabled(false);
        qDebug() << tr("SYSTEM: The system has been connected with ")+portInfo+" " ;
    }
}

void MainWindow::on_pushButton_serial_disconnect_clicked()
{
    serial->clear();
    serial->close();
    ui->pushButton_lifter_excute->setEnabled(false);
    ui->pushButton_lifter_set_height->setEnabled(false);
    ui->pushButton_lifter_set_speed->setEnabled(false);
    ui->pushButton_lifter_stop->setEnabled(false);
    ui->pushButton_lifter_zero->setEnabled(false);
    ui->doubleSpinBox_lifter_height->setEnabled(false);
    ui->spinBox_lifter_speed->setEnabled(false);
    ui->pushButton_serial_disconnect->setEnabled(false);
    ui->comboBox_serial->setEnabled(true);
    ui->pushButton_serial_connect->setEnabled(true);
}


void MainWindow::on_timer_serial()
{
    QStringList newPortStringList;
    QSerialPortInfo info;
    //搜索串口

    foreach (info, QSerialPortInfo::availablePorts()){
#if 0
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
#endif
        newPortStringList += (info.description() + '(' + info.portName() + ")");
    }

    //更新旧的串口列表
    if(newPortStringList != oldPortStringList)
    {
        ui->comboBox_serial->clear();
        oldPortStringList = newPortStringList;
        qDebug() << oldPortStringList;
        for (int i = 0; i < oldPortStringList.length(); ++i) {
            ui->comboBox_serial->addItem( oldPortStringList.at(i));
        }
        if (!serial->isBreakEnabled()) {//检测串口是否掉线
            ui->pushButton_serial_disconnect->setEnabled(false);
            ui->comboBox_serial->setEnabled(true);
            ui->pushButton_serial_connect->setEnabled(true);
        }
        //            emit onNewSerialPort(oldPortStringList);
    }
    int number = ui->comboBox_serial->findText("USB-Serial Controller D",Qt::MatchContains);
    qDebug() << "number" << number;
    ui->comboBox_serial->setCurrentIndex(number);

}

void MainWindow::on_timer_range(){
    if(OpenSuccess){
        stm32Range->rangeData_vec_.clear();
        stm32Range->RecvRangeData();
        for(int i = 0;i<stm32Range->rangeData_vec_.size();i++){
            if(stm32Range->rangeData_vec_[i].range < 0.3){
                    emit cam->show_tutlebot_status(TURLTEBOT_STOP);
                    cam->lock_status(TURLTEBOT_STOP);
                    emit cam->turltebot_stop();
            }
            else{
                //go
            }
        }
    }
}


QString MainWindow::get_localhost_ip()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol){
            //我们使用IPv4地址
            for (int i = 0; i < address.toString().split('\n').length(); ++i) {
                if (address.toString().split('\n').at(i) != "127.0.0.1") {
                    qDebug() <<"本机ip地址:" << address.toString().split('\n').at(i);
                    return address.toString().split('\n').at(i);
                }
            }
        }
    }
}


void MainWindow::on_update_path_node(QByteArray info_4)
{
    quint16 header_index = info_4.indexOf('#');
    quint16 tail_index = info_4.lastIndexOf('#');
    quint16 info_len = tail_index - header_index - 1;
    qDebug() << "header_index" << header_index;
    qDebug() << "tail_index" << tail_index;
    qDebug() << "info_len" << info_len;
    ui->comboBox_start_node->clear();
    ui->comboBox_stop_node->clear();
    for (int i = 1; i <= info_len; ++i) {
        //        qDebug() << QString::number(quint8(info_4.at(header_index + i)),10);
        ui->comboBox_start_node->addItem(QString::number(quint8(info_4.at(header_index + i)),10));
        ui->comboBox_stop_node->addItem(QString::number(quint8(info_4.at(header_index + i)),10));
    }
    ui->comboBox_start_node->setCurrentIndex(0);
    ui->comboBox_stop_node->setCurrentIndex(2);
    qDebug() << "Add success";
}

void MainWindow::on_update_path_start_node(QByteArray path_start_node)
{
    QString str = path_start_node.split('\n').at(0);
    qint8 str_index = ui->comboBox_start_node->findText(str);
    ui->comboBox_start_node->setCurrentIndex(str_index);
}

void MainWindow::socket_listen()
{
    if(!tcp_server->listen(QHostAddress::Any,8989))
    {
        qDebug()<<"错误"<<tcp_server->errorString();
        close();
        return;
    }
    connect(tcp_server,SIGNAL(newConnection()),this,SLOT(on_new_connect_tcp()));

    flag_is_connect_to_tcp = false;
}

qint16 MainWindow::socket_connect()
{
    qDebug() << "开始绑定端口" << this->ip << " " << this->port;
    if ( ip.isEmpty() || port > 9999 ){
        qDebug() << "IP or PORT 不正确";
        return ERROR_USER_INPUT;
    }
    if ( socket->bind(QHostAddress(this->ip), this->port) ){
        qDebug() << "绑定端口：" << this->port << "成功";
        return ERROR_NO_ERROR;
    }
    else {
        qDebug() << "绑定端口失败";
        connect_state = true;
        return ERROR_SYSTEM_NETWORK;
    }
    return 0;
}

void MainWindow::on_new_connect_tcp()
{
    //获取已经建立的连接的套接字
    tcp_client = tcp_server->nextPendingConnection();
    qDebug() << "connect client:";
    qDebug() << tcp_client->peerAddress().toString();
    qDebug() << tcp_client->peerAddress().toString() << ":" << tcp_client->peerPort();
    //qDebug() << tcp_client->peerAddress().toString().split("::ffff:")[1] << ":" << tcp_client->peerPort();
    connect(tcp_client,SIGNAL(readyRead()),this,SLOT(on_read_network()));
    connect(tcp_client,SIGNAL(disconnected()),tcp_client,SLOT(deleteLater()));
    connect(tcp_client,SIGNAL(disconnected()),this,SLOT(on_disconnected()));

    flag_is_connect_to_tcp = true;

    cam->send_path_node_to_pc();
    cam->send_status_to_pc();
    //
}

//void MainWindow::readMesage()
//{
//    QByteArray array;

//    //array.resize( tcp_client->bytesAvailable() );
//    array.append(tcp_client->readAll());
//    qDebug() << "read: " << array;

//}

void MainWindow::on_disconnected()
{
    flag_is_connect_to_tcp = false;
}

void MainWindow::socket_disconnect()
{
    connect_state = false;
    socket->close();
}

void MainWindow::on_send_info_to_pc(QByteArray info)
{
    QByteArray request_array;
    request_array.append(0xAA);
    request_array.append(0xBB);
    request_array.append(0xCC);
    request_array.append(0xDD); // header
    request_array.append(info); // info
    request_array.append(0xCC);
    request_array.append(0xFF); // end
    qDebug() << "request_array" << request_array;
    write_socket(request_array);
}

void MainWindow::write_socket(QByteArray array)
{

    if(flag_is_connect_to_tcp == true)
    {
        tcp_client->write(array);
    }

#if USE_UDP
    quint64 len;
    len = socket->writeDatagram( array, QHostAddress(PC_IP), PC_PORT );
    if (len != array.length() ) {
        qDebug() << "udp retransmit!";
    }
#endif
}

void MainWindow::write_socket(quint8 *buffer, quint32 len)
{
    quint64 sendLen;
    QByteArray array;
    array.append((char*)buffer , len );
    sendLen = socket->writeDatagram((char*)buffer, len, QHostAddress(PC_IP), PC_PORT);
}

void MainWindow::on_read_network()
{
    QByteArray array;

    array.append( tcp_client->readAll() );
    //如果接收到的是检测TCP是否在线的，直接返回即可
    if( array.contains("detect tcp state") )
    {
        return;
    }
    qDebug() << "read: " << array;
    COM_PAC pac = decode_protocal( array );
    if ( pac.flag != true )
        return;

    qDebug() <<  "info type :" << pac.info_type;

    switch (pac.info_type) {
    case INFO_STATUS:
        analyze_info_status(pac);
        break;
    case INFO_PING:
        analyze_info_ping(pac);
        break;
    case INFO_COMMAND:
        analyze_info_command(pac);
        break;
    case INFO_UPDATE:
        analyze_info_update();
        break;
    case INFO_SPEED_VALUE:
        analyze_info_speed(pac);
        break;
    default:
        break;
    }
}

void MainWindow::analyze_info_speed(COM_PAC pac)
{

}

void MainWindow::analyze_info_update()
{
    cam->send_path_node_to_pc();
    cam->send_status_to_pc();
}

void MainWindow::analyze_info_status(COM_PAC pac)
{
    // 从文件记录中获取小车朝向
    QFile file("direction.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream.seek(file.size());
    if (pac.direction == TURLTEBOT_FORWARD) {
        // forward
        stream << "0";
    } else {
        stream << "1";
    }
    file.flush();
    file.close();

    ui->comboBox_start_node->setCurrentText(QString::number(pac.start_node,10));
    ui->comboBox_stop_node->setCurrentText(QString::number(pac.stop_node,10));
    emit lanuch_turltebot_go();
}

void MainWindow::analyze_info_ping(COM_PAC pac)
{
    if (pac.ping_flag == PC_QT_FLAG) {
        qDebug() << "Get ping info from pc qt";
        if (pac.ping_back == PING_BACK_TRUE) {
            // need ping back
            qDebug() << "Ping back";
            QByteArray info_2;
            info_2.append(0x02);
            info_2.append(ARM_QT_FLAG);
            info_2.append(PING_BAC_FALSE);
            send_cmd_to_tcp(info_2);
        }
    }
}

void MainWindow::analyze_info_command(COM_PAC pac)
{
    switch (pac.ctrl_command) {
    case CMD_UP:
        ros->speed.linear.x = 0.06;
        ros->speed.angular.z = 0;
        ui->textBrowser->append("SYSTEM: send cmd [UP]");
        ros->pub_cmd_vel.publish(ros->speed); //
        break;
    case CMD_LEFT:
        ros->speed.linear.x = 0.04;
        ros->speed.angular.z = 0.2;
        send_cmd_serial(CMD_LEFT);
        ui->textBrowser->append("SYSTEM: send cmd [LEFT]");
        ros->pub_cmd_vel.publish(ros->speed); //
        break;
    case CMD_RIGHT:
        ros->speed.linear.x = 0.04;
        ros->speed.angular.z = -0.2;
        send_cmd_serial(CMD_RIGHT);
        ui->textBrowser->append("SYSTEM: send cmd [RIGHT]");
        ros->pub_cmd_vel.publish(ros->speed); //
        break;
    case CMD_DOWN:
        ros->speed.linear.x = -0.06;
        ros->speed.angular.z = 0;
        ui->textBrowser->append("SYSTEM: send cmd [DOWN]");
        ros->pub_cmd_vel.publish(ros->speed); //
        break;
    case CMD_STOP:
        cam->turltebot_go = false;
        ui->textBrowser->append("SYSTEM: send cmd [STOP]");
        ros->speed.linear.x = 0; //
        ros->speed.angular.z = 0; //
        ros->pub_cmd_vel.publish(ros->speed); //
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_lifter_zero_clicked()
{
    const char *cmd;
    read_serial.clear();
    cmd = protocol->GetSRCmd(0x2,1);
    QString str = QString(QLatin1String(cmd));
    //qDebug() << "----------------";
    qDebug() << str.toLatin1();

    serial->write(str.toLatin1());
}

void MainWindow::on_read_serial()
{

    read_serial.append(serial->readAll());
    if(read_serial.contains("\r") == true)
        qDebug() << read_serial;
}

void MainWindow::on_pushButton_lifter_set_height_clicked()
{
    const char *cmd;
    QString str;
    QString input = ui->doubleSpinBox_lifter_height->text();
    if(input.isEmpty() == true)
    {
        QMessageBox::warning(this,"Warning","Please input the data...");
        return;
    }
    double input_data = input.toDouble();// 单位 cm
    quint64 input_data_quint64 = input_data * 10; // 单位 mm
    quint64 offset_hight = 1000;// 单位 mm

    qDebug() << "change hight: " << (input_data_quint64 / 10.0) << "cm";

    quint64 write_data = offset_hight + input_data_quint64;

    qDebug() << write_data * 100;
    read_serial.clear();

    cmd = protocol->GetWriteDWordCmd(2,write_data * 100);

    str = QString(QLatin1String(cmd));
    qDebug() << str;
    serial->write(str.toLatin1());

    // 更新高度信息到PC端
    quint16 height = ui->doubleSpinBox_lifter_height->value()*100;
    quint8  height_high = (height >> 8) & 0x00FF;
    quint8  height_low =   height & 0x00FF;
    qDebug() << height;
    QByteArray info_7;
    info_7.append(0x07);
    info_7.append(LIFTER_SET_HEIGHT);
    info_7.append(height_high);
    info_7.append(height_low);

    send_cmd_to_tcp(info_7);

}

void MainWindow::on_pushButton_lifter_excute_clicked()
{
    const char *cmd;
    read_serial.clear();
    cmd = protocol->GetSRCmd(0x1,1);

    QString str = QString(QLatin1String(cmd));
    qDebug() << str.toLatin1();
    serial->write(str.toLatin1());

}

void MainWindow::on_pushButton_lifter_stop_clicked()
{
    read_serial.clear();
    const char *cmd;
    cmd = protocol->GetSRCmd(0x4,1);
    QString str = QString(QLatin1String(cmd));
    qDebug() << str.toLatin1();

    serial->write(str.toLatin1());
}

void MainWindow::send_cmd_to_tcp(quint8 cmd, QByteArray &value_array)
{
    QByteArray request_array;
    request_array.append(0xAA);
    request_array.append(0xBB);
    request_array.append(0xCC);
    request_array.append(0xDD);     // header
    request_array.append(CMD_ALTER_ID);     // id
    request_array.append(cmd);      // cmd
    request_array.append(value_array.length()); // len
    request_array.append(value_array);
    request_array.append(0xCC);
    request_array.append(0xFF);
    write_socket(request_array);
}
void MainWindow::send_cmd_to_tcp(quint8 cmd)
{
    QByteArray request_array;
    request_array.append(0xAA);
    request_array.append(0xBB);
    request_array.append(0xCC);
    request_array.append(0xDD);     // header
    request_array.append(CMD_ALTER_ID);     // id
    request_array.append(cmd);      // cmd
    request_array.append((char)0);        // len
    request_array.append(0xCC);
    request_array.append(0xFF);
    write_socket(request_array);
}
void MainWindow::send_cmd_to_tcp(QByteArray info)
{
    QByteArray request_array;
    request_array.append(0xAA);
    request_array.append(0xBB);
    request_array.append(0xCC);
    request_array.append(0xDD);                 // header
    request_array.append(info); // info
    request_array.append(0xCC);
    request_array.append(0xFF);                 // end
    write_socket(request_array);
}
COM_PAC MainWindow::decode_protocal(QByteArray array)
{
    double value;
    bool ok;
    QString str;
    int tmp;
    quint16 header_index;
    QByteArray header_array;
    QByteArray tail_array;
    COM_PAC pac;
    header_array.append(0xAA);
    header_array.append(0xBB);
    header_array.append(0xCC);
    header_array.append(0xDD);
    tail_array.append(0xCC);
    tail_array.append(0xFF);

    if ( !(array.contains( header_array ) && array.contains( tail_array )
           && array.indexOf( header_array ) < array.indexOf( tail_array ))
         ) {
        qDebug() << "cmd array illegel: ";
        qDebug() << " ||||||  " << array << " ||||||";
        pac.flag = false;
        return pac;
    }
    header_index = array.indexOf( header_array ) + 3;
    pac.info_type =  array.at( header_index + 1 );
    switch (pac.info_type) {
    case INFO_STATUS:
        pac.direction = array.at( header_index + 2 );
        pac.start_node = array.at( header_index + 3 );
        pac.stop_node = array.at( header_index + 4 );
        pac.current_node = array.at( header_index + 5 );
        pac.current_node = array.at( header_index + 6 );
        pac.flag = true;
        break;
    case INFO_PING:
        pac.ping_flag = array.at( header_index + 2 );
        pac.ping_back = array.at( header_index + 3 );
        pac.flag = true;
        break;
    case INFO_PATH_NODE:
        pac.path_node = array.mid( header_index + 3, array.at(header_index + 2));
        pac.flag = true;
        break;
    case INFO_COMMAND:
        pac.ctrl_command = array.at( header_index + 2 );
        pac.flag = true;
        break;
    case INFO_UPDATE:
        pac.flag = true;
        break;
    case INFO_SPEED_VALUE:

        pac.line_speed = array.at(header_index + 2)&0xFF;
        pac.angular_speed = array.at(header_index + 3)&0xFF;
        //        value = array.split(',').at(1).toHex();
        //        str = array.split(',').at(1);
        pac.line_speed /= 100.0;
        pac.angular_speed /= 100.0;

        qDebug("line speed: %.2f",pac.line_speed);
        qDebug("angular speed: %.2f",pac.angular_speed);

        ui->doubleSpinBox_line_speed->setValue(pac.line_speed);
        ui->doubleSpinBox_angular_speed->setValue(pac.angular_speed);

        this->cam->doubleSpinBox_line_speed = ui->doubleSpinBox_line_speed->value();
        this->cam->doubleSpinBox_angular_speed = ui->doubleSpinBox_angular_speed->value();

        break;
    case INFO_LIFTER:

        pac.lifter_data_type = array.at(header_index + 2);

        if(pac.lifter_data_type == LIFTER_SET_HEIGHT){
            pac.lifter_height_hight = array.at(header_index + 3);
            pac.lifter_height_low = (array.at(header_index + 4)&0xFF);
//            qDebug() << "pac.lifter_height_hight" << pac.lifter_height_hight;
//            qDebug() << "pac.lifter_height_low" << pac.lifter_height_low;

            pac.lifter_height = ((pac.lifter_height_hight << 8) & 0xFF00) | pac.lifter_height_low;
            pac.lifter_height = pac.lifter_height / 100.0;
            ui->doubleSpinBox_lifter_height->setValue(pac.lifter_height);
            on_pushButton_lifter_set_height_clicked();
            qDebug("pac.lifter_heiht %.2f",pac.lifter_height);
        } else if(pac.lifter_data_type == LIFTER_SET_SPEED){
            pac.lifter_speed = array.at(header_index + 3)&0xFF;
            ui->spinBox_lifter_speed->setValue(pac.lifter_speed);
            on_pushButton_lifter_set_speed_clicked();
            qDebug() << "pac.lifter_speed" << pac.lifter_speed;
        } else if(pac.lifter_data_type == LIFTER_SET_ZERO){
            // zero
            on_pushButton_lifter_zero_clicked();
        }else if(pac.lifter_data_type == LIFTER_STOP){
            on_pushButton_lifter_stop_clicked();
            // stop
        }else if(pac.lifter_data_type == LIFTER_EXCUTE){
            on_pushButton_lifter_excute_clicked();
            // LIFTER_EXCUTE
        }

        qDebug() << "INFO_LIFTER";

        break;
    default:
        break;
    }

    return pac;
}


void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_pushButton_up_clicked()
{

    ros->speed.linear.x = 0.06;
    ros->speed.angular.z = 0;
    ui->textBrowser->append("SYSTEM: send cmd [UP]");
    ros->pub_cmd_vel.publish(ros->speed); //
    //    emit TURTLEBOT_up();
    //    ros->move_mode = TURLTEBOT_UP;
    //    ros->terminate();
    //    while(!ros->wait());
    //    ros->start();
    //    loopRate.sleep();
}

void MainWindow::on_pushButton_down_clicked()
{
    //    send_cmd_serial(CMD_DOWN);

    ros->speed.linear.x = -0.06;
    ros->speed.angular.z = 0;
    ui->textBrowser->append("SYSTEM: send cmd [DOWN]");
    ros->pub_cmd_vel.publish(ros->speed); //

    //    emit TURTLEBOT_down();
    //    ros->move_mode = TURLTEBOT_DOWN;

    //    ros->terminate();
    //    while(!ros->wait());
    //    ros->start();

}

void MainWindow::on_pushButton_left_clicked()
{
    ros->speed.linear.x = 0.04;
    ros->speed.angular.z = 0.2;
    send_cmd_serial(CMD_LEFT);
    ui->textBrowser->append("SYSTEM: send cmd [LEFT]");
    ros->pub_cmd_vel.publish(ros->speed); //
    //    ros->move_mode = TURLTEBOT_LEFT;
    //    ros->terminate();
    //    while(!ros->wait());
    //    ros->start();
}

void MainWindow::on_pushButton_right_clicked()
{
    ros->speed.linear.x = 0.04;
    ros->speed.angular.z = -0.2;
    //send_cmd_serial(CMD_RIGHT);
    ui->textBrowser->append("SYSTEM: send cmd [RIGHT]");
    ros->pub_cmd_vel.publish(ros->speed); //
    //    ros->move_mode = TURLTEBOT_RIGHT;
    //    ros->terminate();
    //    while(!ros->wait());
    //    ros->start();
}

void MainWindow::send_cmd_serial(quint8 cmd)
{
    QByteArray array;
    array.append(0xAA);
    array.append(0xBB);
    array.append(cmd);
    array.append(0xCC);
}



void MainWindow::on_pushButton_stop_clicked()
{

    //cam->timer->stop();
    cam->turltebot_go = false;
    ui->textBrowser->append("SYSTEM: send cmd [STOP]");
    ros->terminate();
    while(!ros->wait());
    ros->speed.linear.x = 0; //
    ros->speed.angular.z = 0; //
    ros->pub_cmd_vel.publish(ros->speed); //


}

void MainWindow::on_show_frame(QImage image)
{
    //    qDebug() << "show image";
    ui->label_cam->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_show_frame_2(QImage image)
{
    //    qDebug() << "show image";
    ui->label_cam_2->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_show_tutlebot_status(qint16 status)
{
    switch (status) {
    case TURLTEBOT_UP:
        ui->textBrowser->append("SYSTEM: send cmd [UP]");
        break;
    case TURLTEBOT_LEFT:
        ui->textBrowser->append("SYSTEM: send cmd [LEFT]");
        break;
    case TURLTEBOT_RIGHT:
        ui->textBrowser->append("SYSTEM: send cmd [RIGHT]");
        break;
    case TURLTEBOT_STOP:
        ui->textBrowser->append("SYSTEM: send cmd [STOP]");
        break;
    case TURLTEBOT_TURN:
        ui->textBrowser->append("SYSTEM: send cmd [TURN]");
        break;
    default:
        break;
    }
}

void MainWindow::on_show_command(QByteArray path_plan_array){
    qDebug() << "on_show_command";
    ui->lineEdit_command->setText(QString(path_plan_array));
}

void MainWindow::on_update_trultebot_direction(bool direction)
{
    if(direction == FORWARD)
    {
        qDebug() << "******FORWARD";
        ui->comboBox_direction->setCurrentText("正向");
    }
    if(direction == BACKWARD)
    {
        qDebug() << "******BACKWARD";
        ui->comboBox_direction->setCurrentText("反向");
    }
}

void MainWindow::on_pushButton_go_clicked()
{

    cam->turltebot_go = true;
    cam->capture.~VideoCapture();
    QString start_point = ui->comboBox_start_node->currentText();
    QString stop_point = ui->comboBox_stop_node->currentText();
    if (start_point == stop_point) {
        return;
    }

    // 从文件记录中获取小车朝向
    QFile file_direction("direction.txt");
    file_direction.open(QIODevice::ReadOnly | QIODevice::Text);
    if (file_direction.isOpen()) {
        qDebug() << "open direction.txt file";
        QByteArray dir = file_direction.readLine();
        qDebug() << dir;
        //        QString str = file.readLine();
        if (dir.contains("0")) {
            cam->turltebot_direction = FORWARD;
            ui->comboBox_direction->setCurrentText("正向");
            qDebug() << "小车朝向为正向";
        } else {
            cam->turltebot_direction = BACKWARD;
            ui->comboBox_direction->setCurrentText("反向");
            qDebug() << "小车朝向为反向";
        }
        file_direction.close();
    }

    cam->start_stop_node_array.clear();
    cam->start_stop_node_array.append(start_point);
    cam->start_stop_node_array.append('\n');
    cam->start_stop_node_array.append(stop_point);
    qDebug() << "start_stop_node_array **" << cam->start_stop_node_array;

    QFile file("node.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream.seek(file.size());
    stream << start_point << "\n";
    stream << stop_point;
    file.flush();
    file.close();

    send_start_stop_node_to_pc();

    qDebug() << "start cap";
    if (cam->capture.isOpened()){
        cam->capture.release();     //decide if capture is already opened; if so,close it
    }
    cam->capture.open(cam->camera_number);           //open the default camera
    qDebug() << "open cap";
    path->proc->execute("python3 /home/igosens/huawei/softeware/path.py");
    emit path->read_path_plan();
    cam->timer->start();

}

void MainWindow::send_start_stop_node_to_pc()
{
    QByteArray info_1;
    // 信息类型
    info_1.append(0x01);
    // 小车朝向(ARM端会忽略)
    if(ui->comboBox_direction->currentText() == "正向"){
        info_1.append(TURLTEBOT_FORWARD);
    }else {
        info_1.append(TURLTEBOT_BACKWARD);
    }
    // 起始节点

    QString start_node = ui->comboBox_start_node->currentText();
    info_1.append(start_node.toInt());
    // 终点节点
    QString stop_node = ui->comboBox_stop_node->currentText();
    info_1.append(stop_node.toInt());
    // 当前节点(ARM端会忽略)
    info_1.append(0x01);
    // 移动信息(ARM端会忽略)
    info_1.append(0x01);

    send_cmd_to_tcp(info_1);
}

void MainWindow::on_comboBox_direction_currentIndexChanged(int index)
{
    QFile file("direction.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream.seek(file.size());
    if (index == 0) {
        // forward
        stream << "0";
        this->cam->turltebot_direction = FORWARD;
        qDebug() << "改变朝向为正向";
    } else {
        this->cam->turltebot_direction = BACKWARD;
        stream << "1";
        qDebug() << "改变朝向为反向";
    }
    file.flush();
    file.close();

}

void MainWindow::on_doubleSpinBox_line_speed_valueChanged(double arg1)
{
    this->cam->doubleSpinBox_line_speed = arg1;
    qDebug() << "current line speed: " << arg1;
    QByteArray info_6;
    info_6.append(0x06);
    //info_6.append(LINE_SPEED);
    info_6.append((arg1*100));
    info_6.append(ui->doubleSpinBox_angular_speed->value()*100);
    send_cmd_to_tcp(info_6);
}

void MainWindow::on_doubleSpinBox_angular_speed_valueChanged(double arg1)
{
    this->cam->doubleSpinBox_angular_speed = arg1;
    qDebug() << "current angular speed: " << arg1;
    QByteArray info_6;
    info_6.append(0x06);
    info_6.append(ui->doubleSpinBox_line_speed->value()*100);
    info_6.append((arg1*100));
    send_cmd_to_tcp(info_6);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    cam->threshold_img = value;
    ui->lineEdit->setText(QString::number(value,10));
}


void MainWindow::on_pushButton_lifter_set_speed_clicked()
{
    const char *cmd;
    QString str;
    QString input = ui->spinBox_lifter_speed->text();
    if(input.isEmpty() == true)
    {
        QMessageBox::warning(this,"Warning","Please input the data...");
        return;
    }
    int input_data = input.toInt();// 单位 cm

    read_serial.clear();

    cmd = protocol->GetWriteDWordCmd(0,input_data);

    str = QString(QLatin1String(cmd));
    qDebug() << str;
    serial->write(str.toLatin1());

    //更新速度信息到PC 端
    int speed = ui->spinBox_lifter_speed->value();
    qDebug() << speed;
    QByteArray info_7;
    info_7.append(0x07);
    info_7.append(LIFTER_SET_SPEED);
    info_7.append(speed);
    send_cmd_to_tcp(info_7);

}

