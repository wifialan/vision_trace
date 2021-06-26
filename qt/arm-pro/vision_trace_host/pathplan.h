#ifndef PATHPLAN_H
#define PATHPLAN_H
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <Python.h>
#include <QFile>
#include <QProcess>
#include <camera.h>

#define REALEASE 1

class Pathplan : public QThread
{
    Q_OBJECT
public:
    Pathplan();


protected:
    void run();

public:
    void path_plan();
    void read_json_file();

public:
    QProcess *proc;


private:
    qint8 flag;
//    QFile *file;

signals:
    void read_path_plan();
    void send_path_info_to_camera(QByteArray);
    void send_road_boundary(quint16, quint16);




};

#endif // PATHPLAN_H
