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

class Pathplan : public QThread
{
    Q_OBJECT
public:
    Pathplan();


protected:
    void run();

public:
    void path_plan();

private:
    qint8 flag;
    QProcess *proc;
//    QFile *file;

signals:
    void read_path_plan();




};

#endif // PATHPLAN_H
