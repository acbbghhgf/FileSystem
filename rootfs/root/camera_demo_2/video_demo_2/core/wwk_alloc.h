/*************************************************************************
	> File Name: wwk_alloc.h
	> Author: 
	> Mail: 
	> Created Time: Mon 11 Nov 2019 10:25:01 PM PST
 ************************************************************************/

#ifndef _WWK_ALLOC_H
#define _WWK_ALLOC_H
#include <stdlib.h>
#include "wwk_type.h"
#ifdef USE_MALLOC_H
#include <malloc.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif
#define MEM_INLINE

#define wwk_ALIGNMENT   ((int)(sizeof(unsigned long)))    /* platform word */
#define wwk_ALIGNMENT_BIT (wwk_ALIGNMENT<<3)
#ifdef WIN32
#define wwk_align_ptr(ptr, align) \
	((align)>0 ? (void*)(((uintptr_t)(ptr)+(uintptr_t)(align - 1)) & (~((uintptr_t)(align - 1)))) : ptr)
#else
#define wwk_align_ptr(ptr, align) \
	((align)>0 ? (void*)(((unsigned long)(ptr)+(align - 1)) & (~(align - 1))) : ptr)
#endif
    /*
#define wwk_align_ptr(p, a) \
	(uint8_t*)(((unsigned int)(p)+(a-1)) & (~(a-1)))
    */

#define wwk_round(size,align) \
 ((align)>0 ? (((size)+((align)-1))&(~((align)-1))) : size)

#define wwk_round_8(size) ((((size)&7)==0)? (size) : ((size)+8-((size)&7)))
#define wwk_round_16(size) ((((size)&15)==0)? (size) : ((size)+16-((size)&15)))

//#define wwk_round_word(size) ((((size)&(wwk_ALIGNMENT_BIT-1))==0)? (size) : ((size)+wwk_ALIGNMENT_BIT-((size)&(wwk_ALIGNMENT_BIT-1))))
#define wwk_round_word(size) wwk_round_16(size)
//for old version
#define wwk_round8(size) wwk_round_8(size)

//#define DEBUG_MEM

#ifdef MEM_INLINE
#ifdef DEBUG_MEM
#define wwk_free(p)		wwk_free_debug(p,__FUNCTION__,__LINE__)
#define wwk_malloc(n)		wwk_malloc_debug(n,__FUNCTION__,__LINE__)
#define wwk_calloc(nmem,size) wwk_calloc_debug(nmem,size,__FUNCTION__,__LINE__)
#else
#define wwk_free(p)		free(p)
#define wwk_malloc(n)		malloc(n)
#define wwk_calloc(nmem,size) calloc(nmem,size)
#endif
#else
void wwk_free(void* p);
void* wwk_malloc(size_t n);
void* wwk_calloc(int elems,int size);
#endif
char* wwk_data_cpy(char *data,int len);
void* wwk_memalign(size_t alignment,size_t size);
void print_data(char* data, int len);
#ifdef DEBUG_MEM
void* wwk_calloc_debug(int elems,int size,const char *f,int line);
void* wwk_malloc_debug(size_t n,const char *f,int line);
void wwk_free_debug(void *p,const char *f,int line);
#endif
#ifdef __cplusplus
};
#endif

#endif
