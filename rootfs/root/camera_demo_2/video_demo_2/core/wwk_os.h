/*************************************************************************
	> File Name: wwk_os.h
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Nov 2019 06:55:47 PM PST
 ************************************************************************/

#ifndef _WWK_OS_H
#define _WWK_OS_H

#include <stdio.h>
#include "wwk_type.h"
#include "wwk_str.h"
#include "wwk_strbuf.h"
#ifdef __cplusplus
extern "C" {
#endif
#ifdef WIN32
#include <stdlib.h>
#ifdef WINCE
#include <ceconfig.h>
#else
#include <direct.h>
#include <io.h>
#endif

#define F_OK 0
#define R_OK 4
#define W_OK 2
#define wwk_file_access(fn,m) _access(fn,m)
#define wwk_file_exist(fn) wwk_file_access(fn,F_OK)
#define wwk_file_readable(fn) wwk_file_access(fn,R_OK)
#define wwk_file_writeable(fn) wwk_file_access(fn,W_OK)
typedef int (*wwk_dir_walk_handler_t)(void* ,char *fn);
//void dir_monitor(const char* path,  dir_monitor_handler cb,void* user_data,HANDLE* handle);
int wwk_dir_walk(const char* path,wwk_dir_walk_handler_t cb,void* user_data);

int wwk_is_dir(char *dn);
#else
/**
 * @brief return 0 on success.
 */
#define wwk_file_access(fn,m) access(fn,m)
#define wwk_file_exist(fn) wwk_file_access(fn,F_OK)
#define wwk_file_readable(fn) wwk_file_access(fn,R_OK)
#define wwk_file_writeable(fn) wwk_file_access(fn,W_OK)
#define wwk_file_exeable(fn) wwk_file_access(fn,X_OK)
int wwk_is_dir(char *dn);
#endif
#ifdef WIN32
#define DIR_SEP '\\'
#define DIR_SEP1 '/'
#else
#define DIR_SEP '/'
#endif

typedef int(*wwk_dir_walk_f)(void *ths,wwk_string_t *fn);
#if defined __IPHONE_OS__
#else
int wwk_dir_walk(char *dir,void *ths,wwk_dir_walk_f walk);

typedef int(*wwk_dir_walk2_f)(void *ths,char *fn);
int wwk_dir_walk2(char *dir,void *ths,wwk_dir_walk2_f walk);
#endif



//------------------------ directory and file section ---------------------------
/**
 * @brief create directory;
 */
int wwk_mkdir(char* dn);

/**
 * @brief create directory and parent directory if not created;
 */
int wwk_mkdir_p(char* fn,char sep,int create_last_entry);

/**
 * @brief remove . and .. from fn, and save the result into buf;
 * @param fn like /home/lz123/wwk/wvite/httpa/8.9/src/../ext/../html to remove ..
 * @param fn_bytes bytes of fn;
 * @param buf to save the result;
 * @param sep use to separate directory: '/','\';
 */
void wwk_real_fn(char *fn,int fn_bytes,wwk_strbuf_t *buf,char sep);
#define wwk_real_fn_s(fn,buf,sep) wwk_real_fn(fn,sizeof(fn)-1,buf,sep)


/**
 * @directory name for fn;
 */
wwk_string_t* wwk_dir_name(char *fn,char sep);

wwk_string_t wwk_dir_name2(char *data,int len,char sep);

/**
 * @brief realpath of fn;
 * @return char* must be freed;
 */
char* wwk_realpath(char *fn,char *buf);

wwk_string_t* wwk_dirname(char *fn,char sep);
wwk_string_t* wwk_str_left(char *fn,int len,char sep);
wwk_string_t* wwk_basename(char* fn,char sep);
wwk_string_t* wwk_str_right(char* fn,int len,char sep);
wwk_string_t* wwk_real_dirname(char *fn);
FILE* wwk_file_open(char* fn,char * mode);

//------------------------ file section ----------------------------------------
uint64_t file_length(FILE *f);
uint64_t wwk_file_size(char *fn);

char* file_read_buf(char* fn, int *n);
char* file_read_buf2(char* fn, int fn_len,int *n);
int file_write_buf(char* fn, const char* data, size_t len);

//------------------------ time section ----------------------------------------
double time_get_ms();
double time_get_cpu();
void wwk_msleep(int ms);

//----------------------- compile section --------------------------------------
int wwk_gcc_month();
int wwk_gcc_day();
int wwk_gcc_year();
int wwk_get_build_timestamp(char *buf);
int wwk_os_timestamp(char *buf);
int wwk_os_timestamp2(wwk_strbuf_t *buf);

//------------------------- --------------------------------------------------
char* wwk_search_file(char *fn,wwk_string_t **path,int n,wwk_strbuf_t *buf);
unsigned long long  wwk_file_lines(char *fn);
int wwk_file_read_line(FILE *f,wwk_strbuf_t *buf,unsigned long long index,char *tmp,int tmp_size);
int wwk_file_read_line2(FILE *f,wwk_strbuf_t *buf,unsigned long long index);

int wwk_file_copy(char *src,char *dst,char sep);
int wwk_file_copy2(FILE *fin,char *dst,char sep,int want_len);

//[left,right];
int wwk_random(int left,int right);


typedef void(*wwk_os_dir_walk_notify_f)(void *ths,char *name,int len);
int wwk_os_dir_walk(char *dir,void *ths,wwk_os_dir_walk_notify_f notify);

#ifdef __cplusplus
};
#endif


#endif
