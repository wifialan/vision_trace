#ifndef PATHPLAN_H
#define PATHPLAN_H
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <opencv2/core/core.hpp>

class Pathplan
{

public:
    Pathplan();
    void on_path_plan();


};

#endif // PATHPLAN_H
