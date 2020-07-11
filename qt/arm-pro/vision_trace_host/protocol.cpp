#include "protocol.h"

Protocol::Protocol()
{

}


//获取写R寄存器命令
//addr，R寄存器地址
//on, 写入寄存器的值，0或者1。一般来说执行某项功能时写入1
const char* Protocol::GetSRCmd(int addr,bool on)
{
    QString cmd;

    char *str_cmd;
    QString str;
    str.sprintf("%04X",addr);
    cmd.append("%01#WCSR");
    cmd.append(str);
    cmd.append(QString::number(on,10));
    QByteArray ba = cmd.toLatin1();
    str_cmd = ba.data();
    cmd.append(QString(QLatin1String(GetBcc(str_cmd))));
    cmd.append("\r");

    ba = cmd.toLatin1();
    str_cmd = ba.data();

    //qDebug() << str_cmd;

    //qDebug() << "-*-*-*-*-*-";
    return str_cmd;
}


//获取写入DT寄存器值的命令，写入的是双字
//addr，DT寄存器地址
//value，写入DT寄存器的值
const char* Protocol::GetWriteDWordCmd(int addr,quint64 value)
{
    QString cmd;

    char *str_cmd;
    QString addr_begin,addr_end;
    addr_begin.sprintf("%05d",addr);
    cmd.append("%01#WDD");
    cmd.append(addr_begin);
    addr_end.sprintf("%05d",addr+1);
    cmd.append(addr_end);

    if ( value < 0 )
        value = 0xFFFFFFFF +  value + 1;

    int data1 = (value & 0xFFFF);
    int data2 = ((value & 0xFFFF0000) >> 16);

    char *data_1,*data_2;

    QString string_data1,string_data2;
    string_data1.sprintf("%04X",data1);
    string_data2.sprintf("%04X",data2);

    QByteArray ba;
    ba = string_data1.toLatin1();
    data_1 = ba.data();
    ba = string_data2.toLatin1();
    data_2 = ba.data();

    char data[16];
    data[0] = data_1[2];
    data[1] = data_1[3];
    data[2] = data_1[0];
    data[3] = data_1[1];

    data[4] = data_2[2];
    data[5] = data_2[3];
    data[6] = data_2[0];
    data[7] = data_2[1];
    data[8] = '\0';

    cmd.append(data);

    QByteArray ba_bcc = cmd.toLatin1();
    str_cmd = ba_bcc.data();
    cmd.append(QString(QLatin1String(GetBcc(str_cmd))));
    cmd.append("\r");

    ba = cmd.toLatin1();
    str_cmd = ba.data();

    //qDebug() << str_cmd;

    return str_cmd;
}

#if 1
const char* Protocol::GetRDCmd(int addr_Begin,int addr_End)
{

    QString cmd;

    char *str_cmd;
    QString addr_begin,addr_end;
    addr_begin.sprintf("%05d",addr_Begin);
    cmd.append("%01#RDD");
    cmd.append(addr_begin);
    addr_end.sprintf("%05d",addr_End);
    cmd.append(addr_end);

    QByteArray ba_bcc = cmd.toLatin1();
    str_cmd = ba_bcc.data();
    cmd.append(QString(QLatin1String(GetBcc(str_cmd))));
    cmd.append("\r");

    QByteArray ba = cmd.toLatin1();
    str_cmd = ba.data();

    qDebug() << str_cmd;

    return str_cmd;
}

const char *Protocol::GetRCSCmd(int addr)
{

    QString cmd;

    char *str_cmd;
    QString addr_begin;
    addr_begin.sprintf("%04X",addr);
    cmd.append("%01#RCSR");
    cmd.append(addr);

    QByteArray ba_bcc = cmd.toLatin1();
    str_cmd = ba_bcc.data();
    cmd.append(QString(QLatin1String(GetBcc(str_cmd))));
    cmd.append("\r");

    QByteArray ba = cmd.toLatin1();
    str_cmd = ba.data();

    qDebug() << "RCSR: " << str_cmd;

    return str_cmd;

}

const char *Protocol::GetRCCCmd(int addrBegin,int addrEnd)
{

    QString cmd;

    char *str_cmd;
    QString addr_begin,addr_end;
    addr_begin.sprintf("%04X",addrBegin);
    cmd.append("%01#RCCR");
    addr_end.sprintf("%04X",addrEnd);
    cmd.append(addr_begin);
    cmd.append(addr_end);

    QByteArray ba_bcc = cmd.toLatin1();
    str_cmd = ba_bcc.data();
    cmd.append(QString(QLatin1String(GetBcc(str_cmd))));
    cmd.append("\r");

    QByteArray ba = cmd.toLatin1();
    str_cmd = ba.data();

    qDebug() << "RCCR: " << str_cmd;

    return str_cmd;
}

