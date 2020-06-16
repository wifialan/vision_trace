#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Python.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label_cam->setScaledContents(true);
    ui->pushButton_serial_disconnect->setEnabled(false);
    ui->pushButton_serial_connect->setEnabled(true);
    //    ui->label_cam_2->setScaledContents(true);
    cam = new Camera();
    ros = new Ros();
    path = new Pathplan();
    serial = new QSerialPort();
    socket = new QUdpSocket();
    timer_serial = new QTimer;
    timer_serial->setInterval(500);
    connect_state  =  false;
    this->port = HOST_PORT;
    this->ip = get_localhost_ip();

    ros->pub_cmd_vel_topic =  "/cmd_vel_mux/input/navi";
    ros->pub_cmd_vel = ros->n.advertise<geometry_msgs::Twist>(ros->pub_cmd_vel_topic, 1);
    //    ros->loopRate(5);
    ros->speed_x = 0.1;
    ros->flag = 1;
    ros->move_mode = 0;
    connect( (QObject*)this->path, SIGNAL(read_path_plan()), (QObject*)this->cam, SLOT(on_read_path_plan()));
    connect( (QObject*)this->path, SIGNAL(send_path_info_to_camera(QByteArray)), (QObject*)this->cam, SLOT(on_send_path_info_to_camera(QByteArray)));
    connect( (QObject*)this->cam, SIGNAL(show_tutlebot_status(qint8)), this, SLOT(on_show_tutlebot_status(qint8)));
    connect( (QObject*)this->cam, SIGNAL(show_frame(QImage)), this, SLOT(on_show_frame(QImage)));
    connect( (QObject*)this->cam, SIGNAL(show_frame_2(QImage)), this, SLOT(on_show_frame_2(QImage)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_up(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_up(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_down(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_down(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_right(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_right(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_left(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_left(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_turn(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_turn(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_stop() ), (QObject*)this->ros, SLOT(on_turltebot_stop()));
    connect( (QObject*)this->cam, SIGNAL(show_command(QByteArray)), this, SLOT(on_show_command(QByteArray)));
    connect( (QObject*)this->cam, SIGNAL(send_info_to_pc(QByteArray)), this, SLOT(on_send_info_to_pc(QByteArray)));
    connect( (QObject*)this->cam, SIGNAL(update_path_node(QByteArray)), this, SLOT(on_update_path_node(QByteArray)));
    connect( (QObject*)this->cam, SIGNAL(update_path_start_node(QByteArray)), this, SLOT(on_update_path_start_node(QByteArray)));
    connect( this, SIGNAL(lanuch_turltebot_go()), this, SLOT(on_pushButton_go_clicked()));
    connect( timer_serial, SIGNAL(timeout()),SLOT(on_timer_serial()));
    socket_connect();
    connect(socket,                 \
            SIGNAL( readyRead() ), \
            this,               \
            SLOT( on_read_network() )
            );
    connect( serial,              \
             SIGNAL( readyRead()),\
             this,                 \
             SLOT( on_read_serial()));

    oldPortStringList.clear();
    timer_serial->start();
    path->read_json_file();
    //    cam->open();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    //    qDebug() << "header_index" << header_index;
    //    qDebug() << "tail_index" << tail_index;
    //    qDebug() << "info_len" << info_len;
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

qint16 MainWindow::socket_connect()
{
    if ( ip.isEmpty() | port > 9999 )
        return ERROR_USER_INPUT;
    if ( socket->bind(QHostAddress(this->ip), HOST_PORT) ){
        qDebug() << "绑定端口：" << HOST_PORT;
        return ERROR_NO_ERROR;
    }
    else {
        connect_state = true;
        return ERROR_SYSTEM_NETWORK;
    }
    return 0;
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
    write_socket(request_array);
    qDebug() << request_array;
}

void MainWindow::write_socket(QByteArray array)
{
    quint64 len;
    len = socket->writeDatagram( array, QHostAddress(PC_IP), PC_PORT );
    if (len != array.length() ) {
        qDebug() << "udp retransmit!";
    }
}

void MainWindow::write_socket(quint8 *buffer, quint32 len)
{
    quint64 sendLen;
    QByteArray array;
    array.append((char*)buffer , len );
    sendLen = socket->writeDatagram((char*)buffer, len, QHostAddress(PC_IP), this->port);
}

void MainWindow::on_read_network()
{
    QByteArray array;
    QHostAddress address;
    quint16 xport;
    xport = HOST_PORT;
    address.setAddress(ip);
    array.resize( socket->bytesAvailable() );
    socket->readDatagram( array.data(),array.size() );
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
    case INFO_UPDATE:
        analyze_info_update();
        break;
    default:
        break;
    }
}

void MainWindow::analyze_info_update()
{
    cam->send_path_node_to_pc();
    cam->send_status_to_pc();
}

void MainWindow::analyze_info_status(COM_PAC pac)
{
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
            send_cmd_to_udp(info_2);
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
        ui->textBrowser->append("SYSTEM: send cmd [STOP]");
        ros->speed.linear.x = 0; //
        ros->speed.angular.z = 0; //
        ros->pub_cmd_vel.publish(ros->speed); //
        break;
    default:
        break;
    }
}

void MainWindow::on_read_serial()
{
    QByteArray array;
    QByteArray header;
    QByteArray tail;

    array = serial->readAll();
//    qDebug() << array;

    header.append(0xAA);
    header.append(0xBB);
    tail.append(0xEE);
    tail.append(0xFF);

    if (array.contains(header) && array.contains(tail) && (array.indexOf(header) < array.indexOf(tail))) {
        cam->path_status = array.at(2);
        QString str = QString::number(cam->path_status,10);
        ui->lineEdit_path_status->setText(str);
//        qDebug() << cam->path_status;
    }
//    qDebug() << "---------------------------";
}
void MainWindow::send_cmd_to_udp(quint8 cmd, QByteArray &value_array)
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
void MainWindow::send_cmd_to_udp(quint8 cmd)
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
void MainWindow::send_cmd_to_udp(QByteArray info)
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
    default:
        break;
    }

    return pac;
}

void MainWindow::on_pushButton_clicked()
{
    QApplication::quit();
}

void MainWindow::on_pushButton_up_clicked()
{
    ros->speed.linear.x = 0.06;
    ros->speed.angular.z = 0;
    ui->textBrowser->append("SYSTEM: send cmd [UP]");
    ros->pub_cmd_vel.publish(ros->speed); //
    cam->send_ctrl_to_pc(TURLTEBOT_UP);
}

void MainWindow::on_pushButton_down_clicked()
{
    //    send_cmd_serial(CMD_DOWN);

    ros->speed.linear.x = -0.06;
    ros->speed.angular.z = 0;
    ui->textBrowser->append("SYSTEM: send cmd [DOWN]");
    ros->pub_cmd_vel.publish(ros->speed); //
    cam->send_ctrl_to_pc(TURLTEBOT_DOWN);

}

void MainWindow::on_pushButton_left_clicked()
{
    ros->speed.linear.x = 0.04;
    ros->speed.angular.z = 0.2;
    send_cmd_serial(CMD_LEFT);
    ui->textBrowser->append("SYSTEM: send cmd [LEFT]");
    ros->pub_cmd_vel.publish(ros->speed); //
    cam->send_ctrl_to_pc(TURLTEBOT_LEFT);

}

void MainWindow::on_pushButton_right_clicked()
{
    ros->speed.linear.x = 0.04;
    ros->speed.angular.z = -0.2;
    send_cmd_serial(CMD_RIGHT);
    ui->textBrowser->append("SYSTEM: send cmd [RIGHT]");
    ros->pub_cmd_vel.publish(ros->speed); //
    cam->send_ctrl_to_pc(TURLTEBOT_RIGHT);
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
    cam->turltebot_go = false;
    ui->textBrowser->append("SYSTEM: send cmd [STOP]");
    ros->speed.linear.x = 0; //
    ros->speed.angular.z = 0; //
    ros->pub_cmd_vel.publish(ros->speed); //
    cam->send_ctrl_to_pc(TURLTEBOT_STOP);

}

void MainWindow::on_show_frame(QImage image)
{
    ui->label_cam->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_show_frame_2(QImage image)
{
    //    qDebug() << "show image";
    //    ui->label_cam_2->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_show_tutlebot_status(qint8 status)
{
    cam->send_ctrl_to_pc(status);
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
    ui->lineEdit_command->setText(QString(path_plan_array));
}

void MainWindow::on_pushButton_go_clicked()
{
    cam->flag_qr_detect_crossroad = false;
    cam->flag_capture_grap = true;
    cam->turltebot_go = true;
    cam->first_step = true;
    QString start_point = ui->comboBox_start_node->currentText();
    QString stop_point = ui->comboBox_stop_node->currentText();
    if (start_point == stop_point) {
        return;
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
    qDebug() << "start cap";
    if (cam->capture.isOpened()){
        cam->capture.release();     //decide if capture is already opened; if so,close it
    }
    cam->capture.open(cam->camera_number);           //open the default camera
    qDebug() << "open cap";
    path->proc->execute("python3 ../vision_trace_host/path.py");
    emit path->read_path_plan();

}

void MainWindow::on_pushButton_serial_connect_clicked()
{
    serial->close();
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    QString portInfo = ui->comboBox_serial->currentText();
    QString portName = "/dev/";
    quint8 start = portInfo.indexOf("(");
    quint8 stop  = portInfo.indexOf(")");
    for (int i = start + 1; i < stop; ++i) {
        portName.append(portInfo.at(i));
    }

    qDebug() << portName;

    serial->setPortName(portName);
//    serial->setPort(*infoList);
    if (!serial->open(QIODevice::ReadWrite)) {
            QMessageBox::warning(this,"Warning","Open serial port fail!\n Please see the the information window to solve problem.");
            qDebug() << tr("SYSTEM: The serial port failed to open,Please check as follows: ");
            qDebug() << tr("        1> if the serial port is occupied by other software? ");
            qDebug() << tr("        2> if the serial port connection is normal?");
            qDebug() << tr("        3> if the program is run at root user? You can use the cmd sudo ./(programname) and type your password to be done.");

            ui->pushButton_serial_disconnect->setEnabled(false);
            ui->comboBox_serial->setEnabled(true);
            ui->pushButton_serial_connect->setEnabled(true);
            ui->statusBar->showMessage("Open:" + portInfo + "failed!" );
        } else {
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
    ui->pushButton_serial_disconnect->setEnabled(false);
    ui->comboBox_serial->setEnabled(true);
    ui->pushButton_serial_connect->setEnabled(true);
}
