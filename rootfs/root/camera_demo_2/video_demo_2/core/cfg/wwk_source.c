/*************************************************************************
	> File Name: wwk_source.c
	> Author: 
	> Mail: 
	> Created Time: Fri 15 Nov 2019 06:58:19 PM PST
 ************************************************************************/


#include "core/cfg/wwk_source.h"
#include <ctype.h>
#include <math.h>
#include <float.h>
#define SING_QUOTE '\''
#define DBL_QUOTE '"'
#define ESCAPE_CHAR '\\'

/*
*swap first char and second char of short value.
*byte1 byte2 --translate to->byte2 byte1
*/
//void SwapShort(short *p)

void wwk_swap_short(short *p)
{
	char temp,*q;

	q = (char*) p;
	temp = *q; *q = *(q+1); *(q+1) = temp;
}

/*
*	byte1 byte2 byte3 byte4
*	translate->
*	byte4 byte3 byte2 byte1
*/
//void SwapInt32(int *p)
void wwk_swap_int32_x(int *p)
{
	char temp,*q;

	q = (char*) p;
	temp = *q; *q = *(q+3); *(q+3) = temp;
	temp = *(q+1); *(q+1) = *(q+2); *(q+2) = temp;
}

void wwk_swap_int32(int *i)
{
	register char c;
	register char *p;

	p=(char*)i;
	c=p[0];
	p[0]=p[3];
	p[3]=c;

	c=p[1];
	p[1]=p[2];
	p[2]=c;
}



/* check big endian or little endian.*/
//return true- little endian, false- big endian 
int wwk_is_little_endian()
{
	/*
	int d=0x100;
	return (*(char*)&d)==(char)d;
	*/
	short x, *px;
	unsigned char *pc;

	px = &x;
	pc = (unsigned char *) px;
	*pc = 1; *(pc+1) = 0;
	return x==1;
}

wwk_source_file_item_t* wwk_source_file_item_new(FILE *f)
{
	wwk_source_file_item_t *item;

	item=(wwk_source_file_item_t*)wwk_malloc(sizeof(wwk_source_file_item_t));
	item->f=f;
	item->alloc=4096;
	item->buf=(unsigned char*)wwk_malloc(item->alloc);
	item->cur=0;
	item->valid=0;

	item->eof=0;
	return item;
}

void wwk_source_file_item_delete(wwk_source_file_item_t *item)
{
	if(item->f)
	{
		fclose(item->f);
	}
	wwk_free(item->buf);
	wwk_free(item);
}

int wwk_source_file_item_get(wwk_source_file_item_t *item)
{
	int len;

	//wwk_debug("index=%d/%d eof=%d\n",item->index,item->len,item->eof);
	if(item->cur==item->valid)
	{
		if(item->eof)
		{
			return EOF;
		}
		item->cur=item->buf;
		len=fread(item->buf,1,item->alloc,item->f);
		item->valid=item->cur+len;
		if(len<item->alloc)
		{
			item->eof=1;
		}
		if(len<=0)
		{
			item->eof=1;
			return EOF;
		}
	}
	//printf("%c",*item->cur);
	return *(item->cur++);
}

int wwk_source_file_item_get_buf(wwk_source_file_item_t *item,char *buf,int bytes)
{
	int len;
	int ret;

	len=item->valid-item->cur;
	if(len>=bytes)
	{
		memcpy(buf,item->cur,bytes);
		item->cur+=bytes;
		ret=bytes;
	}else
	{
		if(len>0)
		{
			memcpy(buf,item->cur,len);
			item->cur=item->valid;
			buf+=len;
			bytes-=len;
		}
		ret=wwk_source_file_item_get(item);
		if(ret==EOF){return EOF;}
		--item->cur;
		ret=wwk_source_file_item_get_buf(item,buf,bytes);
		if(ret==EOF){return EOF;}
		ret+=len;
	}
	return ret;
}

int wwk_source_file_item_unget(wwk_source_file_item_t* item,int c)
{
	//wwk_debug("index=%d\n",c);
	if(item->cur>item->buf)
	{
		--item->cur;
		*item->cur=c;
	}else
	{
		wwk_debug("unget\n");
		exit(0);
		ungetc(c,item->f);
	}
	return 0;
}

