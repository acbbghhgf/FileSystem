/*************************************************************************
	> File Name: wwk_strbuf.h
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Nov 2019 06:18:13 PM PST
 ************************************************************************/

#ifndef _WWK_STRBUF_H
#define _WWK_STRBUF_H

#include "wwk_type.h"
#include "wwk_str.h"
#include "wwk_str_encode.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct wwk_strbuf wwk_strbuf_t;
#define wwk_strbuf_push_s(b,s) wwk_strbuf_push(b,s,sizeof(s)-1)
#define wwk_strbuf_push_s0(b,s) wwk_strbuf_push(b,s,sizeof(s))
#define wwk_strbuf_push_string(b,s) wwk_strbuf_push(b,s,strlen(s))
#define wwk_strbuf_reset(b) ((b)->pos=0)
#define wwk_strbuf_push_front_s(b,s) wwk_strbuf_push_front(b,s,sizeof(s)-1)
//#define wwk_strbuf_replace_s(buf,src,src_len,needle) wwk_strbuf_replace(buf,src,src_len,needle,sizeof(needle)-1)
#define BUF_USE_MACRO
#ifndef BUF_USE_MACRO
void wwk_strbuf_push_c(wwk_strbuf_t *s,char b);
#else
#define wwk_strbuf_push_c(buf,b) \
{\
	    if(buf->length<=buf->pos) \
	    {\
	        wwk_strbuf_expand(buf,1); \
	    }\
	    buf->data[buf->pos++]=b; \
}
#endif

struct wwk_strbuf
{
    char *data;					//raw data;
    int pos;					//valid data size;
    int length;					//memory size of raw data;
    float rate;					//memory increase rate;
};

/**
 * @brief create string buffer;
 */
wwk_strbuf_t* wwk_strbuf_new(int init_len,float rate);

/**
 * @brief delete string buffer;
 */
int wwk_strbuf_delete(wwk_strbuf_t* b);

/**
 * @brief push data to the end of buffer;
 */
void wwk_strbuf_push(wwk_strbuf_t *s,const char *buf,int bytes);

/*
 * @brief push
 */
void wwk_strbuf_push_word(wwk_strbuf_t *buf,char *data,int bytes);

/**
 * @brief push data by fmt like printf,this is simple util function.
 */
void wwk_strbuf_push_f(wwk_strbuf_t *b,const char *fmt,...);

/**
 * @brief push data to the font of buffer;
 */
void wwk_strbuf_push_front(wwk_strbuf_t *s,const char *buf,int bytes);

/**
 * @brief pop data from the front of buffer;
 */
int wwk_strbuf_pop(wwk_strbuf_t *s,char* data,int bytes);

void wwk_strbuf_replace(wwk_strbuf_t *buf,char *data,int data_len,char *src,int src_len,char *dst,int dst_len);
//void wwk_strbuf_reset(wwk_strbuf_t *s);
void wwk_strbuf_print(wwk_strbuf_t *s);
void wwk_strbuf_merge(wwk_strbuf_t *buf,wwk_string_t *p1,...);
void wwk_strbuf_merge2(wwk_strbuf_t *buf,char *p1,...);
int wwk_strbuf_read(wwk_strbuf_t *buf,char *fn);
int wwk_strbuf_read2(wwk_strbuf_t *buf,char *fn);
void wwk_strbuf_expand(wwk_strbuf_t *s,int bytes);
char* wwk_strbuf_to_str(wwk_strbuf_t *s);
void wwk_strbuf_strip(wwk_strbuf_t *buf);
void wwk_strbuf_pad0(wwk_strbuf_t *buf,int bytes);
int wwk_strbuf_bytes(wwk_strbuf_t *b);

/**
 * @brief skip white space \t
 */
void wwk_strbuf_push_skip_ws(wwk_strbuf_t *b,char *data,int len);

/**
 * @brief skip ws between chinese;  中文 see you 你好=> 中文see you你好
 */
void wwk_strbuf_push_skip_utf8_ws(wwk_strbuf_t *buf,char *data,int len);

/**
 * @brief push string and pad 0 to data;
 */
void wwk_strbuf_string_to_str(wwk_strbuf_t *buf,char *data,int bytes);

void wwk_strbuf_push_add_escape_str(wwk_strbuf_t *buf,char *data,int bytes);

int wwk_strbuf_itoa(wwk_strbuf_t *buf,int v);
void wwk_strbuf_atochn(wwk_strbuf_t *buf,char *data,int len);
int wwk_strbuf_atochn2(wwk_strbuf_t *buf,char *data,int len);


/**
 *  parse; "hello\n\rbcd"
 */
void wwk_strbuf_parse_quote(wwk_strbuf_t *buf,char *data,int bytes);

void wwk_itochn(wwk_strbuf_t *buf,int v);
void wwk_stochn(wwk_strbuf_t *buf,char *data,int len);
void wwk_stotel(wwk_strbuf_t *buf,char *data,int len);
void wwk_strbuf_real_path(wwk_strbuf_t *buf,char *name,int len);

void wwk_strbuf_control_cache(wwk_strbuf_t *buf,int max_cache);
#ifdef __cplusplus
};
#endif


#endif
