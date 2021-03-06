#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQuickItem>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkProxy>

#include <QTimer>
#include <QNetworkInterface>
#include <QThread>
#include <QProcess>

#define             PC_IP                 (tr("192.168.1.237"))
#define             PC_PORT               8788
#define             ARM_IP                  (tr("192.168.1.237"))
#define             ARM_PORT                ((quint16)8399)
#define             SEND_RETRY              10
#define             ERROR_USER_INPUT            ((qint16)0x01)
#define             ERROR_NO_ERROR              ((qint16)0x00)
#define             ERROR_SYSTEM_NETWORK        ((qint16)0x02)
#define             CMD_ALTER_ID            0x01
#define             CMD_UP                  0x01
#define             CMD_DOWN                0x02
#define             CMD_LEFT                0x03
#define             CMD_RIGHT               0x04
#define             CMD_STOP                0x05
#define             CMD_QR                  0x06
#define             CMD_REQUEST             0x07
#define             CMD_ACK                 0x08
#define             LED_STATE_ONLINE        0x00
#define             LED_STATE_OFFLINE       0x01
#define             LED_STATE_DEFAULT       0x02

#define             TURLTEBOT_UP            0x01
#define             TURLTEBOT_DOWN          0x02
#define             TURLTEBOT_RIGHT         0x03
#define             TURLTEBOT_LEFT          0x04
#define             TURLTEBOT_TURN          0x05
#define             TURLTEBOT_STOP          0x06

/*
 * 与ARM端QT间的通信协议
 *
 */

namespace Ui {
class MainWindow;
}
typedef struct {
    bool   flag;
    quint8 info_type;
#define             INFO_STATUS             0x01
#define             FORWARD                 0x01
#define             BACKWARD                0x02
    quint8 direction;
    quint8 start_node;
    quint8 stop_node;
    quint8 current_node;
    quint8 move_info;
#define             INFO_PING               0x02
#define             ARM_QT_FLAG             0x66
#define             PC_QT_FLAG              0x55
#define             PING_BACK_TRUE          0x01
#define             PING_BAC_FALSE          0x02
    quint8 ping_flag;
    quint8 ping_back;
#define             INFO_COMMAND            0x03
    quint8 ctrl_command;
#define             INFO_PATH_NODE          0x04
    QByteArray path_node;

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

signals:
    void change_indicator_state(bool);
private slots:

    void on_pushButton_up_clicked();

    void on_pushButton_left_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_right_clicked();

    void on_pushButtonStop_clicked();

    void on_pushButton_discon_net_clicked();

    void on_pushButton_go_clicked();

    void on_doubleSpinBox_line_speed_valueChanged(double arg1);

    void on_doubleSpinBox_angular_speed_valueChanged(double arg1);

    void on_pushButton_lifter_set_height_clicked();

    void on_pushButton_lifter_set_speed_clicked();

    void on_pushButton_lifter_zero_clicked();

    void on_pushButton_lifter_stop_clicked();

    void on_pushButton_lifter_excute_clicked();

    void on_pushButton_connect_server_ip_clicked();

    void on_pushButton_disconnect_server_ip_clicked();


private:
    Ui::MainWindow *ui;

    QTimer      *timer_check_tcp_online;
    QTcpSocket  *tcp_client;
    QString     ip;
    quint16     port;
    bool        connect_state;
    QByteArray *socket_array;
    quint64     count;
    QTimer      *timer;
    bool        info_6_receive_from_remote;
    bool        info_7_receive_from_remote;
private:

    qint16      tcp_socket_bind();
    void        tcp_socket_disconnect();
    void        write_socket (quint8 byte);
    void        write_socket (QByteArray array);
    void        write_socket (QString str);
    void        write_socket (quint8 *buffer, quint32 len);
    bool        is_connect();
    void        send_cmd_to_tcp( quint8 cmd, QByteArray &value );
    void        send_cmd_to_tcp( quint8 cmd );
    void        send_cmd_to_tcp( QByteArray );
    QString     get_localhost_ip();
    void        get_lan_ip();

    void        update_info();

    COM_PAC     decode_protocal( QByteArray array );

    void        analyze_info_path_node(QByteArray);
    void        analyze_info_status(COM_PAC);
//    void        analyze_info_ping(COM_PAC);
    void        analyze_info_command(COM_PAC);

    void        connect_status();
    void        disconnect_status();


public slots:
    void        on_read_network();
    void        on_timer_check_tcp_online();


};

#endif // MAINWINDOW_H
