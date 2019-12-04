#include "usbserial.h"


usbserial::usbserial()
{
    char def_devName[] = "/dev/ttyUSB0";
    usbserial_devName = new char[strlen(def_devName)+1];
    memcpy(usbserial_devName, def_devName, strlen(def_devName));
    usbserial_speed = getBaudrate(9600);
    dev_fd = -1;
    dev_fd = OpenDev();
}

usbserial::usbserial(char *devname, int speed)
{
    usbserial_speed = getBaudrate(speed);
    if(devname != nullptr){
	usbserial::usbserial_devName = new char[strlen(devname)+1];
        memcpy(usbserial_devName, devname, strlen(devname));
    }
    else{
	usbserial::usbserial_devName = nullptr;
    }
    dev_fd = -1;
    dev_fd = OpenDev();
}

usbserial::~usbserial()
{
    if(dev_fd != -1)
    {
        CloseDev();
    }
    if(usbserial_devName)
    {
        delete []usbserial_devName;
    }
}

speed_t usbserial::getBaudrate(int baudrate)
{
    
	switch(baudrate) {
	case 0: return B0;
	case 50: return B50;
	case 75: return B75;
	case 110: return B110;
	case 134: return B134;
	case 150: return B150;
	case 200: return B200;
	case 300: return B300;
	case 600: return B600;
	case 1200: return B1200;
	case 1800: return B1800;
	case 2400: return B2400;
	case 4800: return B4800;
	case 9600: return B9600;
	case 19200: return B19200;
	case 38400: return B38400;
	case 57600: return B57600;
	case 115200: return B115200;
	case 230400: return B230400;
	case 460800: return B460800;
	case 500000: return B500000;
	case 576000: return B576000;
	case 921600: return B921600;
	case 1000000: return B1000000;
	case 1152000: return B1152000;
	case 1500000: return B1500000;
	case 2000000: return B2000000;
	case 2500000: return B2500000;
	case 3000000: return B3000000;
	case 3500000: return B3500000;
	case 4000000: return B4000000;
	default: return -1;
	}
}

int usbserial::OpenDev()
{
    int fdt;
    struct termios oldtio,newtio;

    fdt=open(usbserial_devName, O_RDWR | O_NONBLOCK| O_NOCTTY | O_NDELAY);
    if(fdt<0)
    {
        perror(usbserial_devName);
        exit(1);
    }
    //save to oldtio
    tcgetattr(fdt,&oldtio);
    //clear newtio
    bzero(&newtio,sizeof(newtio));
    //newtio.c_cflag = speed|CS8|CLOCAL|CREAD|CRTSCTS;
    newtio.c_cflag = usbserial_speed|CS8|CLOCAL|CREAD;
    newtio.c_iflag = IGNPAR;  
    newtio.c_oflag = 0;
    //printf("newtio.c_cflag=%x\n",newtio.c_cflag);
    tcflush(fdt,TCIFLUSH);  
    tcsetattr(fdt,TCSANOW,&newtio);  
    tcgetattr(fdt,&oldtio);
    //printf("oldtio.c_cflag=%x\n",oldtio.c_cflag);
    return fdt; 
}

int usbserial::CloseDev()
{
    printf("close usb tty device.\n");
    if(dev_fd != -1)
    {
        close(dev_fd);
        dev_fd = -1;
    }
    return 0;
}

/*
 * return count - write buf len size counter,
 * 0,-1- error
 * */
int usbserial::send(char *buf, int len)
{
    return write(dev_fd, buf, len);
}

/*
 * return count - read buf len size counter,
 * 0,-1- error
 * */
int usbserial::recv(char *buf, int len)
{
    int nread = 0;
    nread = read(dev_fd, buf, len);
    return nread;
}
