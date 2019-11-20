#ifndef __WWK_STR__H
#define __WWK_STR__H
#include "wwk_type.h"
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct wwk_string
{
	char* data;
	int len;
}wwk_string_t;

#define is_hex(c) (((c)>='0' && (c)<='9')||((c)>='A' && (c)<='F') || ((c)>='a' && (c)<='f'))
#define wwk_string(s) {s,sizeof(s)-1}
#define wwk_string_dup(s) wwk_string_dup_data(s,sizeof(s))
#define wwk_string_end(s) ((s)->data+(s)->len)
#define wwk_string_set(str,d,bytes) (str)->data=d;(str)->len=bytes;
#define wwk_string_set_s(str,data) wwk_string_set(str,data,sizeof(data)-1)
#define wwk_string_cmp_s(str,s) wwk_string_cmp(str,s,sizeof(s)-1)
#define wwk_string_cmp_s_withstart(str,s) wwk_string_cmp_withstart(str,s,sizeof(s)-1)
#define wwk_string_equal_s(str,s) (((str)->len == (sizeof(s)-1)) && (strncmp((str)->data,s,sizeof(s)-1)==0))
#define wwk_str_equal(s1,ns1,s2,ns2) ( (ns1==ns2) && (strncmp(s1,s2,ns1)==0))
#define wwk_str_equal_s(s1,ns1,s2) wwk_str_equal(s1,ns1,s2,sizeof(s2)-1)
#define wwk_string_equal(str1,str2) (((str1)->len==(str2)->len) && (strncmp((str1)->data,(str2)->data,(str1)->len)==0))
#define ngx_str3_cmp(m,c0,c1,c2) \
	(m[0] == c0 && m[1]==c1 && m[2] == c2)

#define ngx_str4_cmp(m,c0,c1,c2,c3) \
	(m[0] == c0 && m[1]==c1 && m[2] == c2 && m[3]==c3)

#define wwk_string_print(s) printf("%.*s\n",(s)->len,(s)->data)
#define wwk_string_str_s(str,s) wwk_string_str(str,s,sizeof(s)-1)
#define wwk_str_str_s(str,len,s) wwk_str_str(str,len,s,sizeof(s)-1)
#define wwk_str_end_with_s(data,len,suf) wwk_str_end_with(data,len,suf,sizeof(suf)-1)
#define wwk_str_start_with_s(data,len,suf) wwk_str_start_with(data,len,suf,sizeof(suf)-1)
#define wwk_string_cmp_withstart_s(s,data) wwk_string_cmp_withstart(s,data,sizeof(data)-1)

wwk_string_t* wwk_string_new(int len);
int wwk_string_delete(wwk_string_t *s);
wwk_string_t* wwk_string_dup_data(char* data,int len);
wwk_string_t* wwk_string_dup_data_pad0(char* data,int len);
char* wwk_string_to_str(wwk_string_t *s);
int wwk_string_cmp_withstart(wwk_string_t* str, char*s, int bytes);
int wwk_str_end_with(char *data,int len,char *suf,int suf_bytes);
int wwk_str_start_with(char *data,int len,char *suf,int suf_bytes);
int wwk_string_cmp(wwk_string_t *str,char* s,int bytes);
int wwk_data_cmp(char *str,int str_bytes,char* s,int bytes);
int wwk_string_cmp2(wwk_string_t *str1,wwk_string_t *str2);
int wwk_string_is_char_in(wwk_string_t *s,char c);
int wwk_string_array_has(wwk_string_t **strs,int n,wwk_string_t *s);
unsigned int wwk_string_to_ord(wwk_string_t *str);
char* wwk_str_dup(const char* str);
int wwk_string_char_count(wwk_string_t *str,char c);
int wwk_str_char_count(const char *s,char c);

/**
 *	@brief like stdlib:atoi
 */
long long wwk_str_atoi(char* s,int len);

/**
 * @brief like stdlib:atof
 */
double wwk_str_atof(char *s,int len);

