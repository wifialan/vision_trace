#include "rs232.h"
#include <iostream>

RS232::RS232() {
    fd = -1;
    errCode = 0;
}
RS232::~RS232() {
    if(fd != -1){
        CloseDevice();
    }
}
bool RS232::OpenDevice(const char *port,int baudrate){
    int baudr;
    // Chossing baudrate
    switch(baudrate)
    {
        case      50 : baudr = B50;       break;
        case      75 : baudr = B75;       break;
        case     110 : baudr = B110;      break;
        case     134 : baudr = B134;      break;
        case     150 : baudr = B150;      break;
        case     200 : baudr = B200;      break;
        case     300 : baudr = B300;      break;
        case     600 : baudr = B600;      break;
        case    1200 : baudr = B1200;     break;
        case    1800 : baudr = B1800;     break;
        case    2400 : baudr = B2400;     break;
        case    4800 : baudr = B4800;     break;
        case    9600 : baudr = B9600;     break;
        case   19200 : baudr = B19200;    break;
        case   38400 : baudr = B38400;    break;
        case   57600 : baudr = B57600;    break;
        case  115200 : baudr = B115200;   break;
        case  230400 : baudr = B230400;   break;
        case  460800 : baudr = B460800;   break;
        case  500000 : baudr = B500000;   break;
        case  576000 : baudr = B576000;   break;
        case  921600 : baudr = B921600;   break;
        case 1000000 : baudr = B1000000;  break;
        default      : baudr = B115200;   break;
    }
    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1){
        errCode = -1;
        return false;
    }
    //memset(&opt, 0, sizeof(opt));  /* clear the new struct */
/*
    opt.c_cflag = baudr | CS8 | CLOCAL | CREAD;  //| ~CSTOPB
    opt.c_iflag = IGNPAR;
    opt.c_oflag = 0;
    opt.c_lflag = 0;
    opt.c_cc[VMIN] = 0;
    opt.c_cc[VTIME] = 0;
*/
    bzero (&opt, sizeof (opt));

    opt.c_cflag |=  baudr | CLOCAL | CREAD;
    opt.c_cflag &= ~CSIZE;
    opt.c_cflag |= CS8;
    opt.c_iflag &= ~INPCK ;
    opt.c_cflag &= ~PARENB;
    cfsetispeed (&opt, baudr);
    cfsetospeed (&opt, baudr);
    opt.c_cflag &= ~CSTOPB;
    opt.c_cc[VTIME] = 0;
    opt.c_cc[VMIN] = 0;
    opt.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_oflag  &= ~OPOST;

    opt.c_iflag &= ~(INLCR | ICRNL); //不要回车和换行转换
    opt.c_iflag &= ~(IXON | IXOFF | IXANY); //不要软件流控制

/*
    opt.c_cflag &= ~HUPCL;
    opt.c_iflag &= ~INPCK;
    opt.c_iflag |= IGNBRK;
    opt.c_iflag &= ~ICRNL;
    opt.c_iflag &= ~IXON;
    opt.c_lflag &= ~IEXTEN;
    opt.c_lflag &= ~ECHOK;
    opt.c_lflag &= ~ECHOCTL;
    opt.c_lflag &= ~ECHOKE;
    opt.c_oflag &= ~ONLCR;

    tcflush(fd,TCIFLUSH);
    */
    //opt.c_cflag &= ~CRTSCTS;
    //opt.c_iflag = IGNPAR;
    //opt.c_oflag = ~ICANON;
    //opt.c_lflag = 0;
    //opt.c_lflag &= ~(ICANON | ECHO | ECHOE);
    //opt.c_cc[VMIN] = 0;      /* block untill n bytes are received */
    //opt.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */
    errCode = tcsetattr(fd, TCSANOW, &opt);
    if(errCode == -1)
    {
       close(fd);
       return false;
    }
    return true;
}
void RS232::CloseDevice(void){
    close(fd);
}
int RS232::Write(unsigned char *buf,int bufSize){
    return write(fd, buf, bufSize);
}
int RS232::Write(const char *buf,int bufSize){
    return write(fd, buf, bufSize);
}
int RS232::Read(unsigned char *buf,int bufSize){
    return read(fd, buf, bufSize);
}
