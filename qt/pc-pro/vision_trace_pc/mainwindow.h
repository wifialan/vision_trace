#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQuickItem>
#include <QtNetwork/QUdpSocket>
#include <QTimer>
#define             HOST_IP                 (tr("192.168.1.237"))
#define             HOST_PORT               ((quint16)8877)
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
namespace Ui {
class MainWindow;
}
typedef struct {
    bool   flag;
    quint8 alter_id;
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
    void on_pushButton_con_net_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_up_clicked();

    void on_pushButton_left_clicked();

    void on_pushButton_down_clicked();

    void on_pushButton_right_clicked();

    void on_pushButtonStop_clicked();

private:
    Ui::MainWindow *ui;

    QUdpSocket  *socket;
    QString     ip;
    quint16     port;
    bool        connect_state;
    QByteArray *socket_array;
    quint64     count;
    quint16     car_state;
    quint16     camera_state;
    QTimer      *timer;
private:

    qint16      socket_connect();
    void        socket_disconnect();
    void        write_socket (quint8 byte);
    void        write_socket (QByteArray array);
    void        write_socket (QString str);
    void        write_socket (quint8 *buffer, quint32 len);
    bool        is_connect();
    void        set_camera_state(quint8);
    void        set_car_state(quint8);
    void        send_cmd_to_udp( quint8 cmd, QByteArray &value );
    void        send_cmd_to_udp( quint8 cmd );
    COM_PAC     decode_protocal( QByteArray array );

public slots:
    void        on_read_network();
    void        on_time_over();


};

#endif // MAINWINDOW_H