void wwk_source_init(wwk_source_t *src)
{
	src->data=NULL;
	src->read_str=NULL;
	src->get=NULL;
	src->get_str=NULL;
	src->unget=NULL;
	src->get_file=NULL;
	//src->get_filesize=NULL;
	src->swap=wwk_is_little_endian();
}

int wwk_source_init_fd(wwk_source_t *s,FILE *f,int pos)
{
	wwk_source_file_item_t *item;
	int ret;

	ret=fseek(f,pos,SEEK_SET);
	if(ret!=0)
	{
		wwk_debug("seek failed %d\n",pos);
		goto end;
	}
	item=wwk_source_file_item_new(f);
	wwk_source_init(s);
	s->data=item;
	s->get=(wwk_source_get_handler_t)wwk_source_file_item_get;
	s->unget=(wwk_source_unget_handler_t)wwk_source_file_item_unget;
	s->get_str=(wwk_source_get_str_f)wwk_source_file_item_get_buf;
	s->swap=wwk_is_little_endian();
	ret=0;
end:
	return ret;
}

int wwk_source_clean_fd(wwk_source_t *s)
{
	//wwk_strbuf_delete(s->buf);
	if(s->data)
	{
		((wwk_source_file_item_t*)s->data)->f=NULL;
		wwk_source_file_item_delete((wwk_source_file_item_t*)s->data);
	}
	return 0;
}

int wwk_source_init_file(wwk_source_t* s,char *fn)
{
	FILE* f;
	int ret;
	wwk_source_file_item_t *item;

	ret=-1;
	f=fopen(fn,"rb");
	if(!f){s->data=0;goto end;}
	item=wwk_source_file_item_new(f);
	wwk_source_init(s);
	s->data=item;
	s->get=(wwk_source_get_handler_t)wwk_source_file_item_get;
	s->unget=(wwk_source_unget_handler_t)wwk_source_file_item_unget;
	s->get_str=(wwk_source_get_str_f)wwk_source_file_item_get_buf;
	s->swap=wwk_is_little_endian();
	ret=0;
end:
	return ret;
}

int wwk_source_clean_file(wwk_source_t *s)
{
	//wwk_strbuf_delete(s->buf);
	if(s->data)
	{
		wwk_source_file_item_delete((wwk_source_file_item_t*)s->data);
	}
	return 0;
}

int wwk_file_unget(FILE* f,int c)
{
	return ungetc(c,f);
}

int wwk_source_init_file2(wwk_source_t* s,char *fn)
{
	FILE* f;
	int ret;

	ret=-1;
	f=fopen(fn,"rb");
	if(!f){s->data=0;goto end;}
	wwk_source_init(s);
	s->data=f;
	s->get=(wwk_source_get_handler_t)fgetc;
	s->unget=(wwk_source_unget_handler_t)wwk_file_unget;
	s->swap=wwk_is_little_endian();
	ret=0;
end:
	return ret;
}

int wwk_source_clean_file2(wwk_source_t *s)
{
	//wwk_strbuf_delete(s->buf);
	if(s->data)
	{
		fclose((FILE*)s->data);
	}
	return 0;
}



typedef struct wwk_source_str
{
	const unsigned char *data;
	int len;
	int pos;
}wwk_source_str_t;

void wwk_source_str_set(wwk_source_str_t *s,const char *data,int len)
{
	s->data=(const unsigned char*)data;
	s->len=len;
	s->pos=0;
}

wwk_source_str_t* wwk_source_str_new(const char *data,int len)
{
	wwk_source_str_t *s;

	s=(wwk_source_str_t*)wwk_malloc(sizeof(*s));
	wwk_source_str_set(s,data,len);
	return s;
}

int wwk_source_str_get(wwk_source_str_t *s)
{
	if(s->pos<s->len)
	{
		return s->data[s->pos++];
	}else
	{
		return EOF;
	}
}

int wwk_sources_str_unget(wwk_source_str_t *s,int c)
{
	if(s->pos>0 && c!=EOF)
	{
		--s->pos;
	}
	return 0;
}

int wwk_source_init_str(wwk_source_t *s,const char *data,int bytes)
{
	wwk_source_init(s);
	s->data=wwk_source_str_new(data,bytes);
	s->get=(wwk_source_get_handler_t)wwk_source_str_get;
	s->unget=(wwk_source_unget_handler_t)wwk_sources_str_unget;
	s->get_str=NULL;
	s->swap=wwk_is_little_endian();
	return 0;
}

