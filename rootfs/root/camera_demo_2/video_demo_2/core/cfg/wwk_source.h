/*************************************************************************
	> File Name: wwk_source.h
	> Author: 
	> Mail: 
	> Created Time: Fri 15 Nov 2019 06:49:18 PM PST
 ************************************************************************/

#ifndef _WWK_SOURCE_H
#define _WWK_SOURCE_H

#include "core/wwk_str.h"
#include "core/wwk_strbuf.h"
#include "core/wwk_str_encode.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct wwk_source wwk_source_t;
typedef struct wwk_source_loader wwk_source_loader_t;
typedef int (*wwk_source_get_handler_t)(void*);
typedef int (*wwk_source_get_str_f)(void*,char *buf,int bytes);
typedef int (*wwk_source_read_str_f)(void*,wwk_strbuf_t *buf);
typedef wwk_string_t* (*wwk_source_get_file_f)(void*);
typedef int (*wwk_soure_get_file_size_f)(void*);
typedef int (*wwk_source_unget_handler_t)(void*,int);
typedef int (*wwk_source_load_handler_t)(void *data_ths,wwk_source_t *s);
typedef int (*wwk_source_loader_v_t)(void* inst,void *data,wwk_source_load_handler_t loader,char *fn);
#define wwk_source_get(s) ((s)->get((s)->data))
//#define wwk_source_get(s) wwk_source_read_char(s)
#define wwk_source_unget(s,c) ((s)->unget((s)->data,c))
#define wwk_source_seek_to_s(s,d) wwk_source_seek_to(s,d,sizeof(d)-1)
#define wwk_source_seek_to2_s(s,d,b) wwk_source_seek_to2(s,d,sizeof(d)-1,b)

typedef struct
{
	FILE *f;
	unsigned char *buf;
	unsigned char *valid;
	unsigned char *cur;
	unsigned eof:1;
	int alloc;
}wwk_source_file_item_t;

struct wwk_source
{
	wwk_source_get_handler_t get;
	wwk_source_unget_handler_t unget;
	wwk_source_get_str_f get_str;
	wwk_source_read_str_f read_str;
	wwk_source_get_file_f get_file;
	//wwk_soure_get_file_size_f get_filesize;
	void *data;
	unsigned char swap:1;
	//wwk_strbuf_t *buf;
};

struct wwk_source_loader
{
	void *hook;
	wwk_source_loader_v_t vf;
};

void wwk_source_init(wwk_source_t *src);
void wwk_swap_short(short *p);
void wwk_swap_int32(int *p);
int wwk_is_little_endian();
int wwk_file_write_float(FILE *file,float *f,int n,int bin,int swap);
int wwk_source_init_file(wwk_source_t *s,char *fn);
int wwk_source_clean_file(wwk_source_t *s);
int wwk_source_init_file2(wwk_source_t *s,char *fn);
int wwk_source_clean_file2(wwk_source_t *s);
int wwk_source_init_str(wwk_source_t *s,const char *data,int bytes);
void wwk_source_set_str(wwk_source_t *s,const char *data,int bytes);
int wwk_source_clean_str(wwk_source_t *s);


int wwk_source_init_fd(wwk_source_t *s,FILE *f,int pos);
int wwk_source_clean_fd(wwk_source_t *s);

/**
 * @brief input buf b will be reset
 */
int wwk_source_read_string(wwk_source_t *s,wwk_strbuf_t *b);

/**
 * @brief input buf b will not be reset
 */
int wwk_source_read_string2(wwk_source_t *s,wwk_strbuf_t *b);
int wwk_source_read_string3(wwk_source_t *s,wwk_strbuf_t *b);

int wwk_source_read_normal_string(wwk_source_t *s,wwk_strbuf_t *b);

int wwk_source_read_wwkstr(wwk_source_t *s,wwk_strbuf_t *b);
int wwk_source_read_wwkstr2(wwk_source_t *s,wwk_strbuf_t *b,int bi);
int wwk_source_read_line(wwk_source_t *s,wwk_strbuf_t *b);
int wwk_source_read_line2(wwk_source_t *s,wwk_strbuf_t *b,int *eof);
int wwk_source_skip_sp(wwk_source_t *s,int *nl);
int wwk_source_skip_sp2(wwk_source_t *s,int *nl,int *eof);
int wwk_source_skip_sp3(wwk_source_t *s,int *nl);
int wwk_source_peek(wwk_source_t *s);
int wwk_source_fill(wwk_source_t* s,char* data,int len);
int wwk_source_read_short(wwk_source_t* s,short* v,int n,int bin);
int wwk_source_read_ushort(wwk_source_t* s,unsigned short* v,int n,int bin);
int wwk_source_read_int(wwk_source_t *s,int* v,int n,int bin);
int wwk_source_atof(wwk_source_t* s,double *v);
int wwk_source_atoi(wwk_source_t* s,int* value);
int wwk_source_read_double(wwk_source_t *s,double *f,int n);
int wwk_source_read_float(wwk_source_t *s,float *f,int n,int bin);
int wwk_source_read_char(wwk_source_t *s);
int wwk_source_read_utf8_char(wwk_source_t *s,wwk_strbuf_t *buf);
int wwk_source_seek_to(wwk_source_t *s,char *data,int len);
int wwk_source_seek_to2(wwk_source_t *src,char *data,int len,wwk_strbuf_t *buf);
int wwk_source_load_file(void *data,wwk_source_load_handler_t loader,char *fn);
int wwk_source_load_file_v(void *hook,void *data,wwk_source_load_handler_t loader,char *fn);
void wwk_source_loader_init_file(wwk_source_loader_t *sl);
int wwk_source_loader_load(wwk_source_loader_t *l,void *data_ths,wwk_source_load_handler_t loader,char *fn);
int wwk_source_loader_file_lines(wwk_source_loader_t *sl,char *fn);
wwk_string_t* wwk_source_read_file(wwk_source_t *src);
void wwk_source_read_file2(wwk_source_t *src,wwk_strbuf_t *buf);

float* wwk_file_read_float(char *fn,int *n);
#ifdef __cplusplus
};
#endif
#endif