char* wwk_data_dup(const char* data,size_t data_len,size_t alloc_len);
char* wwk_data_dup2(char *data,int bytes);
char* wwk_data_to_str(char *data,int len);
void print_hex(char *data,int len);
void print_char(unsigned char *data,int len);

/**
 * @brief print data in readable format;
 */
void print_data(char* data, int len);
void print_short(short *f,int len);
void print_short2(short *f,int len);
void print_int(int *f,int len);
void print_char2(signed char *f,int len);
void print_uchar(unsigned char *f,int len);
void print_data_f(FILE* f,char* data, int len);
void print_data_f2(FILE* f,char* data, int len,int cn);
int zero_dispose(void *data);
char* str_merge(char* arg1,...);
char *wwk_str_found(char *src,char *key,int key_bytes);

/**
 * find the first occurrence of the sub string in src.
 */
int wwk_string_str(wwk_string_t *str,char *sub,int bytes);
char* wwk_str_chr(char* s,int slen,char c);
char* wwk_str_rchr(char *s,int len,char c);
int wwk_str_str(char *src,int src_len,char *sub,int bytes);
int wwk_char_to_hex(char c);
uint32_t hash_string_value(char* name);
uint32_t hash_string(char* name, uint32_t hash_size);
//uint32_t hash_string_value_len(char* data,int len,int hash_size);

uint32_t hash_string_value_len_seed(unsigned char* p,int len,int hash_size);

#define hash_string_value_len(p,len,slot) hash_string_value_len_seed((unsigned char*)p,len,slot)

char* wwk_itoa(int n);


//---------------------- string util section --------------------
typedef void (*wwk_str_split_f)(void *ths,char *item,int len,int index);
typedef int (*wwk_str_split_is_sep_f)(void *ths,char c);

int wwk_str_split(char *data,int len,char sep,void *split_ths,wwk_str_split_f split);
int wwk_str_split2(char *data,int len,void *split_ths,wwk_str_split_f split,wwk_str_split_is_sep_f is_sep);

void print_float(float *f,int len);
void print_float2(float *f,int len);
void print_double(double *f,int len);
void print_double2(double *f,int len);
void print_double3(double *f,int len);
int wwk_chnstr_atoi(char *data,int len,int *left);
int wwk_chnstr_atoi2(char *data,int len);
int wwk_chnstr_atoi3(char *s,int len);
int wwk_chnstr_atoi4(char *s,int len);
int wwk_str_atoi2(char *data,int len,int *left);
int wwk_is_all_digit(char *data,int len);
int wwk_num_get_unit(int num);
wwk_string_t* wwk_chnstr_itoa(int v);

//-------------------- string format parser --------------------------
typedef void(*wwk_str_attr_f)(void *ths,wwk_string_t *k,wwk_string_t *v);
/**
 * [a=1,b,c=d]
 */
int wwk_str_attr_parse(char *data,int bytes,void *ths,wwk_str_attr_f attr_notify);
int wwk_str_attr_parse2(char *data,int bytes,void *ths,wwk_str_attr_f attr_notify,int *consume);
void print_hex2(char *data,int len);

typedef void(*wwk_str_chnwrd_f)(void *ths,char *data,int len);
void wwk_str_parse_chnwrd(char *data,int len,void *ths,wwk_str_chnwrd_f wrd);

typedef struct
{
	char *s;
	char *e;
}wwk_str_chnwrd_iter_t;

void wwk_str_chnwrd_iter_init(wwk_str_chnwrd_iter_t *iter,char *data,int len);
wwk_string_t wwk_str_chnwrd_iter_next(wwk_str_chnwrd_iter_t *iter);

typedef struct
{
	char *s;
	char *e;
}wwk_str_spwrd_iter_t;
void wwk_str_spwrd_iter_init(wwk_str_spwrd_iter_t *iter,char *data,int len);
wwk_string_t wwk_str_spwrd_iter_next(wwk_str_spwrd_iter_t *iter);
void float_nan_check(float *f,int len);

void wwk_float_print2(float **f,int n1,int n2);


#ifdef __cplusplus
};
#endif
#endif