void wwk_source_set_str(wwk_source_t *s,const char *data,int bytes)
{
	wwk_source_str_set((wwk_source_str_t*)s->data,data,bytes);
}

int wwk_source_clean_str(wwk_source_t *s)
{
	if(s->data)
	{
		wwk_free(s->data);
	}
	return 0;
}

int wwk_source_peek(wwk_source_t *s)
{
	int c;

	c=wwk_source_get(s);
	wwk_source_unget(s,c);
	return c;
}

int wwk_source_read_line(wwk_source_t *s,wwk_strbuf_t *b)
{
	int ret;
	int c;

	wwk_strbuf_reset(b);
	while(1)
	{
		c=wwk_source_get(s);
		if(c=='\n'||c==EOF){goto end;}
		wwk_strbuf_push_c(b,c);
	}
end:
	//ret=b->pos<=0?-1:0;
	ret=0;
	return ret;
}

int wwk_source_read_line2(wwk_source_t *s,wwk_strbuf_t *b,int *eof)
{
	int ret;
	int c;

	if(eof)
	{
		*eof=0;
	}
	wwk_strbuf_reset(b);
	while(1)
	{
		c=wwk_source_get(s);
		if(c=='\n'||c==EOF)
		{
			if(eof && c==EOF)
			{
				*eof=1;
			}
			goto end;
		}
		wwk_strbuf_push_c(b,c);
	}
end:
	//ret=b->pos<=0?-1:0;
	ret=0;
	return ret;
}


int wwk_source_read_string(wwk_source_t *s,wwk_strbuf_t *b)
{
	wwk_strbuf_reset(b);
	return wwk_source_read_string2(s,b);
}

int wwk_source_read_normal_string(wwk_source_t *s,wwk_strbuf_t *b)
{
	int c,ret;
	//char t;

	wwk_strbuf_reset(b);
//	if(s->read_str)
//	{
//		return s->read_str(s->data,b);
//	}
	ret=-1;
	while(isspace(c=wwk_source_get(s)));
	if(c==EOF){goto end;}
	while(1)
	{
		//wwk_debug("%d:%c\n",c,c);
		if(c==EOF){ret=0;goto end;}
		if(c==EOF||isspace(c))
		{
			wwk_source_unget(s,c);
			break;
		}
		wwk_strbuf_push_c(b,c);
		c=wwk_source_get(s);
	}
	ret=0;
end:
	return ret;
}

int wwk_source_read_string2(wwk_source_t *s,wwk_strbuf_t *b)
{
	int isq,q=0,c,ret,n,i;
	//char t;

	if(s->read_str)
	{
		return s->read_str(s->data,b);
	}
	ret=-1;
	while(isspace(c=wwk_source_get(s)));
	if(c==EOF){goto end;}
	if(c==DBL_QUOTE||c==SING_QUOTE)
	{
		isq=1;q=c;
		c=wwk_source_get(s);
	}else
	{
		isq=0;
	}
	while(1)
	{
		//wwk_debug("%d:%c\n",c,c);
		if(c==EOF){ret=0;goto end;}
		if(isq)
		{
			if(c==q){break;}
		}else
		{
			if(c==EOF||isspace(c))
			{
				wwk_source_unget(s,c);
				break;
			}
		}
		if(c==ESCAPE_CHAR)
		{
			c=wwk_source_get(s);
			if(c==EOF){goto end;}
			if(c>='0' && c<='7')
			{
				n=c-'0';
				for(i=0;i<2;++i)
				{
					c=wwk_source_get(s);
					if(c==EOF||c<'0'||c>'7'){goto end;}
					n=(n<<3)+c-'0';
				}
				c=n;
			}
		}
		//t=c;
		wwk_strbuf_push_c(b,c);
		c=wwk_source_get(s);
	}
	ret=0;
end:
	return ret;
}

int wwk_source_read_wwkstr(wwk_source_t *s,wwk_strbuf_t *b)
{
	unsigned char bi;
	int ret;

	ret=wwk_source_fill(s,(char*)&bi,1);
	if(ret!=0)
	{
		//wwk_debug("read len failed\n");
		goto end;
	}
	wwk_strbuf_reset(b);
	wwk_strbuf_expand(b,bi);
	ret=wwk_source_fill(s,b->data,bi);
	if(ret!=0)
	{
		wwk_debug("read data failed bi=%d\n",bi);
		goto end;
	}
	b->pos=bi;
end:
	return ret;
}


