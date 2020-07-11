#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcp_client(new QTcpSocket)
{
    ui->setupUi(this);
    this->port = PC_PORT;
    this->ip = get_localhost_ip();
    emit change_indicator_state(true);
    count = 0;

    timer_check_tcp_online = new QTimer();
    timer_check_tcp_online->setInterval(500);

    connect(timer_check_tcp_online, SIGNAL(timeout()), this, SLOT(on_timer_check_tcp_online()));

    tcp_client->abort();//取消原有连接
    tcp_socket_bind();

    connect(tcp_client,                 \
            SIGNAL( readyRead() ), \
            this,               \
            SLOT( on_read_network() )
            );

    ui->lineEdit_server_ip->setText("192.168.1.119");
    ui->lineEdit_server_port->setText("8989");

    ui->pushButton_connect_server_ip->setEnabled(true);
    ui->pushButton_disconnect_server_ip->setEnabled(false);

    ui->comboBox_direction->addItem("正向");
    ui->comboBox_direction->addItem("反向");

    ui->doubleSpinBox_line_speed->setRange(0.01,0.5);
    ui->doubleSpinBox_line_speed->setSingleStep(0.01);
    ui->doubleSpinBox_line_speed->setDecimals(2);
    ui->doubleSpinBox_line_speed->setValue(0.15);
    ui->doubleSpinBox_angular_speed->setRange(0.01,0.5);
    ui->doubleSpinBox_angular_speed->setSingleStep(0.05);
    ui->doubleSpinBox_angular_speed->setDecimals(2);
    ui->doubleSpinBox_angular_speed->setValue(0.02);

    ui->doubleSpinBox_lifter_height->setDecimals(2);
    ui->doubleSpinBox_lifter_height->setRange(2,200);
    ui->doubleSpinBox_lifter_height->setValue(10);
    ui->doubleSpinBox_lifter_height->setSingleStep(0.01);

    ui->spinBox_lifter_speed->setRange(1,100);
    ui->spinBox_lifter_speed->setValue(100);
    ui->spinBox_lifter_speed->setSingleStep(1);

    //    ui->pushButton_lifter_excute->setEnabled(false);
    //    ui->pushButton_lifter_set_height->setEnabled(false);
    //    ui->pushButton_lifter_set_speed->setEnabled(false);
    //    ui->pushButton_lifter_stop->setEnabled(false);
    //    ui->pushButton_lifter_zero->setEnabled(false);
    //    ui->doubleSpinBox_lifter_height->setEnabled(false);
    //    ui->spinBox_lifter_speed->setEnabled(false);

    info_6_receive_from_remote = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::get_localhost_ip()
{
    QString ip = "";
    QProcess cmd_pro ;
    QString cmd_str = QString("ipconfig");
    cmd_pro.start("cmd.exe", QStringList() << "/c" << cmd_str);
    cmd_pro.waitForStarted();
    cmd_pro.waitForFinished();
    QString result = cmd_pro.readAll();
    QString pattern("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");
    QRegExp rx(pattern);

    int pos = 0;
    bool found = false;
    while((pos = rx.indexIn(result, pos)) != -1){
        QString tmp = rx.cap(0);
        //qDebug() << tmp << "-*-*" << ip;
        //跳过子网掩码 eg:255.255.255.0
        if(-1 == tmp.indexOf("255")){
            //qDebug() << ip.lastIndexOf(".") << "--" << ip.mid(0,ip.lastIndexOf(".")) << "**" << tmp.indexOf(ip.mid(0,ip.lastIndexOf(".")));
            if(ip != "" && -1 != tmp.indexOf(ip.mid(0,ip.lastIndexOf(".")))){
                found = true;
                break;
            }
            ip = tmp;
        }
        pos += rx.matchedLength();
    }
    qDebug()<<"local ip: " << ip;

    return ip;
}

void MainWindow::get_lan_ip()
{
    QString ip = "";
    QProcess cmd_pro ;
    QString cmd_str = QString("ipconfig");
    cmd_pro.start("cmd.exe", QStringList() << "/c" << cmd_str);
    cmd_pro.waitForStarted();
    cmd_pro.waitForFinished();
    QString result = cmd_pro.readAll();
    QString pattern("[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}");
    QRegExp rx(pattern);

    int pos = 0;
    bool found = false;
    while((pos = rx.indexIn(result, pos)) != -1){
        QString tmp = rx.cap(0);
        //qDebug() << tmp << "-*-*" << ip;
        //跳过子网掩码 eg:255.255.255.0
        if(-1 == tmp.indexOf("255")){
            //qDebug() << ip.lastIndexOf(".") << "--" << ip.mid(0,ip.lastIndexOf(".")) << "**" << tmp.indexOf(ip.mid(0,ip.lastIndexOf(".")));
            if(ip != "" && -1 != tmp.indexOf(ip.mid(0,ip.lastIndexOf(".")))){
                found = true;
                break;
            }
            ip = tmp;
        }
        pos += rx.matchedLength();
    }

    qDebug()<<"local ip: " << ip;

    QString gateway;
    QString fixed_ip1;
    QString fixed_ip255;

    for (int i = 0;i<ip.lastIndexOf(".");i++) {
        gateway.append(ip[i]);
    }
    qDebug() << gateway;
    fixed_ip1.append(gateway);
    fixed_ip1.append(".1");
    fixed_ip255.append(gateway);
    fixed_ip255.append(".255");


    cmd_pro.start("cmd.exe", QStringList() << "/c" << "arp -a");
    cmd_pro.waitForStarted();
    cmd_pro.waitForFinished();
    QString result_arp = cmd_pro.readAll();
    pos = 0;
    qDebug() << "---------";
    while((pos = rx.indexIn(result_arp, pos)) != -1){
        QString tmp = rx.cap(0);
        qDebug() << tmp;
        //跳过子网掩码 eg:255.255.255.0
        if( tmp != ip && tmp.contains(gateway) && tmp != fixed_ip1 && tmp != fixed_ip255)
        {
            ui->lineEdit_server_ip->setText(tmp);
            qDebug() << tmp;
        }
        pos += rx.matchedLength();
    }
}

void MainWindow::on_timer_check_tcp_online()
{
    if (tcp_client->write("detect tcp state") == -1)
    {
        // 返回-1说明连接失败
        ui->lineEdit_server_ip->setEnabled(true);
        ui->lineEdit_server_port->setEnabled(true);
        ui->pushButton_connect_server_ip->setEnabled(true);
        ui->pushButton_disconnect_server_ip->setEnabled(false);
    }
}

qint16 MainWindow::tcp_socket_bind()
{
    qDebug() << "tcp_socket_bind";
    if ( this->ip.isEmpty() || this->port > 9999 ){
        qDebug() << "IP 和 PORT 有误";
        return ERROR_USER_INPUT;
    }
    //防止PC端有代理影响连接
    QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);
    if ( tcp_client->bind(QHostAddress(this->ip), this->port) ){
        qDebug() << "成功绑定端口:" << this->port;
        connect_state = true;
        return ERROR_NO_ERROR;
    }
    else {
        qDebug() << "绑定端口失败";
        qDebug() << "IP: " << this->ip << " PORT: " << this->port;
        connect_state = false;
        return ERROR_SYSTEM_NETWORK;
    }
    return 0;
}

