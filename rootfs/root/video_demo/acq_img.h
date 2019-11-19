#ifndef __ACQ_IMG_H_
#define __ACQ_IMG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <thread.h>
#include "v4l2_c/v4l2.h"
#include "core/wwk_type.h"
#include "core/wwk_uchar.h"
#include "core/wwk_queue.h"

typedef struct acq_img
{
    pass_data m_v4l2;
    wwk_queue_t *image_q;
    wwk_uchar_t *yuv_image;
    wwk_uchar_t *gray_image;
    unsigned int run:1;
}acq_img_t;

typedef struct acq_img_queue_node
{
    wwk_uchar_t *store_image;
    wwk_queue_node_t *n;
}acq_img_queue_node_t;


void acq_main_work(void);
int acq_image_init(void);
int acq_image_delete(acq_img_t *a);

int acq_image_work(acq_img_t *a);
int acq_image_getimag_work(acq_img_t *a, wwk_uchar_t *acq_img);
int convert_yuv_to_gray_buffer(unsigned char *yuv, unsigned int yuv_len, unsigned char *gray, unsigned int *gray_len);

void get_current_image(acq_img_t *a);

#ifdef __cplusplus
};
#endif

#endif