int wwk_source_read_wwkstr2(wwk_source_t *s,wwk_strbuf_t *b,int bi)
{
	int ret;

	wwk_strbuf_reset(b);
	wwk_strbuf_expand(b,bi);
	ret=wwk_source_fill(s,b->data,bi);
	if(ret!=0){goto end;}
	b->pos=bi;
end:
	return ret;
}

int wwk_source_read_string3(wwk_source_t *s,wwk_strbuf_t *b)
{
	int isq,q=0,c,ret,n,i;
	//char t;

	wwk_strbuf_reset(b);
	if(s->read_str)
	{
		return s->read_str(s->data,b);
	}
	ret=-1;
	while(isspace(c=wwk_source_get(s)));
	if(c==EOF){goto end;}
	if(c==DBL_QUOTE||c==SING_QUOTE)
	{
		isq=1;q=c;
		c=wwk_source_get(s);
	}else
	{
		isq=0;
	}
	while(1)
	{
		//wwk_debug("%d:%c\n",c,c);
		if(c==EOF){ret=0;goto end;}
		if(isq)
		{
			if(c==q){break;}
		}else
		{
			if(c==EOF||isspace(c)||c=='\"' ||c=='\'')
			{
				wwk_source_unget(s,c);
				break;
			}
		}
		if(c==ESCAPE_CHAR)
		{
			c=wwk_source_get(s);
			if(c==EOF){goto end;}
			if(c>='0' && c<='7')
			{
				n=c-'0';
				for(i=0;i<2;++i)
				{
					c=wwk_source_get(s);
					if(c==EOF||c<'0'||c>'7'){goto end;}
					n=(n<<3)+c-'0';
				}
				c=n;
			}
		}
		//t=c;
		wwk_strbuf_push_c(b,c);
		c=wwk_source_get(s);
	}
	ret=0;
end:
	return ret;
}

int wwk_source_skip_sp(wwk_source_t *s,int *nl)
{
	int c,ret,n;

	ret=-1;n=0;
	while(1)
	{
		c=wwk_source_get(s);
		if(c==EOF){n=1;ret=0;goto end;}
		if(!isspace(c)){break;}
		if(c=='\n')
		{
			++n;
		}
	}
	ret=0;
end:
	if(c!=EOF){wwk_source_unget(s,c);}
	if(nl){*nl=n;}
	return ret;
}

int wwk_source_skip_sp2(wwk_source_t *s,int *nl,int *eof)
{
	int c,ret,n;

	if(eof)
	{
		*eof=0;
	}
	ret=-1;n=0;
	while(1)
	{
		c=wwk_source_get(s);
		if(c==EOF)
		{
			if(eof)
			{
				*eof=1;
			}
			n=1;ret=0;
			goto end;
		}
		if(!isspace(c)){break;}
		if(c=='\n')
		{
			++n;
		}
	}
	ret=0;
end:
	if(c!=EOF){wwk_source_unget(s,c);}
	if(nl){*nl=n;}
	return ret;
}

int wwk_source_skip_sp3(wwk_source_t *s,int *nl)
{
	int c,ret,n;

	ret=-1;n=0;
	c=wwk_source_get(s);
	if(c==EOF){n=1;ret=0;goto end;}
	if(!isspace(c)){goto end;}
	if(c=='\n')
	{
		++n;
	}

	ret=0;
end:
	if(ret==-1){wwk_source_unget(s,c);}
	if(nl){*nl=n;}
	return ret;
}

int wwk_source_fill(wwk_source_t* s,char* data,int len)
{
	int ret,i;
	int c;
	unsigned char*p;

	if(s->get_str)
	{
		ret=s->get_str(s->data,data,len);
		//print_hex(data,len);
		if(ret!=len)
		{
			ret=-1;
		}else
		{
			ret=0;
		}
	}else
	{
		p=(unsigned char*)data;
		ret=0;
		for(i=0;i<len;++i)
		{
			c=wwk_source_get(s);
			//wwk_debug("c=%x\n",c);
			if(c==EOF){ret=-1;break;}
			p[i]=c;
		}
	}
	return ret;
}