void MainWindow::tcp_socket_disconnect()
{
    connect_state = false;
    tcp_client->close();
    tcp_client->abort();

}

void MainWindow::write_socket(QByteArray array)
{
    qDebug() << array;
    if(tcp_client->isOpen())
    {
        tcp_client->write(array);
    }
}


void MainWindow::on_read_network()
{
    QByteArray array;

    array.append( tcp_client->readAll() );
    qDebug() << "read: " << array;
    COM_PAC pac = decode_protocal( array );
    if ( pac.flag != true )
        return;

    qDebug() <<  "info type :" << pac.info_type;

    switch (pac.info_type) {
    case INFO_STATUS:
        analyze_info_status(pac);
        break;

    case INFO_PATH_NODE:
        analyze_info_path_node(pac.path_node);
        break;
    case INFO_COMMAND:
        analyze_info_command(pac);
    default:
        break;
    }

}

void MainWindow::analyze_info_command(COM_PAC pac) {

    switch (pac.ctrl_command) {
    case TURLTEBOT_UP:
        //ui->textBrowser->append("SYSTEM: send cmd [UP]");
        break;
    case TURLTEBOT_LEFT:
        //ui->textBrowser->append("SYSTEM: send cmd [LEFT]");
        break;
    case TURLTEBOT_RIGHT:
        //ui->textBrowser->append("SYSTEM: send cmd [RIGHT]");
        break;
    case TURLTEBOT_STOP:
        //ui->textBrowser->append("SYSTEM: send cmd [STOP]");
        break;
    case TURLTEBOT_DOWN:
        //ui->textBrowser->append("SYSTEM: send cmd [DOWN]");
        break;
    default:
        break;
    }
}


