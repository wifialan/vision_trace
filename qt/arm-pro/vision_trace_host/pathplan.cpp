#include "pathplan.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>

Pathplan::Pathplan()
{
    flag = 0;

    qDebug() << "Initializing Python...";
    proc = new QProcess();
    proc->setEnvironment(proc->environment());

}

void Pathplan::read_json_file(){

#if REALEASE
    QFile file("/home/igosens/huawei/path_node.json");
#else
    QFile file("../vision_trace_host/path_node.json");
#endif
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "open json file error";
    } else {
        qDebug() << "open json file success";
    }
    // read json data
    QByteArray jsonData = file.readAll();
    file.close();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!";
        return;
    }

    QByteArray path_node_origin;
    QJsonObject rootObj = jsonDoc.object();
    QStringList keys = rootObj.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        qDebug() << "key" << i << " is:" << keys.at(i);
        if(rootObj.contains(keys.at(i)))
        {
            QJsonArray subArray = rootObj.value(keys.at(i)).toArray();
            for (int j = 0; j < subArray.size(); ++j) {
                if(subArray.at(j).toInt() == 0)
                {
                    emit send_road_boundary(subArray.at(j-1).toInt(), subArray.at(j+1).toInt());
                    continue;
                }

                path_node_origin.append(QString(subArray.at(j).toInt()) + ',');
            }
            path_node_origin.append('#');
        }
    }
    qDebug() << "send_path_info_to_camera";
    emit send_path_info_to_camera(path_node_origin);
    qDebug() << "send_path_info_to_camera over";
}

void Pathplan::run()
{

}


void Pathplan::path_plan()
{
    //proc->execute("python3 ../vision_trace_host/path.py");
    //    emit read_path_plan();
}