int wwk_source_atoi(wwk_source_t* s,int* value)
{
	int c,ret,i;

	//wwk_debug("=============\n");
	while(1)
	{
		c=wwk_source_get(s);
		//printf("%c",c);
		if(isspace(c))
		{
			continue;
		}else
		{
			if(isdigit(c)==0)
			{
				ret=-1;goto end;
			}
			break;
		}
	}
	i=0;
	do
	{
		i=(c-'0')+i*10;
		c=wwk_source_get(s);
		//printf("%c",c);
	}while(isdigit(c));
	wwk_source_unget(s,c);
	*value=i;ret=0;
end:
	//wwk_debug("i=%d\n",i);
	return ret;
}

#include <stdarg.h>
int wwk_source_atof(wwk_source_t* s,double *v)
{
	double number;
	int exponent;
	int negative;
	double p10;
	int n;
	int num_digits;
	int num_decimals;
	int c;
	int ret;

	while(1)
	{
		c=wwk_source_get(s);
		if(isspace(c)==0){break;}
	}
	negative=0;
	switch(c)
	{
	case '-':
		negative=1;
	case '+':
		c=wwk_source_get(s);
		break;
	}
	number = 0.;
	exponent = 0;
	num_digits = 0;
	num_decimals = 0;

	// Process string of digits
	while (isdigit(c))
	{
		number = number * 10. + (c - '0');
		c=wwk_source_get(s);
		++num_digits;
	}
	if (c == '.')
	{
		c=wwk_source_get(s);
		while (isdigit(c))
		{
			number = number * 10. + (c- '0');
			c=wwk_source_get(s);
			++num_digits;
			++num_decimals;
		}
		exponent -= num_decimals;
	}
    if (num_digits == 0)
	{
    	//wwk_debug("num_digits is 0\n");
    	ret=-1;goto end;
	}
    if (negative) number = -number;
	if (c == 'e' || c == 'E')
	{
		// Handle optional sign
		negative = 0;
		c=wwk_source_get(s);
		switch(c)
		{
		case '-':
			negative = 1;   // Fall through to increment pos
		case '+':
			c=wwk_source_get(s);
			break;
		}
		n = 0;
		while (isdigit(c))
		{
			n = n * 10 + (c - '0');
			c=wwk_source_get(s);
		}
		if (negative)
				exponent -= n;
		else
				exponent += n;
	}
	if (exponent < DBL_MIN_EXP  || exponent > DBL_MAX_EXP)
    {
		//wwk_debug("read exponent failed\n");
		ret=-1;goto end;
    }

	p10 = 10.;
	n = exponent;
	if (n < 0) n = -n;
	while (n)
	{
		if (n & 1)
		{
			if (exponent < 0)
					number /= p10;
			else
					number *= p10;
		}
		n >>= 1;
		p10 *= p10;
	}
	if (number == HUGE_VAL)
	{
		//wwk_debug("read number failed\n");
		ret=-1;goto end;
	}
	wwk_source_unget(s,c);
	*v=number;
	ret=0;
end:
    return ret;
}

int wwk_source_read_int(wwk_source_t *s,int* v,int n,int bin)
{
	int ret=0,x;
	int *p,*e;

	e=v+n;
	if(bin)
	{
		ret=wwk_source_fill(s,(char*)v,sizeof(int)*n);
		if(ret!=0 || !s->swap){goto end;}
		for(p=v;p<e;++p)
		{
			wwk_swap_int32(p);
		}
	}else
	{
		for(p=v;p<e;++p)
		{
			ret=wwk_source_atoi(s,&x);
			if(ret!=0){goto end;}
			*p=x;
		}
	}
end:
	return ret;
}


int wwk_source_read_char(wwk_source_t *s)
{
	/*
	char c;

	c=(s)->get((s)->data);
	//printf("%c",c);
	return c;
	*/
	return (s)->get((s)->data);
}

int wwk_source_read_utf8_char2(wwk_source_t *s,wwk_strbuf_t *buf)
{
	char c;
	int ret;
	int i,n;

	wwk_strbuf_reset(buf);
	ret=wwk_source_get(s);
	if(ret==EOF){ret=-1;goto end;}
	c=(char)ret;
	wwk_strbuf_push_c(buf,c);
	n=wwk_utf8_bytes(c);
	i=1;
	while(i<n)
	{
		ret=wwk_source_get(s);
		if(ret==EOF){ret=-1;goto end;}
		c=(char)ret;
		wwk_strbuf_push_c(buf,c);
		++i;
	}
	ret=0;
end:
	return ret;
}