void MainWindow::analyze_info_status(COM_PAC pac)
{
    // direction
    ui->comboBox_direction->setCurrentText( (pac.direction == FORWARD ? "正向" : "反向") );
    // start node
    if (pac.start_node != 0xFF) {
        ui->comboBox_start_node->setCurrentText(QString::number(pac.start_node,10));
    }
    // stop node
    if (pac.stop_node != 0xFF) {
        ui->comboBox_stop_node->setCurrentText(QString::number(pac.stop_node,10));
    }
    // current node
    if (pac.current_node == 0xFF) {
        //        //ui->lineEdit_current_node->setText("");
    } else {
        //ui->lineEdit_current_node->setText(QString::number(pac.current_node,10));
    }
}

void MainWindow::analyze_info_path_node(QByteArray info_4)
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
    ui->comboBox_stop_node->setCurrentIndex(1);
    qDebug() << "Add success";
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
    QByteArray request_array_to_arm;
    QByteArray request_array;
    request_array_to_arm.append(0x02);
    request_array_to_arm.append(0x55);

    request_array.append(0xAA);
    request_array.append(0xBB);
    request_array.append(0xCC);
    request_array.append(0xDD);                 // header
    request_array.append(request_array_to_arm); // info
    request_array.append(0xCC);
    request_array.append(0xFF);                 // end
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
    qDebug() << "request_array" << request_array;
}

