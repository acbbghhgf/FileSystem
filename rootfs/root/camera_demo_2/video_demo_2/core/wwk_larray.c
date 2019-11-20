/*************************************************************************
	> File Name: core/wwk_larray.c
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Nov 2019 06:51:36 PM PST
 ************************************************************************/

#include "wwk_larray.h"

wwk_larray_t* wwk_larray_new(uint32_t n,uint32_t size)
{
	wwk_larray_t* a;

	a=(wwk_larray_t*)wwk_malloc(sizeof(*a));
	a->slot_alloc=n;
	a->slot_size=size;
	a->nslot=0;
	a->slot=wwk_calloc(n,size);
	return a;
}

int wwk_larray_delete(wwk_larray_t* a)
{
	wwk_free(a->slot);
	wwk_free(a);
	return 0;
}

int wwk_larray_bytes(wwk_larray_t *a)
{
	int bytes=sizeof(wwk_larray_t);

	bytes+=a->slot_size*a->slot_alloc;
	return bytes;
}

wwk_larray_t* wwk_larray_dup(wwk_larray_t *src)
{
	wwk_larray_t* dst;

	dst=wwk_larray_new(src->nslot,src->slot_size);
	dst->nslot=src->nslot;
	memcpy(dst->slot,src->slot,src->nslot*src->slot_size);
	return dst;
}

void wwk_larray_cpy(wwk_larray_t *src,wwk_larray_t *dst)
{
	wwk_larray_reset(dst);
	if(dst->slot_alloc<src->slot_alloc)
	{
		wwk_larray_reset2(dst,src->slot_alloc);
	}
	memcpy(dst->slot,src->slot,src->nslot*src->slot_size);
	dst->nslot=src->nslot;
}

void wwk_larray_merge(wwk_larray_t *merged,wwk_larray_t *pad)
{
	void *dst;

	dst=wwk_larray_push_n(merged,pad->nslot);
	//print_data(dst,4);
	memcpy(dst,pad->slot,pad->slot_size*pad->nslot);
}

void wwk_larray_reset(wwk_larray_t *a)
{
	a->nslot=0;
}

void wwk_larray_reset2(wwk_larray_t *a,int n)
{
	if(a->slot_alloc!=n)
	{
		wwk_free(a->slot);
		a->slot=wwk_calloc(n,a->slot_size);
		a->slot_alloc=n;
	}
	a->nslot=0;
}

void* wwk_larray_push(wwk_larray_t* a)
{
	return wwk_larray_push_n(a,1);
}

void* wwk_larray_get(wwk_larray_t *a,int idx)
{
	//wwk_debug("[%p]=%p\n",a->slot,((char*)a->slot)+idx*a->slot_size);
	return (void*)(((char*)a->slot)+idx*a->slot_size);
}

void wwk_larray_push2(wwk_larray_t *a,void *src)
{
	void *dst;

	dst=wwk_larray_push_n(a,1);
	//print_data(dst,4);
	memcpy(dst,src,a->slot_size);
	//print_data(dst,4);
	//wwk_debug("src:%p,%d\n",src,a->slot_size);
}

void* wwk_larray_push_n(wwk_larray_t* a,uint32_t n)
{
	uint32_t alloc;
	void *s;

	if(a->nslot+n > a->slot_alloc)
	{
		alloc=2*max(n,a->slot_alloc);
		s=wwk_calloc(alloc,a->slot_size);
		memcpy(s,a->slot,a->slot_size*a->nslot);
		wwk_free(a->slot);
		a->slot=s;
		a->slot_alloc=alloc;
	}
	s=(char*)a->slot+a->slot_size*a->nslot;
	a->nslot+=n;
	return s;
}

wwk_flta_t* wwk_flta_new(int n)
{
	wwk_flta_t *f;

	f=(wwk_flta_t*)wwk_malloc(sizeof(wwk_flta_t));
	f->len=n;
	f->p=(float*)wwk_malloc(sizeof(float)*n);
	f->pos=0;
	wwk_flta_zero(f);
	return f;
}

void wwk_flta_delete(wwk_flta_t *a)
{
	wwk_free(a->p);
	wwk_free(a);
}

void wwk_flta_reset(wwk_flta_t *a)
{
	a->pos=0;
}

void wwk_flta_zero(wwk_flta_t *a)
{
	memset(a->p,0,a->len*sizeof(float));
}


//------------------------------- test/examle section ------------------
void wwk_larray_test_g()
{
	wwk_larray_t *a;

	a=wwk_larray_new(10,sizeof(int));
	*((int*)wwk_larray_push(a))=10;
	wwk_larray_delete(a);
}

