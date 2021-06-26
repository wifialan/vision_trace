#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QDebug>
#include <QString>

class Protocol
{
public:
    Protocol();

public:
    QString GetRCSCmd(int addr);
    QString GetRCCCmd(int addrBegin,int addrEnd);
    QString GetSRCmd(int addr,bool on);
    QString GetWriteDWordCmd(int addr,quint64 value);
    QString GetRDCmd(int addr_begin,int addr_end);

    static bool IsBccFailure(const char* data);

    static bool IsWcsAck(const char* data);
    static bool IsRcsAck(const char* data);
    static bool IsRccAck(const char* data);

    static bool IsRdAck(const char* data);
    static bool IsRdMultiAck(const char* data);
    static bool IsWdAck(const char* data);

    static bool GetRcsData(const char* data);
    static void GetRccData(const char* msg,int* data, int* len);

    static int GetRDData(const char* data);
    static int GetMultiDTData(const char* msg,char* data,int len);

private:
    static const char* GetBcc(const char* cmd);
};

#endif // PROTOCOL_H