COM_PAC MainWindow::decode_protocal(QByteArray array)
{
    quint16 cmd_count;
    quint16 header_index;
    quint16 tail_index;
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
    tail_index = array.indexOf(tail_array);
    pac.info_type =  array.at( header_index + 1 );
    switch (pac.info_type) {
    case INFO_STATUS:
        pac.direction = array.at( header_index + 2 );
        pac.start_node = array.at( header_index + 3 );
        pac.stop_node = array.at( header_index + 4 );
        pac.current_node = array.at( header_index + 5 );
        pac.flag = true;
        break;
    case INFO_PING:
        pac.ping_flag = array.at( header_index + 2 );
        pac.ping_back = array.at( header_index + 3 );
        pac.flag = true;
        break;
    case INFO_PATH_NODE:
        pac.path_node = array.mid( header_index + 1, tail_index - header_index - 1);
        pac.flag = true;
        break;
    case INFO_COMMAND:
        pac.ctrl_command = array.at( header_index + 2 );
        pac.flag = true;
        break;
    case INFO_SPEED_VALUE:

        pac.line_speed = array.at(header_index + 2) & 0xFF;
        pac.angular_speed = array.at(header_index + 3)& 0xFF;
        pac.line_speed /= 100.0;
        pac.angular_speed /= 100.0;
        //        value = array.split(',').at(1).toHex();
        //        str = array.split(',').at(1);
        qDebug("line speed: %.2f\r\n",pac.line_speed);
        qDebug("angular speed: %.2f\r\n",pac.angular_speed);

        info_6_receive_from_remote = true;
        ui->doubleSpinBox_line_speed->setValue(pac.line_speed);
        ui->doubleSpinBox_angular_speed->setValue(pac.angular_speed);

        break;
    case INFO_LIFTER:

        pac.lifter_data_type = array.at(header_index + 2);

        if(pac.lifter_data_type == LIFTER_SET_HEIGHT){
            info_7_receive_from_remote = true;
            pac.lifter_height_hight = array.at(header_index + 3);
            pac.lifter_height_low = (array.at(header_index + 4)&0xFF);
            //            qDebug() << "pac.lifter_height_hight" << pac.lifter_height_hight;
            //            qDebug() << "pac.lifter_height_low" << pac.lifter_height_low;

            pac.lifter_height = ((pac.lifter_height_hight << 8) & 0xFF00) | pac.lifter_height_low;
            pac.lifter_height = pac.lifter_height / 100.0;
            ui->doubleSpinBox_lifter_height->setValue(pac.lifter_height);

            qDebug("pac.lifter_heiht %.2f",pac.lifter_height);
        } else if(pac.lifter_data_type == LIFTER_SET_SPEED){
            info_7_receive_from_remote = true;
            pac.lifter_speed = array.at(header_index + 3)&0xFF;
            ui->spinBox_lifter_speed->setValue(pac.lifter_speed);

            qDebug() << "pac.lifter_speed" << pac.lifter_speed;
        } else if(pac.lifter_data_type == LIFTER_SET_ZERO){
            // zero

        }else if(pac.lifter_data_type == LIFTER_STOP){

            // stop
        }else if(pac.lifter_data_type == LIFTER_EXCUTE){

            // LIFTER_EXCUTE
        }

        qDebug() << "INFO_LIFTER";

        break;
    default:
        break;
    }

    return pac;
}


void MainWindow::on_pushButton_up_clicked()
{
    QByteArray info_3;
    info_3.append(0x03);
    info_3.append(CMD_UP);
    send_cmd_to_tcp(info_3);
    //ui->textBrowser->append("SYSTEM: send cmd [UP]");
}

void MainWindow::on_pushButton_left_clicked()
{
    QByteArray info_3;
    info_3.append(0x03);
    info_3.append(CMD_LEFT);
    send_cmd_to_tcp(info_3);
    //ui->textBrowser->append("SYSTEM: send cmd [LEFT]");
}

void MainWindow::on_pushButton_down_clicked()
{
    QByteArray info_3;
    info_3.append(0x03);
    info_3.append(CMD_DOWN);
    send_cmd_to_tcp(info_3);
    //ui->textBrowser->append("SYSTEM: send cmd [DOWN]");
}

void MainWindow::on_pushButton_right_clicked()
{
    QByteArray info_3;
    info_3.append(0x03);
    info_3.append(CMD_RIGHT);
    send_cmd_to_tcp(info_3);
    //ui->textBrowser->append("SYSTEM: send cmd [RIGHT]");
}

void MainWindow::on_pushButtonStop_clicked()
{
    QByteArray info_3;
    info_3.append(0x03);
    info_3.append(CMD_STOP);
    send_cmd_to_tcp(info_3);
    //ui->textBrowser->append("SYSTEM: send cmd [STOP]");
}

void MainWindow::on_pushButton_discon_net_clicked()
{

}

