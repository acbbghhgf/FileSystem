#ifndef __USBSERIAL_H
#define __USBSERIAL_H
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <signal.h>

class usbserial
{
public:
    usbserial();
    usbserial(char *devname, int speed = 9600);
    int send(char *buf, int len);
    int recv(char *buf, int len);
    ~usbserial();
private:
    speed_t getBaudrate(int baudrate);
    int OpenDev();
    int CloseDev();

private:
    /* data */
    speed_t usbserial_speed;
    char *usbserial_devName;
    int dev_fd;

};

#endif