int wwk_source_read_utf8_char(wwk_source_t *s,wwk_strbuf_t *b)
{
	char bi;
	int ret;
	int len;

	wwk_strbuf_reset(b);
	ret=wwk_source_fill(s,(char*)&bi,1);
	if(ret!=0)
	{
		//wwk_debug("read len failed\n");
		goto end;
	}
	wwk_strbuf_push_c(b,bi);
	len=wwk_utf8_bytes(bi);
	if(len>1)
	{
		ret=wwk_source_fill(s,b->data+1,len-1);
		if(ret!=0)
		{
			wwk_debug("read data failed bi=%d\n",bi);
			goto end;
		}
	}
	b->pos=len;
end:
	return ret;
}


int wwk_source_read_short(wwk_source_t* s,short* v,int n,int bin)
{
	int ret=0,x;
	short *p,*e;

	e=v+n;
	if(bin)
	{
		ret=wwk_source_fill(s,(char*)v,sizeof(short)*n);
		if(ret!=0 || !s->swap){goto end;}
		for(p=v;p<e;++p)
		{
			wwk_swap_short(p);
		}
	}else
	{
		for(p=v;p<e;++p)
		{
			ret=wwk_source_atoi(s,&x);
			if(ret!=0){goto end;}
			*p=x;
		}
	}
end:
	return ret;
}

int wwk_source_read_ushort(wwk_source_t* s,unsigned short* v,int n,int bin)
{
	int ret=0,x;
	unsigned short *p,*e;

	e=v+n;
	if(bin)
	{
		ret=wwk_source_fill(s,(char*)v,sizeof(unsigned short)*n);
		if(ret!=0 || !s->swap){goto end;}
		for(p=v;p<e;++p)
		{
			wwk_swap_short((short*)p);
		}
	}else
	{
		for(p=v;p<e;++p)
		{
			ret=wwk_source_atoi(s,&x);
			if(ret!=0){goto end;}
			*p=x;
		}
	}
end:
	return ret;
}


int wwk_source_read_float(wwk_source_t *s,float *f,int n,int bin)
{
	int ret=0;
	float *p,*e;
	double d;

	e=f+n;
	if(bin)
	{
		ret=wwk_source_fill(s,(char*)f,n*sizeof(float));
		if(ret!=0 || !s->swap){goto end;}
		for(p=f;p<e;++p)
		{
			wwk_swap_int32((int*)p);
		}
	}else
	{
		//wwk_debug("---------- n=%d ------------\n",n);
		for(p=f;p<e;++p)
		{
			ret=wwk_source_atof(s,&d);
			//wwk_debug("d=%f\n",d);
			if(ret!=0)
			{
				goto end;
			}
			*p=d;
//			if(isnan(*p) || isinf(*p))
//			{
//				wwk_debug("found err\n");
//				exit(0);
//			}
		}
	}
end:
	return ret;
}

int wwk_source_read_double(wwk_source_t *s,double *f,int n)
{
	int ret=0;
	double *p,*e;
	//double d;

	e=f+n;
	for(p=f;p<e;++p)
	{
		ret=wwk_source_atof(s,p);
		if(ret!=0){goto end;}
		//*p=d;
	}
end:
	return ret;
}

int wwk_file_write_float(FILE *file,float *f,int n,int bin,int swap)
{
	int ret=0;
	float *p,*e;

	e=f+n;
	if(bin)
	{
		if(swap)
		{
			for(p=f;p<e;++p)
			{
				wwk_swap_int32((int*)p);
			}
		}
		ret=fwrite(f,sizeof(float),n,file);
		ret=ret==n?0:-1;
		if(swap)
		{
			for(p=f;p<e;++p)
			{
				wwk_swap_int32((int*)p);
			}
		}
	}else
	{
		for(p=f;p<e;++p)
		{
			fprintf(file," %e",*p);
		}
	}
	return ret;
}


int wwk_source_seek_to(wwk_source_t *src,char *data,int len)
{
	char *s,*e;
	int ret;
	int c;

	ret=-1;
	s=data;e=s+len;
	while(1)
	{
		c=wwk_source_get(src);
		if(c==EOF){goto end;}
		if(c==*s)
		{
			++s;
			if(s>=e)
			{
				ret=0;
				break;
			}
		}else
		{
			s=data;
		}
	}
end:
	return ret;
}

