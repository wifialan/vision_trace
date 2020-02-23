#include "pathplan.h"


Pathplan::Pathplan()
{
    flag = 0;

    qDebug() << "Initializing Python...";
    proc = new QProcess();

    proc->setEnvironment(proc->environment());


}


void Pathplan::run()
{

}


void Pathplan::path_plan()
{

    proc->execute("python3 ../vison_trace_host/path.py");
    emit read_path_plan();

}