//计算命令校验码
const char* Protocol::GetBcc(const char* cmd)
{
    int len = strlen(cmd);
    int bcc = 0;
    for ( int i = 0; i < len; i++ )
        bcc = bcc ^ (*(cmd+i));

    static char *str_bcc;

    QString string_bcc;
    string_bcc.sprintf("%02x",bcc);
    QByteArray ba = string_bcc.toLatin1();

    str_bcc = ba.data();
    return str_bcc;
}
#if 0
bool Protocol::IsBccFailure(const char* data)
{
    int len = strlen(data) - 2;
    int bcc = 0;
    for ( int i = 0; i < len; i++ )
        bcc = bcc ^ (*(data+i));

    char str_bcc[4];
    sprintf_s(str_bcc,sizeof(str_bcc),"%X",bcc);
    len = strlen(str_bcc);
    if ( len == 1 )
        sprintf_s(str_bcc,sizeof(str_bcc),"0%X",bcc);
    else if ( len > 2 )
        assert(false);

    len = strlen(data);
    if( len > 2 )
    {
        if( str_bcc[0] == data[len-2] && str_bcc[1] == data[len-1] )
            return false;
    }
    return true;
}

bool Protocol::IsWcsAck(const char* data)
{
    if( strlen(data) != 8 )
        return false;

    if( data[3] == '$' && data[4] == 'W' && data[5] == 'C' )
        return true;
    else
        return false;
}
bool Protocol::IsRcsAck(const char* data)
{
    if( strlen(data) != 9 )
        return false;

    if( data[3] == '$' && data[4] == 'R' && data[5] == 'C' )
        return true;
    else
        return false;
}
bool Protocol::IsRccAck(const char* data)
{
    if( strlen(data) < 12 )
        return false;

    if( data[3] == '$' && data[4] == 'R' && data[5] == 'C' )
        return true;
    else
        return false;
}
bool Protocol::IsRdAck(const char* data)
{
    if( strlen(data) != 16 )
        return false;
    int k = strlen(data);
    if( data[3] == '$' && data[4] == 'R' && data[5] == 'D' )
        return true;
    else
        return false;
}
bool Protocol::IsRdMultiAck(const char* data)
{
    if( strlen(data) <= 16 )
        return false;
    int k = strlen(data);
    if( data[3] == '$' && data[4] == 'R' && data[5] == 'D' )
        return true;
    else
        return false;
}
bool Protocol::IsWdAck(const char* data)
{
    if( strlen(data) != 8 )
        return false;

    if( data[3] == '$' && data[4] == 'W' && data[5] == 'D' )
        return true;
    else
        return false;
}

bool Protocol::GetRcsData(const char* data)
{
    return (data[6] == '1' );
}
void Protocol::GetRccData(const char* msg,int* data, int* len)
{
    int count = strlen(msg);
    int dataLen = count - 6 - 2;
    if( *len < (dataLen / 4))
    {
        *len = 0;
        return;
    }

    int i = 0;
    int k = 0;
    int j = 0;
    char buff[8];
    const char* p = msg + 6;
    while( i < dataLen )
    {
        buff[k] = *(p+i);
        i++;
        k++;
        if( (i % 4) == 0 )
        {
            char temp[8];
            temp[0] = buff[0];
            temp[1] = buff[1];
            temp[2] = '\0';
            int v1;
            sscanf_s(temp,"%X",&v1);

            temp[0] = buff[2];
            temp[1] = buff[3];
            temp[2] = '\0';
            int v2;
            sscanf_s(temp,"%X",&v2);

            data[j] =  ((v2 << 8) + v1);
            j++;
            k = 0;
        }
    }
    *len = j;
}

int Protocol::GetRDData(const char* data)
{
    char byte[8];
    int v,v1,v2,v11,v22;

    byte[0] = data[6];
    byte[1] = data[7];
    byte[2] = '\0';
    sscanf_s(byte,"%X",&v1);

    byte[0] = data[8];
    byte[1] = data[9];
    byte[2] = '\0';
    sscanf_s(byte,"%X",&v2);

    byte[0] = data[10];
    byte[1] = data[11];
    byte[2] = '\0';
    sscanf_s(byte,"%X",&v11);

    byte[0] = data[12];
    byte[1] = data[13];
    byte[2] = '\0';
    sscanf_s(byte,"%X",&v22);

    v= (((v22 << 8) + v11) << 16) + ((v2 << 8) + v1);
    return v;
}
int Protocol::GetMultiDTData(const char* msg,char* data,int len)
{
    int count = strlen(msg);
    int dataLen = count - 6 - 2;
    if( len < dataLen )
        return 1;

    for( int i = 0; i < dataLen; i++ )
    {
        *(data+i) = msg[6+i];
    }
    *(data+len) = '\0';
    return 0;
}
#endif
#endif