void MainWindow::on_pushButton_go_clicked()
{
    QByteArray info_1;
    // 信息类型
    info_1.append(0x01);
    // 小车朝向
    if(ui->comboBox_direction->currentText() == "正向"){
        info_1.append(FORWARD);
    }else {
        info_1.append(BACKWARD);
    }
    // 起始节点
    if (ui->comboBox_start_node->currentText().isEmpty()) {
        update_info();
        QThread::msleep(200);
    }
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

void MainWindow::update_info()
{
    QByteArray info_5;
    info_5.append(0x05);
    send_cmd_to_tcp(info_5);
}

void MainWindow::on_doubleSpinBox_line_speed_valueChanged(double arg1)
{
    if(info_6_receive_from_remote == true)
    {
        info_6_receive_from_remote = false;
        return;
    }

    QByteArray info_6;
    info_6.append(0x06);
    //info_6.append(LINE_SPEED);
    info_6.append((arg1*100));
    info_6.append(ui->doubleSpinBox_angular_speed->value()*100);
    send_cmd_to_tcp(info_6);
}

void MainWindow::on_doubleSpinBox_angular_speed_valueChanged(double arg1)
{
    if(info_6_receive_from_remote == true)
    {
        info_6_receive_from_remote = false;
        return;
    }
    QByteArray info_6;
    info_6.append(0x06);
    info_6.append(ui->doubleSpinBox_line_speed->value()*100);
    info_6.append((arg1*100));
    send_cmd_to_tcp(info_6);
}

void MainWindow::on_pushButton_lifter_set_height_clicked()
{
    if(info_7_receive_from_remote == true)
    {
        info_7_receive_from_remote = false;
        return;
    }
    qDebug() << "ui->doubleSpinBox_lifter_height->value()" << ui->doubleSpinBox_lifter_height->value();
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

void MainWindow::on_pushButton_lifter_set_speed_clicked()
{
    int speed = ui->spinBox_lifter_speed->value();
    qDebug() << speed;
    QByteArray info_7;
    info_7.append(0x07);
    info_7.append(LIFTER_SET_SPEED);
    info_7.append(speed);
    send_cmd_to_tcp(info_7);
}

void MainWindow::on_pushButton_lifter_zero_clicked()
{
    QByteArray info_7;
    info_7.append(0x07);
    info_7.append(LIFTER_SET_ZERO);
    send_cmd_to_tcp(info_7);
}

void MainWindow::on_pushButton_lifter_stop_clicked()
{
    QByteArray info_7;
    info_7.append(0x07);
    info_7.append(LIFTER_STOP);
    send_cmd_to_tcp(info_7);
}

void MainWindow::on_pushButton_lifter_excute_clicked()
{
    QByteArray info_7;
    info_7.append(0x07);
    info_7.append(LIFTER_EXCUTE);
    send_cmd_to_tcp(info_7);
}

void MainWindow::on_pushButton_connect_server_ip_clicked()
{
    quint16 remote_port;
    QString remote_ip;
    remote_ip = ui->lineEdit_server_ip->text();
    remote_port = quint16(ui->lineEdit_server_port->text().toUInt());
    tcp_client->abort();
    tcp_client->connectToHost(remote_ip,remote_port, QIODevice::ReadWrite);

    qDebug() << "远程IP:" << remote_ip;
    qDebug() << "远程端口:" << remote_port;

    //QObject::connect((QObject*) socket,SIGNAL(readyRead()),(QObject*)this,SLOT(on_read_network()));
    if( !tcp_client->waitForConnected(500) ) {
        //1//xqDebug("netclientread@set_connect() >: socket Connection failed!!");
        qDebug() << "connect failed!";
        ui->pushButton_connect_server_ip->setEnabled(true);
        ui->pushButton_disconnect_server_ip->setEnabled(false);
        ui->lineEdit_server_ip->setEnabled(true);
        ui->lineEdit_server_port->setEnabled(true);
        timer_check_tcp_online->stop();
    }else {
        //1//xqDebug("netclientread@set_connect() >: socket conncetion succussful.");
        qDebug() << "connect success!";
        ui->pushButton_connect_server_ip->setEnabled(false);
        ui->pushButton_disconnect_server_ip->setEnabled(true);
        ui->lineEdit_server_ip->setEnabled(false);
        ui->lineEdit_server_port->setEnabled(false);
        timer_check_tcp_online->start();
    }
}

void MainWindow::on_pushButton_disconnect_server_ip_clicked()
{
    tcp_client->disconnectFromHost();
    ui->pushButton_connect_server_ip->setEnabled(true);
    ui->pushButton_disconnect_server_ip->setEnabled(false);
    ui->lineEdit_server_ip->setEnabled(true);
    ui->lineEdit_server_port->setEnabled(true);
    timer_check_tcp_online->stop();
}

