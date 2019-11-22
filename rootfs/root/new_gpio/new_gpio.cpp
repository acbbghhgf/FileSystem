#include "new_gpio.h"

New_Gpio::New_Gpio(void)
{
    gpio0 = 55;
    gpio1 = 56;
    gpio2 = 57;
    gpio3 = 58;
    gpio4 = 59;
    gpio7 = 60;

    g_gpio0Status = false;
    g_gpio1Status = false;
    g_gpio2Status = false;
    g_gpio3Status = false;
    g_gpio4Status = false;
    g_gpio7Status = false;

    g_gpio0TrueCounter = 0;
    g_gpio0FalseCounter = 0;
    g_gpio1TrueCounter = 0;
    g_gpio1FalseCounter = 0;
    g_gpio2TrueCounter = 0;
    g_gpio2FalseCounter = 0;
    g_gpio3TrueCounter = 0;
    g_gpio3FalseCounter = 0;
    g_gpio4TrueCounter = 0;
    g_gpio4FalseCounter = 0;
    g_gpio7TrueCounter = 0;
    g_gpio7FalseCounter = 0;

    gpio0_fd = -1;
    gpio1_fd = -1;
    gpio2_fd = -1;
    gpio3_fd = -1;
    gpio4_fd = -1;
    gpio7_fd = -1;
    gpio_num = 6;
    //init
    Gpio_init();
}

void New_Gpio::Gpio_init(void)
{
    //申请GPIO资源并设置相应的输入输出方向。
    gpio_export(gpio0);
    gpio_set_dir(gpio0, 0);
    gpio_export(gpio1);
    gpio_set_dir(gpio1, 0);
    gpio_export(gpio2);
    gpio_set_dir(gpio2, 0);
    gpio_export(gpio3);
    gpio_set_dir(gpio3, 0);
    gpio_export(gpio4);
    gpio_set_dir(gpio4, 0);
    gpio_export(gpio7);
    gpio_set_dir(gpio7, 0);
    // init open gpio fds.

    Gpio_get_value_init();
}

//gpio get value init 
//return -1- error , 0-success.
int New_Gpio::Gpio_get_value_init(void)
{
    int ret = 0;
    opt_fds = (struct pollfd *)malloc(sizeof(struct pollfd));
    
    gpio0_fd = gpio_fd_open(gpio0);
    gpio1_fd = gpio_fd_open(gpio1);
    gpio2_fd = gpio_fd_open(gpio2);
    gpio3_fd = gpio_fd_open(gpio3);
    gpio4_fd = gpio_fd_open(gpio4);
    gpio7_fd = gpio_fd_open(gpio7);
    return 0;
}

void New_Gpio::Gpio_exit(void)
{
    if(opt_fds)
        free(opt_fds);
    //close fd
    if(gpio0_fd > 0)
        gpio_fd_close(gpio0_fd);
    if(gpio1_fd > 0)
        gpio_fd_close(gpio1_fd);
    if(gpio2_fd > 0)
        gpio_fd_close(gpio2_fd);
    if(gpio3_fd > 0)
        gpio_fd_close(gpio3_fd);
    if(gpio4_fd > 0)
        gpio_fd_close(gpio4_fd);
    if(gpio7_fd > 0)
        gpio_fd_close(gpio7_fd);

    gpio_unexport(gpio0);
    gpio_unexport(gpio1);
    gpio_unexport(gpio2);
    gpio_unexport(gpio3);
    gpio_unexport(gpio4);
    gpio_unexport(gpio7);
}

New_Gpio::~New_Gpio()
{
    Gpio_exit();
}

int New_Gpio::add_poll_gpio_dev_fd(int fd, int num)
{
    if(!opt_fds)
    {
        printf("add poll dev fd fail\n");
        return -1;
    }
    opt_fds[num].fd = fd;
    opt_fds[num].events = POLLPRI;
}

