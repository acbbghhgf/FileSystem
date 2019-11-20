/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Sun 17 Nov 2019 05:58:08 PM PST
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include "v4l2_c/v4l2.h"
#include "acq_img.h"

int main()
{
    printf("hello world\n");

	acq_main_work();
    return 0;
}

