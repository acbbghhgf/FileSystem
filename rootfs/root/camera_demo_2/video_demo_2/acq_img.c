#include "acq_img.h"

void acq_main_work(void)
{
    pthread_t t;
    int ret = 0;
    wwk_uchar_t *uc_image;
    acq_img_t *a = acq_image_init();
    a->run = 1;
    pthread_create(&t, NULL, acq_image_work, (void *)a);
    sleep(5);
    for(;;)
    {
        sleep(1);
        ret = acq_image_getimag_work(a, &uc_image);
        if(uc_image && ret != -1)
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

void* acq_image_work(void *param)
{
    acq_img_t* a = (acq_img_t*)param;
    int ret = 0;
    if(!a)
    {
        wwk_debug("a is null\n");
        ret = -1;
        return (void*)&ret;
    }
    int dev_flag = init_dev(&a->m_v4l2);
    if(dev_flag != 0)
    {
        wwk_debug("init dev fail\n");
        ret = -1;
        return (void*)&ret;
    }
    while(a->run)
    {
        read_frame(&a->m_v4l2);
        //process image
        get_current_image(a);
        return_data(&a->m_v4l2);
//        sleep(1);
    }
    stop_dev(&a->m_v4l2);
    ret = 0;
    return (void*)&ret;
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
    wwk_debug("work thread pop image length : [%d]\n", node->store_image->len);
    *acq_img = node->store_image;
    *acq_img = wwk_uchar_dup_data(node->store_image->data, node->store_image->len);
    wwk_uchar_delete(node->store_image);
    wwk_free(node);
}


void get_current_image(acq_img_t *a)
{
    a->yuv_image = wwk_uchar_dup_data((unsigned char *)a->m_v4l2.buffers[a->m_v4l2.buf.index].start, a->m_v4l2.buffers[a->m_v4l2.buf.index].length);
    wwk_debug("work thread push image length : [%d] \n", a->yuv_image->len);
    //convert yuv to gray.
    acq_img_queue_node_t *node = wwk_malloc(sizeof(*node));
//    node->store_image = wwk_uchar_dup_data(a->yuv_image->data, a->yuv_image->len);
    node->store_image = convert_yuv_to_gray_uchar(a->yuv_image);
    wwk_free(a->yuv_image);
    //push  queue data
    wwk_queue_push(a->image_q, &node->n);
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
//        *(pTemp_int + i)= *pIn & 0xff00ff00 | 0x00800080;
        *(pTemp_int + i)= *pIn & 0x00ff00ff;
        pIn++;
//        pTemp_int++;
    }
    yuv_len >>= 1;
    for(i = 0; i < yuv_len; i ++)
    {
        *gray = *pTemp_short;// >> 8;
        gray++;
        pTemp_short++;
    }
    *gray_len = yuv_len;
    wwk_free(pTemp_int);
    return *gray_len;
}

wwk_uchar_t * convert_yuv_to_gray_uchar(wwk_uchar_t *yuv)
{
    int ret = 0;
    wwk_uchar_t *tmp = wwk_uchar_dup_data_x(yuv);
    wwk_uchar_t *gray;
    wwk_debug("tmp len : %d\n", tmp->len);
    ret = convert_yuv_to_gray_buffer(yuv->data, yuv->len, tmp->data, &tmp->len);
    if(ret != tmp->len || ret == 0)
    {
        wwk_debug(">>>>>\n");
        wwk_uchar_delete(tmp);
        return NULL;
    }
    wwk_debug("tmp after len : %d\n", tmp->len);
    gray = wwk_uchar_dup_data_x(tmp);
    wwk_uchar_delete(tmp);
    return gray;
}
