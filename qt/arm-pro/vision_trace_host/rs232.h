#ifndef RS232_H
#define RS232_H
#include <stdio.h>
#include <string.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

class RS232 {
public:
    RS232();
    ~RS232();
    bool OpenDevice(const char *port,int baudrate);
    void CloseDevice(void);

    int Write(unsigned char *buf,int bufSize);
    int Write(const char *buf,int bufSize);
    int Read(unsigned char *buf,int bufSize);
private:
    int errCode;
    int fd;
    struct termios opt;
};

#endif // RS232_H