GpioSumType_new New_Gpio::getGpioSumType(void)
{
    GpioSumType_new ret_type;
//     if(scan_gpio())
//     {
//         std::cerr << "scan gpio val error" << std::endl;
//         return GPIO_VAL_ERR;
//     }
    if(scan_gpio_block())
    {
        std::cerr << "scan gpio val error" << std::endl;
        return GPIO_VAL_ERR;
    }
    gpio7_process();
    gpio1_process();
    gpio2_process();
    gpio3_process();
    gpio4_process();
    
    if (g_gpio0)
    {
        //gpiosum->0
        if (!g_gpio0Status && g_gpio0TrueCounter >= MAX_BOOL_COUNTER)
        {
            //zhixingxiangxing
            //             printf("0 = 1\n");
            switch (gpiosum)
            {
            case 1: //DA
                ret_type = GPIO_VAL_DA;
                //                 printf("gpio 1\n");
                break;
            case 2: //CA
                ret_type = GPIO_VAL_CA;
                break;
            case 3: //CDA
                ret_type = GPIO_VAL_CDA;
                //                 printf("gpio 3\n");
                break;
            case 4: //BA
                ret_type = GPIO_VAL_BA;
                //                 printf("gpio 4\n");
                break;
            case 5: //BDA
                ret_type = GPIO_VAL_BDA;
                //                 printf("gpio 5\n");
                break;
            case 6: //BCA
                ret_type = GPIO_VAL_BCA;
                //                 printf("gpio 6\n");
                break;
            case 7: //BCDA
                ret_type = GPIO_VAL_BCDA;
                //                 printf("gpio 7\n");
                break;
            case 8: //EA
                ret_type = GPIO_VAL_EA;
                break;
            case 9: //EDA
                ret_type = GPIO_VAL_EDA;
                break;
            case 10: //EDA
                ret_type = GPIO_VAL_ECA;
                break;
            case 11: //EDA
                ret_type = GPIO_VAL_ECDA;
                break;
            case 12: //EDA
                ret_type = GPIO_VAL_EBA;
                break;
            case 13: //EDA
                ret_type = GPIO_VAL_EBDA;
                break;
            case 14: //EDA
                ret_type = GPIO_VAL_EBCA;
                break;
            case 15: //EDA
                ret_type = GPIO_VAL_EBCDA;
                break;
            default:
                ret_type = GPIO_VAL_UNDIFINE;
            }
            g_gpio0Status = true;
            g_gpio0FalseCounter = 0;
        }
        else
        {
            if (g_gpio0TrueCounter < MAX_BOOL_COUNTER)
            {
                g_gpio0TrueCounter++;
                ret_type = GPIO_VAL_NULL;
            }
        }
    }
    else
    {
        if (g_gpio0Status && g_gpio0FalseCounter >= MAX_BOOL_COUNTER)
        {
            gpiosum = 0;
            g_gpio0Status = false;
            g_gpio0TrueCounter = 0;
            ret_type = GPIO_VAL_NULL;
        }
        else
        {
            if (g_gpio0FalseCounter < MAX_BOOL_COUNTER)
            {
                g_gpio0FalseCounter++;
                ret_type = GPIO_VAL_NULL;
            }
        }
    }
    return ret_type;
}

void New_Gpio::gpio7_process(void)
{
        //restart
    if (gpio7sum > 0)
    {
        if (!g_gpio7)
        {
            if (!g_gpio7Status && g_gpio7TrueCounter >= RESTART_COUNTER)
            {
                g_gpio7Status = true;
                gpio7count++;
                g_gpio7FalseCounter = 0;
            }
            else
            {
                if (g_gpio7TrueCounter < RESTART_COUNTER)
                {
                    g_gpio7TrueCounter++;
                }
            }
        }
        else
        {
            if (g_gpio7Status && g_gpio7FalseCounter >= RESTART_COUNTER)
            {
                g_gpio7Status = false;
                g_gpio7TrueCounter = 0;
            }
            else
            {
                if (g_gpio7FalseCounter < RESTART_COUNTER)
                {
                    g_gpio7FalseCounter++;
                }
            }
        }
    }
    if (gpio7flag)
    {
        gpio7flag = 0;
        if (gpio7count == 1)
        {
            std::cerr << "gpio7 duankai" << std::endl;
        }
        else
        {
            std::cerr << "gpio7 connect" << std::endl;
        }
    }
}

void New_Gpio::gpio1_process(void)
{
    if (g_gpio1)
    {
        if (!g_gpio1Status && g_gpio1TrueCounter >= MAX_BOOL_COUNTER)
        {
            g_gpio1Status = true;
            gpiosum += 4;
            g_gpio1FalseCounter = 0;
        }
        else
        {
            if (g_gpio1TrueCounter < MAX_BOOL_COUNTER)
            {
                g_gpio1TrueCounter++;
            }
        }
    }
    else
    {
        if (g_gpio1Status && g_gpio1FalseCounter >= MAX_BOOL_COUNTER)
        {
            g_gpio1Status = false;
            g_gpio1TrueCounter = 0;
        }
        else
        {
            if (g_gpio1FalseCounter < MAX_BOOL_COUNTER)
            {
                g_gpio1FalseCounter++;
            }
        }
    }
}

