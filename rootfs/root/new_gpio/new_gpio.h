#ifndef NEW_GPIO_H
#define NEW_GPIO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <iostream>

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BOOL_COUNTER 50
#define RESTART_COUNTER 50
#define POLL_TIMEOUT    300
#define TMP_BUF 64

//typedef enum GpioSumType
//{
//    GPIO_VAL_DA = 1,
//    GPIO_VAL_CA,
//    GPIO_VAL_CDA,
//    GPIO_VAL_BA,
//    GPIO_VAL_BDA,
//    GPIO_VAL_BCA,
//    GPIO_VAL_BCDA,
//    GPIO_VAL_EA,
//    GPIO_VAL_EDA,
//    GPIO_VAL_ECA,
//    GPIO_VAL_ECDA,
//    GPIO_VAL_EBA,
//    GPIO_VAL_EBDA,
//    GPIO_VAL_EBCA,
//    GPIO_VAL_EBCDA,
//    GPIO_VAL_NULL,
//    GPIO_VAL_ERR,
//    GPIO_VAL_UNDIFINE
//}GpioSumType_t;


typedef enum
{
    GPIO_VAL_DA = 1,
    GPIO_VAL_CA,
    GPIO_VAL_CDA,
    GPIO_VAL_BA,
    GPIO_VAL_BDA,
    GPIO_VAL_BCA,
    GPIO_VAL_BCDA,
    GPIO_VAL_EA,
    GPIO_VAL_EDA,
    GPIO_VAL_ECA,
    GPIO_VAL_ECDA,
    GPIO_VAL_EBA,
    GPIO_VAL_EBDA,
    GPIO_VAL_EBCA,
    GPIO_VAL_EBCDA,
    GPIO_VAL_NULL,
    GPIO_VAL_ERR,
    GPIO_VAL_UNDIFINE
}GpioSumType_new;

class New_Gpio{
public:
    New_Gpio(void);
    void Gpio_init(void);
    GpioSumType_new getGpioSumType(void);
    void Gpio_exit(void);
    ~New_Gpio(void);

private:
    int gpio_export(const unsigned int gpio);//申请GPIO管脚资源
    int gpio_unexport(const unsigned int gpio);//卸载GPIO管脚资源
    int gpio_set_dir(const unsigned int gpio, unsigned int out_flag);//设置GPIO方向
    int gpio_set_value(const unsigned int gpio, unsigned int value);//设置GPIO输出值
    int gpio_get_value(const unsigned int gpio, int *value);//获取GPIO读取值
    int gpio_get_value_noblock(int gpiofd, int *value);//直接从之前打开的fd中读取数据
    int gpio_set_edge(const unsigned int gpio, char *edge);//设置GPIO触发边沿
    int gpio_fd_open(const unsigned int gpio);//非阻塞方式打开GPIO设备
    int gpio_fd_close(int fd);//关闭GPIO设备
    int Gpio_get_value_init(void);
    int add_poll_gpio_dev_fd(int fd, int num);

    //process
    int scan_gpio(void);
    int scan_gpio_block(void);
    void gpio1_process(void);
    void gpio2_process(void);
    void gpio3_process(void);
    void gpio4_process(void);
    void gpio7_process(void);

private:
//init gpio number
    unsigned int gpio0;
    unsigned int gpio1;
    unsigned int gpio2;
    unsigned int gpio3;
    unsigned int gpio4;
    unsigned int gpio7;

// total gpio counter
    unsigned int gpio_num;
    struct pollfd *opt_fds;
// gpio fd
    int gpio0_fd;
    int gpio1_fd;
    int gpio2_fd;
    int gpio3_fd;
    int gpio4_fd;
    int gpio7_fd;

//tmp gpio value
    int g_gpio0;
    int g_gpio1;
    int g_gpio2;
    int g_gpio3;
    int g_gpio4;
    int g_gpio7;

    int gpiosum;
    int gpiopre;
    int gpio7count;
    long gpio7sum;
    int gpio7flag;

    //bool status
    bool g_gpio0Status;
    bool g_gpio1Status;
    bool g_gpio2Status;
    bool g_gpio3Status;
    bool g_gpio4Status;
    bool g_gpio7Status;

    //bool counter
    int g_gpio0TrueCounter;
    int g_gpio0FalseCounter;
    int g_gpio1TrueCounter;
    int g_gpio1FalseCounter;
    int g_gpio2TrueCounter;
    int g_gpio2FalseCounter;
    int g_gpio3TrueCounter;
    int g_gpio3FalseCounter;
    int g_gpio4TrueCounter;
    int g_gpio4FalseCounter;
    int g_gpio7TrueCounter;
    int g_gpio7FalseCounter;
};


#endif
