#include "acq_img.h"

void acq_main_work(void)
{
    pthread_t t;
    wwk_uchar_t *uc_image;
    acq_img_t *a = acq_image_init();
    a->run = 1;
    pthread_create(&t, acq_image_work, (void *)a);
    sleep(5);
    for(;;)
    {
        acq_image_getimag_work(a, &uc_image);
        if(uc_image)
        {
            wwk_debug("obtin image length : %d\n", uc_image->len);
            wwk_free(uc_image);
        }
        else
        {
            wwk_debug("obtion image error....\n");
        }
    }
}

acq_img_t* acq_image_init(void)
{
    acq_img_t *a = (acq_img_t *)wwk_malloc(sizeof(*a));
    a->m_v4l2.dev_name = "/dev/video0";
    a->image_q = wwk_queue_new();
    a->run = 0;
    return a;
}

int acq_image_delete(acq_img_t *a)
{
    if(a->image_q)
    {
        wwk_queue_delete(a->image_q);
    }
    if(a->yuv_image)
    {
        wwk_uchar_delete(a->yuv_image);
    }
    if(a->gray_image)
    {
        wwk_uchar_delete(a->gray_image);
    }
    wwk_free(a);
}

int acq_image_work(void *param)
{
    acq_img_t* a = (acq_img_t*)param;
    if(!a)
    {
        wwk_debug("a is null\n");
        return -1;
    }
    int dev_flag = init_dev(&a->m_v4l2);
    if(dev_flag != 0)
    {
        wwk_debug("init dev fail\n");
        return -1;
    }
    while(a->run)
    {
        read_frame(&m_v4l2);
        //process image
        get_current_image(a);
        return_data(&m_v4l2);
    }
    stop_dev(&a->m_v4l2);
    return 0;
}

int acq_image_getimag_work(acq_img_t *a, wwk_uchar_t **acq_img)
{
    wwk_queue_node_t *qn = wwk_queue_pop(a->image_q);
    if(!qn)
    {
        wwk_debug("qn is null , current image is null\n");
        return -1;
    }
    acq_img_queue_node_t *node = data_offset2(qn, acq_img_queue_node_t, n);
    if(!node)
    {
        wwk_debug("node is null, data_offset2 error.\n");
        return -1;
    }
    *acq_img = node->store_image;
    wwk_free(node);
}


void get_current_image(acq_img_t *a)
{
    a->yuv_image = wwk_uchar_dup_data((unsigned char *)a->m_v4l2.buffers[a->m_v4l2.buf.index].start, a->m_v4l2.buffers[a->m_v4l2.buf.index].length);
    //convert yuv to gray.
    acq_img_queue_node_t *node = wwk_malloc(sizeof(*node));
    node->store_image->data = a->yuv_image->data;
    node->store_image->len = a->yuv_image->len;
    a->yuv_image = 0;
    //push  queue data
    wwk_queue_push(a->image_q, node->n);
}

int convert_yuv_to_gray_buffer(unsigned char *yuv, unsigned int yuv_len, unsigned char *gray, unsigned int *gray_len)
{
    unsigned int i;
    unsigned int *pIn = (unsigned int*)yuv;
    unsigned int *pTemp_int= (unsigned int*)wwk_malloc(sizeof(unsigned char) * yuv_len);
    unsigned short *pTemp_short = (unsigned short *)pTemp_int;
    int len = yuv_len >> 2;
    
    for(i=0; i < len; i++)
    {
        *pTemp_int= *pIn & 0xff00ff00 | 0x00800080;
        pIn++;
        pTemp_int++;
    }
    yuv_len >>= 1;
    for(i = 0; i < yuv_len; i ++)
    {
        *gray = *pTemp_short >> 8;
        gray++;
        pTemp_short++;
    }
    *gray_len = yuv_len;
    return *gray_len;
}