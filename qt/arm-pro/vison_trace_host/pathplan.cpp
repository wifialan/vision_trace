#include "pathplan.h"


Pathplan::Pathplan()
{

}


void Pathplan::on_path_plan()
{

//    Mat mat_col = mat.col(100);
    qDebug() << "The 100th pix: " ;
#if 0
    QByteArray color;
    QBuffer buffer(&color);
    buffer.open(QIODevice::ReadWrite);
//    image.save(&buffer, "PNG");
    qint16 color_line_len = color.length();
    qint16 black_count = 0;
    qint16 black_index =0;
    qint16 black_center = 0;
    bool flag = false;
    //calc the black pix number and the first black pix location
    for(int i = 0; i < color_line_len; i++)
    {
        if(color[i] == 0){
            black_count ++;
        }
    }
    qDebug() << "black count: " << black_count;

    if (black_count > 20){

        black_center = 0;
    }
#endif
}