int wwk_source_seek_to2(wwk_source_t *src,char *data,int len,wwk_strbuf_t *buf)
{
	char *s,*e;
	int ret;
	int c;

	ret=-1;
	s=data;e=s+len;
	if(buf)
	{
		wwk_strbuf_reset(buf);
	}
	while(1)
	{
		c=wwk_source_get(src);
		if(c==EOF){goto end;}
		if(c==*s)
		{
			++s;
			if(s>=e)
			{
				ret=0;
				break;
			}
		}else
		{
			if(buf)
			{
				if(s-data)
				{
					wwk_strbuf_push(buf,data,s-data);
				}
				wwk_strbuf_push_c(buf,c);
			}
			s=data;
		}
	}
end:
	return ret;
}

int wwk_source_load_file(void *data,wwk_source_load_handler_t loader,char *fn)
{
	wwk_source_t s,*ps=&s;
	int ret;

	ret=wwk_source_init_file(ps,fn);
	if(ret!=0){goto end;}
	ret=loader(data,ps);
	wwk_source_clean_file(ps);
end:
	if(ret!=0)
	{
		wwk_debug("load %s failed.\n",fn);
		exit(0);
	}
	return ret;
}

int wwk_source_load_file_v(void *hook,void *data,wwk_source_load_handler_t loader,char *fn)
{
	return wwk_source_load_file(data,loader,fn);
}

int wwk_source_loader_load(wwk_source_loader_t *l,void *data,wwk_source_load_handler_t loader,char *fn)
{
	return l->vf(l->hook,data,loader,fn);
}

void wwk_source_loader_init_file(wwk_source_loader_t *sl)
{
	sl->hook=NULL;
	sl->vf=wwk_source_load_file_v;
}


int wwk_source_get_lines(int *nw,wwk_source_t *s)
{
	int n=0;
	int c;
	int newline;

	if(!s){goto end;}
	newline=1;
	while(1)
	{
		c=wwk_source_get(s);
		if(c == EOF)
		{
			goto end;
		}
		else if(c=='\n')
		{
			newline = 1;
		}
		else
		{
			if(newline)
			{
				++n;
				newline=0;
			}
		}
	}
end:
	*nw=n;
	return 0;
}

int wwk_source_loader_file_lines(wwk_source_loader_t *sl,char *fn)
{
	int line;
	int ret;

	line=0;
	ret=wwk_source_loader_load(sl, &line,(wwk_source_load_handler_t)wwk_source_get_lines, fn);
	if(ret==0)
	{
		return line;
	}else
	{
		return -1;
	}
}


void wwk_source_read_file2(wwk_source_t *src,wwk_strbuf_t *buf)
{
	int c;

	wwk_strbuf_reset(buf);
	while(1)
	{
		c=wwk_source_get(src);
		if(c==EOF){break;}
		wwk_strbuf_push_c(buf,c);
	}
}

wwk_string_t* wwk_source_read_file(wwk_source_t *src)
{
	wwk_strbuf_t *buf;
	wwk_string_t *data;
	int c;

	buf=wwk_strbuf_new(256,1);
	while(1)
	{
		c=wwk_source_get(src);
		if(c==EOF){break;}
		wwk_strbuf_push_c(buf,c);
	}
	data=wwk_string_dup_data(buf->data,buf->pos);
	wwk_strbuf_delete(buf);
	return data;
}

#include "core/wwk_larray.h"

float* wwk_file_read_float(char *fn,int *n)
{
	float *p=NULL;
	int ret;
	wwk_source_t src;
	wwk_larray_t *a;
	float f;

	if(n)
	{
		*n=0;
	}
	ret=wwk_source_init_file(&(src),fn);
	if(ret!=0){goto end;}
	a=wwk_larray_new(100,sizeof(float));
	while(1)
	{
		ret=wwk_source_read_float(&(src),&f,1,0);
		if(ret!=0){break;}
		wwk_larray_push2(a,&(f));
	}
	p=(float*)wwk_calloc(a->nslot,sizeof(float));
	memcpy(p,a->slot,a->nslot*sizeof(float));
	if(n)
	{
		*n=a->nslot;
	}
	wwk_larray_delete(a);
end:
	wwk_source_clean_file(&(src));
	return p;
}
