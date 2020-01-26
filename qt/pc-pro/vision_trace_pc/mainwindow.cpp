#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    socket(new QUdpSocket),
    timer(new QTimer),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->port = HOST_PORT;
    this->ip = HOST_IP;
    emit change_indicator_state(true);
    count = 0;
    car_state = LED_STATE_DEFAULT;
    camera_state = LED_STATE_DEFAULT;
    set_camera_state(camera_state);
    set_car_state(car_state);
    socket_connect();
    connect( this->timer,
             SIGNAL(timeout()),
             this,
             SLOT(on_time_over()));
    connect(socket,                 \
            SIGNAL( readyRead() ), \
            this,               \
            SLOT( on_read_network() )
            );
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_time_over()
{
    set_camera_state( camera_state );
    set_car_state( car_state );
    timer->stop();
}

qint16 MainWindow::socket_connect()
{
    if ( ip.isEmpty() | port > 9999 )
        return ERROR_USER_INPUT;
    if ( socket->bind(QHostAddress("192.168.1.216"), (quint16)8499) )
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

    len = socket->writeDatagram( array, QHostAddress(ip), this->port );
    if (len != array.length() ) {
        qDebug() << "udp retransmit!";
    }

}

void MainWindow::write_socket(QString str)
{
    quint64 len;
    QByteArray array;
    array.append(str);
    len = socket->writeDatagram( array, QHostAddress(ip), this->port );
    if (len != array.length() ) {
        qDebug() << "udp retransmit!";
    }
}

void MainWindow::write_socket(quint8 *buffer, quint32 len)
{
    quint64 sendLen;
    QByteArray array;
    array.append((char*)buffer , len );
    sendLen = socket->writeDatagram((char*)buffer, len, QHostAddress(HOST_IP), HOST_PORT);
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
    if ( pac.flag != true || pac.alter_id == CMD_ALTER_ID)
        return;

    qDebug() <<  "recv pac:";
    qDebug() <<  "cmd:" << pac.cmd;
    qDebug() <<  "id:" << pac.alter_id;
    qDebug() <<  "len:" << pac.len;
    switch (pac.cmd) {
    case CMD_ACK:
        if (pac.alter_id == 0x02) {
            camera_state = LED_STATE_ONLINE;
        } else if (pac.alter_id == 0x03) {
            car_state = LED_STATE_ONLINE;
        }
        break;

    case CMD_DOWN:
    case CMD_LEFT:
    case CMD_UP:
    case CMD_RIGHT:
        break;
    default:
        break;
    }

}


void MainWindow::on_pushButton_con_net_clicked()
{
    QByteArray array;
    ui->statusBar->showMessage("Get devices status from network...wait...", 3000);
    car_state = LED_STATE_OFFLINE;
    camera_state = LED_STATE_OFFLINE;
    send_cmd_to_udp(CMD_REQUEST);
    timer->start(2000);
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
    request_array.append(0x01);     // id
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
    if ( !(array.contains( header_array ) && array.contains( tail_array )
         && array.indexOf( header_array ) < array.indexOf( tail_array ))
         ) {
        qDebug() << "cmd array illegel: ";
        qDebug() << " ||||||  " << array << " ||||||";
        pac.flag = false;
        return pac;
    }
    header_index = array.indexOf( header_array ) + 3;
    pac.flag = true;
    pac.alter_id =  array.at( header_index + 1 );
    pac.cmd =  array.at( header_index + 2 );
    pac.len =  array.at( header_index + 3 );
    pac.payload = array.mid( header_index + 4, pac.len );
    return pac;
}

void MainWindow::on_pushButton_send_clicked()
{
    this->write_socket( ui->textBrowser->toPlainText() );
    ui->statusBar->showMessage( "send udp: " + ui->textBrowser->toPlainText(), 3000 );
}

void MainWindow::set_camera_state(quint8 state)
{
    if (state == LED_STATE_DEFAULT) {
        ui->radioButtonCamera->setStyleSheet("background-color: rgb(85, 87, 83);");
    }else if ( state == LED_STATE_ONLINE ) {
        ui->radioButtonCamera->setStyleSheet("background-color: rgb(138, 226, 52);");
    }else {
        ui->radioButtonCamera->setStyleSheet("background-color: rgb(204, 0, 0);");
    }
}
void MainWindow::set_car_state(quint8 state)
{
    if (state == LED_STATE_DEFAULT) {
        ui->radioButtonCar->setStyleSheet("background-color: rgb(85, 87, 83);");
    }else if ( state == LED_STATE_ONLINE ) {
        ui->radioButtonCar->setStyleSheet("background-color: rgb(138, 226, 52);");
    }else {
        ui->radioButtonCar->setStyleSheet("background-color: rgb(204, 0, 0);");
    }
}

void MainWindow::on_pushButton_up_clicked()
{
    send_cmd_to_udp(CMD_UP);
    ui->textBrowser->append("SYSTEM: send cmd [UP]");
}

void MainWindow::on_pushButton_left_clicked()
{
    send_cmd_to_udp(CMD_LEFT);
    ui->textBrowser->append("SYSTEM: send cmd [LEFT]");
}

void MainWindow::on_pushButton_down_clicked()
{
    send_cmd_to_udp(CMD_DOWN);
    ui->textBrowser->append("SYSTEM: send cmd [DOWN]");
}

void MainWindow::on_pushButton_right_clicked()
{
    send_cmd_to_udp(CMD_RIGHT);
    ui->textBrowser->append("SYSTEM: send cmd [RIGHT]");
}

void MainWindow::on_pushButtonStop_clicked()
{
    send_cmd_to_udp(CMD_STOP);
    ui->textBrowser->append("SYSTEM: send cmd [STOP]");
}
