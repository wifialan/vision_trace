#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort();
    socket = new QUdpSocket();
    connect_state  =  false;
    this->port = PYTHON_PORT;
    this->ip = HOST_IP;
    socket_connect();
    connect(socket,                 \
            SIGNAL( readyRead() ), \
            this,               \
            SLOT( on_read_network() )
            );
    connect(serial,     \
            SIGNAL( readyRead() ), \
            this, \
            SLOT( on_read_serial()  )
            );
}

MainWindow::~MainWindow()
{
    delete ui;
}


qint16 MainWindow::socket_connect()
{
    if ( ip.isEmpty() | port > 9999 )
        return ERROR_USER_INPUT;
    if ( socket->bind(QHostAddress(PYTHON_IP), (quint16)8399) )
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

    return pac;
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_pushButton_con_serial_clicked()
{
    QString serial_name = "/dev/ttyUSB0";
    QString portName;
    QString uartName;
    QSerialPortInfo info;

    qDebug() << "Debug: Refresh the list...";
    foreach ( info , QSerialPortInfo::availablePorts()) {
        serial->setPort(info);
        portName = info.portName();
        uartName = info.description();
        qDebug() << tr("SYSTEM: Scan the uart device: ")+uartName + "("+portName+")"+tr(" has been added to the available list! ");
    }
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setParity( QSerialPort::NoParity );
    serial->setDataBits(QSerialPort::Data8);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setPortName(serial_name);
    if (!serial->open(QIODevice::ReadWrite)) {
        QMessageBox::warning(this,"Warring","Open serial port fail!\n Please see the the information window to solve problem.");
        qDebug() << tr("SYSTEM: The serial port failed to open,Please check as follows: ");
        qDebug() << tr("        1> if the serial port is occupied by other software? ");
        qDebug() << tr("        2> if the serial port connection is normal?");
        qDebug() << tr("        3> if the program is run at root user? You can use the cmd sudo ./(programname) and type your password to be done.");
        ui->textBrowser->append("SYSTEM: The serial port failed to open,Please check as follows: ");
        ui->textBrowser->append("        1> if the serial port is occupied by other software? ");
        ui->textBrowser->append("        2> if the serial port connection is normal?");
        ui->textBrowser->append("        3> if the program is run at root user? You can use the cmd sudo ./(programname) and type your password to be done.");
        ui->pushButton_con_serial->setEnabled( true );
        ui->statusBar->showMessage("Open serial port fail!", 3000);
    } else {
        qDebug() << tr("SYSTEM: The system has been connected with ")+serial_name+" " ;
        QMessageBox::information(this,"Information", "UART: "+ serial_name+" has been connected! \n"+"Wait device signals.");
        ui->pushButton_con_serial->setEnabled( false );
        ui->statusBar->showMessage("Open serial port succuss!", 3000);
    }
}

void MainWindow::on_pushButton_discon_serial_clicked()
{
    serial->close();
    ui->pushButton_con_serial->setEnabled( true );
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
    send_cmd_serial(CMD_UP);
    ui->textBrowser->append("SYSTEM: send cmd [UP]");
}

void MainWindow::on_pushButton_down_clicked()
{
    send_cmd_serial(CMD_DOWN);
    ui->textBrowser->append("SYSTEM: send cmd [DOWN]");
}

void MainWindow::on_pushButton_left_clicked()
{
    send_cmd_serial(CMD_LEFT);
    ui->textBrowser->append("SYSTEM: send cmd [LEFT]");
}

void MainWindow::on_pushButton_right_clicked()
{
    send_cmd_serial(CMD_RIGHT);
    ui->textBrowser->append("SYSTEM: send cmd [RIGHT]");
}

void MainWindow::send_cmd_serial(quint8 cmd)
{
    QByteArray array;
    array.append(0xAA);
    array.append(0xBB);
    array.append(cmd);
    array.append(0xCC);
}
