#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QPainter>
#include <QNetworkInterface>
#include "ros.h"
#include "camera.h"
#include "pathplan.h"
#include "protocol.h"
#include "stm32range.h"

/*
// cmd
#define             CMD_ALTER_ID            0x01
#define             CMD_UP                  0x01
#define             CMD_DOWN                0x02
#define             CMD_LEFT                0x03
#define             CMD_RIGHT               0x04
#define             CMD_STOP                0x05
#define             CMD_QR                  0x06
#define             CMD_REQUEST             0x07
#define             CMD_ACK                 0x08

//protocal
0xAA 0xBB 0xCC 0xDD
0x(alter_id)
0x(cmd)
0x(length)
0x(payload)
0xcc
0xff
*/

#define             PC_IP               (tr("192.168.1.237"))
#define             PC_PORT             ((quint16)8799)

#define             ARM_IP                 (tr("192.168.1.237"))
#define             ARM_PORT               ((quint16)8399)
#define             SEND_RETRY              10
#define             ERROR_USER_INPUT            ((qint16)0x01)
#define             ERROR_NO_ERROR              ((qint16)0x00)
#define             ERROR_SYSTEM_NETWORK        ((qint16)0x02)

#define             CMD_ALTER_ID            0x03
#define             CMD_UP                  0x01
#define             CMD_DOWN                0x02
#define             CMD_LEFT                0x03
#define             CMD_RIGHT               0x04
#define             CMD_STOP                0x05
#define             CMD_TURN                0x09
#define             CMD_QR                  0x06
#define             CMD_REQUEST             0x07
#define             CMD_ACK                 0x08
#define             LED_STATE_ONLINE        0x00
#define             LED_STATE_OFFLINE       0x01
#define             LED_STATE_DEFAULT       0x02


namespace Ui {
class MainWindow;
}
typedef struct {
    bool   flag;
    quint8 info_type;
#define             INFO_STATUS             0x01
#define             TURLTEBOT_FORWARD                 0x01
#define             TURLTEBOT_BACKWARD                0x02
    quint8 direction;
    quint8 start_node;
    quint8 stop_node;
    quint8 current_node;

#define             INFO_PING               0x02
#define             ARM_QT_FLAG             0x66
#define             PC_QT_FLAG              0x55
#define             PING_BACK_TRUE          0x01
#define             PING_BAC_FALSE          0x02
    quint8 ping_flag;
    quint8 ping_back;

#define             INFO_COMMAND            0x03
    quint8 ctrl_command;
    quint8 move_info;
#define             INFO_PATH_NODE          0x04
    QByteArray path_node;

#define             INFO_UPDATE             0x05

#define             INFO_SPEED_VALUE        0x06
#define             LINE_SPEED              0x01
#define             ANGULAR_SPEED           0x02

    double line_speed;
    double angular_speed;

#define             INFO_LIFTER             0x07
#define             LIFTER_SET_HEIGHT       0x01
#define             LIFTER_SET_SPEED        0x02
#define             LIFTER_SET_ZERO         0x03
#define             LIFTER_STOP             0x04
#define             LIFTER_EXCUTE           0x05

    int     lifter_data_type;
    int lifter_height_hight;
    int lifter_height_low;

    double lifter_height;
    int    lifter_speed;


    quint8 cmd;
    quint8 len;
    QByteArray payload;
} COM_PAC;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    QTcpServer  *tcp_server;
    QTcpSocket  *tcp_client;
    bool        flag_is_connect_to_tcp;

private:
    Ui::MainWindow *ui;

    QUdpSocket  *socket;

    Camera      *cam;
    Ros         *ros;
    Pathplan    *path;
    Protocol    *protocol;
    STM32Range  *stm32Range;
    //    Pathplan    *path;
    QTimer      *timer_serial;
    QTimer      *timer_range;
    QSerialPort *serial;

    QString     ip;
    quint16     port;
    bool        connect_state;
    QByteArray *socket_array;
    QStringList oldPortStringList;
    QByteArray read_serial;

    bool OpenSuccess;
public:
    void update_remote_turltebot_status(qint16);

private:

    qint16      socket_connect();
    void        socket_listen();


    void        socket_disconnect();
    void        write_socket (quint8 byte);
    void        write_socket (QByteArray array);
    void        write_socket (QString str);
    void        write_socket (quint8 *buffer, quint32 len);
    bool        is_connect();
    void        send_cmd_serial  (quint8 cmd);
    void        send_cmd_to_tcp( quint8 cmd, QByteArray &value );
    void        send_cmd_to_tcp( quint8 cmd );
    void        send_cmd_to_tcp(QByteArray);
    COM_PAC     decode_protocal( QByteArray array );
    QString     get_localhost_ip();

    void        analyze_info_status(COM_PAC);
    void        analyze_info_ping(COM_PAC);
    void        analyze_info_command(COM_PAC);
    void        analyze_info_update();
    void        analyze_info_speed(COM_PAC);
    void        analyze_info_lifter(COM_PAC);
    void        send_start_stop_node_to_pc();


public slots:
    void        on_read_network();
    void        on_read_serial();
    void        on_timer_serial();
    void        on_timer_range();
    void        on_disconnected();
    void        on_new_connect_tcp();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();
    void on_pushButton_left_clicked();
    void on_pushButton_right_clicked();
private slots:
    void on_show_frame(QImage);
    void on_show_frame_2(QImage);
    void on_show_tutlebot_status(qint16);
    void on_show_command(QByteArray);
    void on_send_info_to_pc(QByteArray);
    void on_update_path_node(QByteArray);
    void on_update_path_start_node(QByteArray);
    void on_update_trultebot_direction(bool);


    void on_pushButton_stop_clicked();

    void on_pushButton_go_clicked();

    void on_pushButton_serial_connect_clicked();

    void on_pushButton_serial_disconnect_clicked();

    void on_comboBox_direction_currentIndexChanged(int index);

    void on_doubleSpinBox_line_speed_valueChanged(double arg1);

    void on_doubleSpinBox_angular_speed_valueChanged(double arg1);



    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_lifter_zero_clicked();


    void on_pushButton_lifter_set_height_clicked();

    void on_pushButton_lifter_stop_clicked();

    void on_pushButton_lifter_set_speed_clicked();

    void on_pushButton_lifter_excute_clicked();

signals:
    void lanuch_turltebot_go();


};

#endif // MAINWINDOW_H
