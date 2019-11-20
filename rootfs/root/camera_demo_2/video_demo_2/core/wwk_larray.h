/*************************************************************************
	> File Name: wwk_larray.h
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Nov 2019 06:50:21 PM PST
 ************************************************************************/

#ifndef _WWK_LARRAY_H
#define _WWK_LARRAY_H

#include "core/wwk_type.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct wwk_larray wwk_larray_t;
#define wwk_larray_push2_s(l,t,v) {t x1=v;wwk_larray_push2(l,&x1);}

struct wwk_larray
{
	void* slot;
	uint32_t nslot;
	uint32_t slot_size;
	uint32_t slot_alloc;
};

wwk_larray_t* wwk_larray_new(uint32_t n,uint32_t size);
int wwk_larray_delete(wwk_larray_t *a);
int wwk_larray_bytes(wwk_larray_t *a);
wwk_larray_t* wwk_larray_dup(wwk_larray_t *a);
void wwk_larray_cpy(wwk_larray_t *src,wwk_larray_t *dst);
void wwk_larray_merge(wwk_larray_t *merged,wwk_larray_t *pad);
void wwk_larray_reset(wwk_larray_t *a);
void wwk_larray_reset2(wwk_larray_t *a,int n);
/**
 * @brief return address of element, for float element,will return float*;
 */
void* wwk_larray_push(wwk_larray_t* a);
void wwk_larray_push2(wwk_larray_t *a,void *src);
void* wwk_larray_push_n(wwk_larray_t* a,uint32_t n);
void* wwk_larray_get(wwk_larray_t *a,int idx);


typedef struct
{
	float *p;
	int len;
	int pos;
}wwk_flta_t;

wwk_flta_t* wwk_flta_new(int n);
void wwk_flta_delete(wwk_flta_t *a);
void wwk_flta_reset(wwk_flta_t *a);
void wwk_flta_zero(wwk_flta_t *a);
#define wwk_flta_push(a,f) (a)->p[(a)->pos++]=f


//------------------------------- test/examle section ------------------
void wwk_larray_test_g();
#ifdef __cplusplus
};
#endif


#endif