void New_Gpio::gpio2_process(void)
{
    if (g_gpio2)
    {
        if (!g_gpio2Status && g_gpio2TrueCounter >= MAX_BOOL_COUNTER)
        {
            g_gpio2Status = true;
            gpiosum += 2;
            g_gpio2FalseCounter = 0;
        }
        else
        {
            if (g_gpio2TrueCounter < MAX_BOOL_COUNTER)
            {
                g_gpio2TrueCounter++;
            }
        }
    }
    else
    {
        if (g_gpio2Status && g_gpio2FalseCounter >= MAX_BOOL_COUNTER)
        {
            g_gpio2Status = false;
            g_gpio2TrueCounter = 0;
        }
        else
        {
            if (g_gpio2FalseCounter < MAX_BOOL_COUNTER)
            {
                g_gpio2FalseCounter++;
            }
        }
    }
}

void New_Gpio::gpio3_process(void)
{
    //gpio3
    if (g_gpio3)
    {
        if (!g_gpio3Status && g_gpio3TrueCounter >= MAX_BOOL_COUNTER)
        {
            g_gpio3Status = true;
            gpiosum += 1;
            printf("3 = 1  %d\n", g_gpio3FalseCounter);
            g_gpio3FalseCounter = 0;
        }
        else
        {
            if (g_gpio3TrueCounter < MAX_BOOL_COUNTER)
                g_gpio3TrueCounter++;
        }
    }
    else
    {
        if (g_gpio3Status && g_gpio3FalseCounter >= MAX_BOOL_COUNTER)
        {
            g_gpio3Status = false;
            printf("3 = 0  %d\n", g_gpio3TrueCounter);
            g_gpio3TrueCounter = 0;
        }
        else
        {
            if (g_gpio3FalseCounter < MAX_BOOL_COUNTER)
                g_gpio3FalseCounter++;
        }
    }
}

void New_Gpio::gpio4_process(void)
{
    //gpio4    E
    if (g_gpio4)
    {
        if (!g_gpio4Status && g_gpio4TrueCounter >= MAX_BOOL_COUNTER)
        {
            g_gpio4Status = true;
            gpiosum += 8;
            //                printf("4 = 1  %d\n",g_gpio4FalseCounter);
            g_gpio4FalseCounter = 0;
        }
        else
        {
            if (g_gpio4TrueCounter < MAX_BOOL_COUNTER)
                g_gpio4TrueCounter++;
        }
    }
    else
    {
        if (g_gpio4Status && g_gpio4FalseCounter >= MAX_BOOL_COUNTER)
        {
            g_gpio4Status = false;
            //                printf("4 = 0  %d\n",g_gpio4TrueCounter);
            g_gpio4TrueCounter = 0;
        }
        else
        {
            if (g_gpio4FalseCounter < MAX_BOOL_COUNTER)
                g_gpio4FalseCounter++;
        }
    }
}


/**************************************************************** 
 * gpio_export 
 ****************************************************************/  
int New_Gpio::scan_gpio(void)
{
    if(gpio_get_value(gpio0, &g_gpio0)){
        printf("get gpio0 value error.\n");
        goto err;
    }
    if(gpio_get_value(gpio1, &g_gpio1)){
        printf("get gpio1 value error.\n");
        goto err;
    }
    if(gpio_get_value(gpio2, &g_gpio2)){
        printf("get gpio2 value error.\n");
        goto err;
    }
    if(gpio_get_value(gpio3, &g_gpio3)){
        printf("get gpio3 value error.\n");
        goto err;
    }
    if(gpio_get_value(gpio4, &g_gpio4)){
        printf("get gpio4 value error.\n");
        goto err;
    }
    if(gpio_get_value(gpio7, &g_gpio7)){
        printf("get gpio7 value error.\n");
        goto err;
    }
    return 0;

err:
    printf("get gpio value error.\n");
    return -1;
}

/**************************************************************** 
 * gpio_export 
 * -1 error, 0- timeout and success
 ****************************************************************/ 
int New_Gpio::scan_gpio_block(void)
{
    if(gpio_get_value_noblock(gpio0_fd, &g_gpio0))
    {
        printf("get gpio0 value error.\n");
        goto scan_err;
    }
    if(gpio_get_value_noblock(gpio1_fd, &g_gpio1))
    {
        printf("get gpio0 value error.\n");
        goto scan_err;
    }
    if(gpio_get_value_noblock(gpio2_fd, &g_gpio2))
    {
        printf("get gpio0 value error.\n");
        goto scan_err;
    }
    if(gpio_get_value_noblock(gpio3_fd, &g_gpio3))
    {
        printf("get gpio0 value error.\n");
        goto scan_err;
    }
    if(gpio_get_value_noblock(gpio4_fd, &g_gpio4))
    {
        printf("get gpio0 value error.\n");
        goto scan_err;
    }
    if(gpio_get_value_noblock(gpio7_fd, &g_gpio7))
    {
        printf("get gpio0 value error.\n");
        goto scan_err;
    }
    
    return 0;
scan_err:
    printf("get gpio value error.\n");
    return -1;
}

