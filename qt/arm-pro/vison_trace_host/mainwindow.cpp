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
    socket = new QUdpSocket();
    connect_state  =  false;
    this->port = PYTHON_PORT;
    this->ip = HOST_IP;

    ui->lineEdit->setText("1");
    ui->lineEdit_2->setText("3");
    ros->pub_cmd_vel_topic =  "/cmd_vel_mux/input/navi";
    ros->pub_cmd_vel = ros->n.advertise<geometry_msgs::Twist>(ros->pub_cmd_vel_topic, 1);
    //    ros->loopRate(5);
    ros->speed_x = 0.1;
    ros->flag = 1;
    ros->move_mode = 0;
    connect( (QObject*)this->path, SIGNAL(read_path_plan()), (QObject*)this->cam, SLOT(on_read_path_plan()));
    connect( (QObject*)this->cam, SIGNAL(show_tutlebot_status(qint16)), this, SLOT(on_show_tutlebot_status(qint16)));
    connect( (QObject*)this->cam, SIGNAL(show_frame(QImage)), this, SLOT(on_show_frame(QImage)));
    connect( (QObject*)this->cam, SIGNAL(show_frame_2(QImage)), this, SLOT(on_show_frame_2(QImage)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_up(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_up(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_down(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_down(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_right(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_right(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_left(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_left(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_turn(double, double) ), (QObject*)this->ros, SLOT(on_turltebot_turn(double, double)));
    connect( (QObject*)this->cam, SIGNAL(turltebot_stop() ), (QObject*)this->ros, SLOT(on_turltebot_stop()));
    connect( (QObject*)this->cam, SIGNAL(show_command(QByteArray)), this, SLOT(on_show_command(QByteArray)));

    socket_connect();
    connect(socket,                 \
            SIGNAL( readyRead() ), \
            this,               \
            SLOT( on_read_network() )
            );

//    cam->open();


}

MainWindow::~MainWindow()
{
    delete ui;
}


qint16 MainWindow::socket_connect()
{
    if ( ip.isEmpty() | port > 9999 )
        return ERROR_USER_INPUT;
    if ( socket->bind(QHostAddress(HOST_IP), (quint16)HOST_PORT) )
        return ERROR_NO_ERROR;
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

void MainWindow::write_socket(QByteArray array)
{
    quint64 len;
    len = socket->writeDatagram( array, QHostAddress(PYTHON_IP), this->port );
    if (len != array.length() ) {
        qDebug() << "udp retransmit!";
    }
}

void MainWindow::write_socket(quint8 *buffer, quint32 len)
{
    quint64 sendLen;
    QByteArray array;
    array.append((char*)buffer , len );
    sendLen = socket->writeDatagram((char*)buffer, len, QHostAddress(PYTHON_IP), this->port);
}

void MainWindow::on_read_network()
{
    QByteArray array;
    QHostAddress address;
    quint16 xport;

    xport = HOST_PORT;
    address.setAddress(ip);
    array.resize( socket->bytesAvailable() );
    socket->readDatagram( array.data(),array.size());
    qDebug() << "read : " << array;
    COM_PAC pac = decode_protocal( array );
    if ( pac.flag != true || pac.alter_id == CMD_ALTER_ID)
        return;

    qDebug() <<  "recv pac:";
    qDebug() <<  "cmd:" << pac.cmd;
    qDebug() <<  "id:" << pac.alter_id;
    qDebug() <<  "len:" << pac.len;
    qDebug() <<  "payload" << QString(pac.payload.toHex());
    switch (pac.cmd) {
    case CMD_ACK:

        break;

    case CMD_DOWN:
        ui->textBrowser->append("SYSTEM: send cmd [DOWN]");
        break;
    case CMD_LEFT:
        ui->textBrowser->append("SYSTEM: send cmd [LEFT]");
        break;
    case CMD_UP:
        ui->textBrowser->append("SYSTEM: send cmd [UP]");
        break;
    case CMD_RIGHT:
        ui->textBrowser->append("SYSTEM: send cmd [RIGHT]");
        break;
    case CMD_STOP:
        ui->textBrowser->append("SYSTEM: send cmd [STOP]");
        break;
    case CMD_TURN:
        ui->textBrowser->append("SYSTEM: send cmd [TURN]");
        break;
    case CMD_QR:
        ui->textBrowser->append("SYSTEM: Current QR code is: " + QString(pac.payload.toHex()));
        break;

    case CMD_REQUEST:
        qDebug() << "write ack...";
        send_cmd_to_udp(CMD_ACK);
        break;
    default:
        break;
    }
}

void MainWindow::on_read_serial()
{
    QByteArray array;

    array = serial->readAll();
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

COM_PAC MainWindow::decode_protocal(QByteArray array)
{
    quint16 cmd_count;
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
    qDebug() << header_array;
    if ( !(array.contains( header_array ) && array.contains( tail_array )
           && array.indexOf( header_array ) < array.indexOf( tail_array ))
         ) {
        qDebug() << "cmd array illegel: ";
        qDebug() << " ||||||  " << array << " ||||||";
        pac.flag = false;
        return pac;
    }

    header_index = array.indexOf( header_array ) + 3 ;
    qDebug() << header_index;
    pac.flag = true;
    pac.alter_id =  array.at( header_index + 1 );
    pac.cmd =  array.at( header_index + 2 );
    pac.len =  array.at( header_index + 3 );
    pac.payload = array.mid( header_index + 4, pac.len );
    qDebug() << "pay: " << pac.payload;

    return pac;
}

void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_pushButton_con_net_clicked()
{
    this->socket_connect();
}

void MainWindow::on_pushButton_discon_net_clicked()
{
    this->socket_disconnect();
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
    send_cmd_serial(CMD_RIGHT);
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

    ui->textBrowser->append("SYSTEM: send cmd [STOP]");
    ros->terminate();
    while(!ros->wait());
    ros->speed.linear.x = 0; //
    ros->speed.angular.z = 0; //
    ros->pub_cmd_vel.publish(ros->speed); //

    QFile file("node.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream.seek(file.size());
    QString start_point = ui->lineEdit->text();
    stream << start_point << "\n";
    QString stop_point = ui->lineEdit_2->text();
    stream << stop_point;
    file.flush();
    file.close();
    path->path_plan();



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
    ui->lineEdit_command->setText(QString(path_plan_array));
}