/**************************************************************** 
 * gpio_export 
 ****************************************************************/  
int New_Gpio::gpio_export(const unsigned int gpio)  
{  
    int fd, len;  
    char buf[TMP_BUF];  
   
    fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);  
    if (fd < 0) {  
        perror("gpio/export"); 
        return fd;  
    }  
   
    len = snprintf(buf, sizeof(buf), "%d", gpio);  
    write(fd, buf, len);  
    close(fd);  
   
    return 0;  
}  
  
/**************************************************************** 
 * gpio_unexport 
 ****************************************************************/  
int New_Gpio::gpio_unexport(const unsigned int gpio)  
{  
    int fd, len;  
    char buf[TMP_BUF];  
   
    fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);  
    if (fd < 0) {  
        perror("gpio/export");  
        return fd;  
    }  
   
    len = snprintf(buf, sizeof(buf), "%d", gpio);  
    write(fd, buf, len);  
    close(fd);  
    return 0;  
}  
  
/**************************************************************** 
 * gpio_set_dir out_flag- 1-output 0-intput
 ****************************************************************/  
int New_Gpio::gpio_set_dir(const unsigned int gpio, unsigned int out_flag)  
{  
    int fd, len;  
    char buf[TMP_BUF];  
   
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);  
   
    fd = open(buf, O_WRONLY);  
    if (fd < 0) {  
        perror("gpio/direction");  
        return fd;  
    }  
   
    if (out_flag)  
        write(fd, "out", 4);  
    else  
        write(fd, "in", 3);  
   
    close(fd);  
    return 0;  
}  
  
/**************************************************************** 
 * gpio_set_value 
 ****************************************************************/  
int New_Gpio::gpio_set_value(const unsigned int gpio, unsigned int value)  
{  
    int fd, len;  
    char buf[TMP_BUF];  
   
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);  
   
    fd = open(buf, O_WRONLY);  
    if (fd < 0) {  
        perror("gpio/set-value");  
        return fd;  
    }  
   
    if (value)  
        write(fd, "1", 2);  
    else  
        write(fd, "0", 2);  
   
    close(fd);  
    return 0;  
}  
  
int New_Gpio::gpio_get_value_noblock(int gpiofd, int *value)
{
    char ch;
    int ret = 0;

    if (gpiofd < 0) {  
        perror("gpio/get-value");
        return gpiofd;  
    }
    if(lseek(gpiofd,0,SEEK_SET)==-1)
    {
        perror("lseek failed!\n");
        return -1;
    }
    ret = read(gpiofd, &ch, 1);
    if(ret < 0)
    {
        if(errno == EAGAIN)
        {
//            printf("read fast error.\n");
            return 0;
        }
        perror("read dev fd ");
        return -1;
    }
  
    if (ch != '0') {  
        *value = 1;  
    } else {  
        *value = 0;  
    }  
    return 0;  
}

/**************************************************************** 
 * gpio_get_value return -1 error 0-success
 ****************************************************************/  
int New_Gpio::gpio_get_value(const unsigned int gpio, int *value)  
{  
    int fd, len;  
    char buf[TMP_BUF];  
    char ch;  
  
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);  
   
    fd = open(buf, O_RDONLY);  
    if (fd < 0) {  
        perror("gpio/get-value");
        printf("gpio %u device open error.\n", gpio);
        return fd;  
    }  
   
    read(fd, &ch, 1);  
  
    if (ch != '0') {  
        *value = 1;  
    } else {  
        *value = 0;  
    }  
   
    close(fd);  
    return 0;  
}  
  
  
/**************************************************************** 
 * gpio_set_edge 
 ****************************************************************/  
  
int New_Gpio::gpio_set_edge(const unsigned int gpio, char *edge)  
{  
    int fd, len;  
    char buf[TMP_BUF];  
  
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);  
   
    fd = open(buf, O_WRONLY);  
    if (fd < 0) {  
        perror("gpio/set-edge");  
        return fd;  
    }  
   
    write(fd, edge, strlen(edge) + 1);   
    close(fd);  
    return 0;  
}  
  
/**************************************************************** 
 * gpio_fd_open 
 ****************************************************************/  
  
int New_Gpio::gpio_fd_open(const unsigned int gpio)  
{  
    int fd, len;  
    char buf[TMP_BUF];  
  
    len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);  
   
    fd = open(buf, O_RDONLY | O_NONBLOCK );  
    if (fd < 0) {  
        perror("gpio/fd_open");  
    }
//    printf("open gpio/fd success, buf = %s\n", buf);
    return fd;  
}  
  
/**************************************************************** 
 * gpio_fd_close 
 ****************************************************************/  
  
int New_Gpio::gpio_fd_close(int fd)  
{  
    return close(fd);  
} 
