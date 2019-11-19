/*************************************************************************
	> File Name: wwk_strbuf.c
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Nov 2019 06:20:19 PM PST
 ************************************************************************/
#include "wwk_strbuf.h"
#include "wwk_os.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

wwk_strbuf_t* wwk_strbuf_new(int init_len,float rate)
{
    wwk_strbuf_t *b;
    char *data;

    data=(char*)wwk_malloc(init_len);
    if(!data){b=0;goto end;}
    b=(wwk_strbuf_t*)wwk_malloc(sizeof(*b));
    b->data=data;
    b->length=init_len;
    b->pos=0;
    b->rate=1.0+rate;
end:
    return b;
}

int wwk_strbuf_bytes(wwk_strbuf_t *b)
{
	return sizeof(*b)+b->length;
}

int wwk_strbuf_delete(wwk_strbuf_t* b)
{
    wwk_free(b->data);
    wwk_free(b);
    return 0;
}

void wwk_strbuf_expand(wwk_strbuf_t *s,int bytes)
{
    int left,alloc;
    char *p;
    int t1,t2;

    left=s->length-s->pos;
    if(bytes>left)
    {
    	t1=s->length*s->rate;
    	t2=s->pos+bytes;
        alloc=max(t1,t2);//s->length*s->rate,s->pos+bytes);
        p=s->data;
        s->data=(char*)wwk_malloc(alloc);
        s->length=alloc;
        memcpy(s->data,p,s->pos);
        wwk_free(p);
    }
    return;
}

void wwk_strbuf_push(wwk_strbuf_t *s,const char *buf,int bytes)
{
    if(bytes<=0){return;}
    if(bytes>s->length-s->pos)
    {
        wwk_strbuf_expand(s,bytes);
    }
    if(buf)
    {
    	memcpy(s->data+s->pos,buf,bytes);
    }else
    {
    	memset(s->data+s->pos,0,bytes);
    }
    s->pos+=bytes;
    return;
}

void wwk_strbuf_push_word(wwk_strbuf_t *buf,char *data,int bytes)
{
	if(buf->pos>0 && (bytes==1 || wwk_utf8_bytes(data[0])==1) && (isalpha(data[0])))
	{
		wwk_strbuf_push_s(buf," ");
	}
	wwk_strbuf_push(buf,data,bytes);
}

void wwk_strbuf_push_front(wwk_strbuf_t *s,const char *buf,int bytes)
{
    int left;

    if(bytes<0){return;}
    left=s->length-s->pos;
    if(bytes>left)
    {
        wwk_strbuf_expand(s,bytes);
    }
    memmove(s->data+bytes,s->data,s->pos);
    if(buf)
    {
    	memcpy(s->data,buf,bytes);
    }else
    {
    	memset(s->data,0,bytes);
    }
    s->pos+=bytes;
    return;
}

/*
void wwk_strbuf_push_c(wwk_strbuf_t *s,char b)
{
	//wwk_strbuf_push(s,&b,1);
    if(s->length<=s->pos)
    {
        wwk_strbuf_expand(s,1);
    }
    s->data[s->pos++]=b;
}
*/

void wwk_strbuf_push_f(wwk_strbuf_t *b,const char *fmt,...)
{
	char buf[4096]={0};
	va_list ap;
	int n;

	va_start(ap,fmt);
	n=vsprintf(buf,fmt,ap);
	wwk_strbuf_push(b,buf,n);
	va_end(ap);
}

int wwk_strbuf_pop(wwk_strbuf_t *s,char* data,int bytes)
{
	int ret;

	if(s->pos<bytes)
	{
		ret=-1;goto end;
	}
	if(data)
	{
		memcpy(data,s->data,bytes);
	}
	s->pos-=bytes;
	if(s->pos>0)
	{
		memmove(s->data,&(s->data[bytes]),s->pos);
	}
	ret=0;
end:
	return ret;
}

char* wwk_strbuf_to_str(wwk_strbuf_t *s)
{
	char *p=0;

	if(s->pos<=0){goto end;}
	p=(char*)wwk_malloc(s->pos+1);
	memcpy(p,s->data,s->pos);
	p[s->pos]=0;
end:
	return p;
}

void wwk_strbuf_merge(wwk_strbuf_t *buf,wwk_string_t *p1,...)
{
    va_list ap;

    va_start(ap,p1);
    while(p1)
    {
    	wwk_strbuf_push(buf,p1->data,p1->len);
        p1=va_arg(ap,wwk_string_t*);
    }
    va_end(ap);
}

void wwk_strbuf_merge2(wwk_strbuf_t *buf,char *p1,...)
{
    va_list ap;

    va_start(ap,p1);
    while(p1)
    {
    	wwk_strbuf_push(buf,p1,strlen(p1));
        p1=va_arg(ap,char*);
    }
    va_end(ap);
}

/*
void wwk_strbuf_reset(wwk_strbuf_t *s)
{
    s->pos=0;
}
*/

void wwk_strbuf_print(wwk_strbuf_t *s)
{
    //print_data(s->data,s->pos);
    printf("%*s\n",s->pos,s->data);
}

int wwk_strbuf_read2(wwk_strbuf_t *buf,char *fn)
{
	FILE *f;
	int ret=-1;
	char tmp[4096];
	int n;

	f=fopen(fn,"rb");
	if(!f){goto end;}
	wwk_strbuf_reset(buf);
	while(1)
	{
		n=fread(tmp,1,sizeof(tmp),f);
		if(n>0)
		{
			wwk_strbuf_push(buf,tmp,n);
		}
		if(n<sizeof(tmp)){break;}
	}
	ret=0;
end:
	if(f){fclose(f);}
	return ret;
}

int wwk_strbuf_read(wwk_strbuf_t *buf,char *fn)
{
    FILE *f;
    int ret=-1;
    int n,len;

    f=fopen(fn,"rb");
    if(!f){goto end;}
    len=file_length(f);
    wwk_strbuf_reset(buf);
    wwk_strbuf_expand(buf,len);
    n=fread(buf->data,1,len,f);
    if(n>0)
    {
        buf->pos=n;
    }
    ret=(n==len)?0:-1;
end:
    if(f){fclose(f);}
    return ret;
}



void wwk_strbuf_strip(wwk_strbuf_t *buf)
{
	int i,n;

	for(i=0,n=0;i<buf->pos;++i)
	{
#ifdef WIN32
		if(isspace((unsigned char)buf->data[i]))
#else
		if(isspace(buf->data[i]))
#endif
		{
			++n;
		}else
		{
			break;
		}
	}
	//wwk_debug("[%d]\n",n);
	if(n>0)
	{
		wwk_strbuf_pop(buf,0,n);
	}
	for(i=buf->pos-1;i>=0;--i)
	{
#ifdef WIN32
		if(isspace((unsigned char)buf->data[i]))
#else
		if(isspace(buf->data[i]))
#endif
		{
			--buf->pos;
		}else
		{
			break;
		}
	}
}

void wwk_strbuf_pad0(wwk_strbuf_t *buf,int bytes)
{
	wwk_strbuf_expand(buf,bytes);
	memset(buf->data+buf->pos,0,bytes);
	buf->pos+=bytes;
}

void wwk_strbuf_replace(wwk_strbuf_t *buf,char *data,int data_len,char *src,int src_len,char *dst,int dst_len)
{
	char *s,*e;
	int i;

	wwk_strbuf_reset(buf);
	s=data;e=s+data_len;
	while(s<e)
	{
		i=wwk_str_str(s,e-s,src,src_len);
		if(i<0)
		{
			wwk_strbuf_push(buf,s,e-s);
			break;
		}
		if(i>0)
		{
			wwk_strbuf_push(buf,s,i);
		}
		wwk_strbuf_push(buf,dst,dst_len);
		s+=i+src_len;
	}
}

#ifndef BUF_USE_MACRO
void wwk_strbuf_push_c(wwk_strbuf_t *buf,char b)
{
    if(buf->length<=buf->pos)
    {
        wwk_strbuf_expand(buf,1);
    }
    buf->data[buf->pos++]=b;
}
#endif

void wwk_strbuf_push_skip_ws(wwk_strbuf_t *buf,char *data,int len)
{
	char *s,*e;
	char c;

	s=data;e=s+len;
	while(s<e)
	{
		c=*s;
		if(c==' '||c=='\t')
		{

		}else
		{
			wwk_strbuf_push_c(buf,c);
		}
		++s;
	}
}

void wwk_strbuf_push_skip_utf8_ws(wwk_strbuf_t *buf,char *data,int len)
{
	char *s,*e;
	char c;
	int n;
	int prev_n=-1;
	int space=0;

	s=data;e=s+len;
	while(s<e)
	{
		c=*s;
		n=wwk_utf8_bytes(c);
		if(n==1)
		{
			if(prev_n>1 && (c==' '||c=='\t'))
			{

			}else
			{
				if(c==' ' || c=='\t')
				{
					space=1;
				}else
				{
					if(space)
					{
						wwk_strbuf_push_c(buf,' ');
					}
					space=0;
					wwk_strbuf_push_c(buf,c);
				}
			}
		}else
		{
			space=0;
			wwk_strbuf_push(buf,s,n);
		}
		prev_n=n;
		s+=n;
	}
}

void wwk_strbuf_push_add_escape_str(wwk_strbuf_t *buf,char *data,int bytes)
{
	char *s,*e;
	char c;

	s=data;e=s+bytes;
	while(s<e)
	{
		c=*s;
#ifdef WIN32
		if(c=='\"' ||c=='\\')
#else
		if(c=='\'' || c=='\"' ||c=='\\')
#endif
		{
			wwk_strbuf_push_c(buf,'\\');
		}
		wwk_strbuf_push_c(buf,c);
		++s;
	}
}

void wwk_strbuf_string_to_str(wwk_strbuf_t *buf,char *data,int bytes)
{
	wwk_strbuf_reset(buf);
	wwk_strbuf_push(buf,data,bytes);
	wwk_strbuf_push_c(buf,0);
}

void wwk_strbuf_parse_quote(wwk_strbuf_t *buf,char *data,int bytes)
{
	char quote;
	char *s,*e;
	char c;
	int esc;

	s=data;e=s+bytes;
	wwk_strbuf_reset(buf);
	quote=*s;
	if(quote!='\'' && quote !='"')
	{
		wwk_strbuf_push(buf,data,bytes);
		return;
	}
	++s;
	esc=0;
	while(s<e)
	{
		c=*s;

		if(esc)
		{
			wwk_strbuf_push_c(buf,c);
			esc=0;
		}else
		{
			if(c=='\\')
			{
				esc=1;
			}else if(c==quote)
			{
				break;
			}else
			{
				wwk_strbuf_push_c(buf,c);
			}
		}
		++s;
	}

}

static wwk_string_t num[]={
		wwk_string("零"),
		wwk_string("一"),
		wwk_string("二"),
		wwk_string("三"),
		wwk_string("四"),
		wwk_string("五"),
		wwk_string("六"),
		wwk_string("七"),
		wwk_string("八"),
		wwk_string("九"),
};

void wwk_stochn(wwk_strbuf_t *buf,char *data,int len)
{
	char *s,*e;
	int v;

	wwk_strbuf_reset(buf);
	s=data;e=s+len;
	while(s<e)
	{
		v=*s-'0';
		if(v<10)
		{
			wwk_strbuf_push(buf,num[v].data,num[v].len);
		}
		++s;
	}
}

static wwk_string_t num2[]={
		wwk_string("零"),
		wwk_string("幺"),
		wwk_string("二"),
		wwk_string("三"),
		wwk_string("四"),
		wwk_string("五"),
		wwk_string("六"),
		wwk_string("七"),
		wwk_string("八"),
		wwk_string("九"),
};

void wwk_stotel(wwk_strbuf_t *buf,char *data,int len)
{
	char *s,*e;
	int v;

	wwk_strbuf_reset(buf);
	s=data;e=s+len;
	while(s<e)
	{
		v=*s-'0';
		if(v<10)
		{
			wwk_strbuf_push(buf,num2[v].data,num2[v].len);
		}
		++s;
	}
}

void wwk_itochn2(wwk_strbuf_t *buf,int v,int last_u)
{
typedef struct
{
	int v;
	wwk_string_t s;
}wwk_iunit_t;
	static wwk_iunit_t unit1[]={
			{100000000,wwk_string("亿")},
			{10000,wwk_string("万")},
	};
	static wwk_iunit_t unit2[]={
			{1000,wwk_string("千")},
			{100,wwk_string("百")},
			{10,wwk_string("十")},
	};
	int x;
	int i;

	for(i=0;i<sizeof(unit1)/sizeof(wwk_iunit_t);++i)
	{
		x=v/(unit1[i].v);
		if(x>0)
		{
			//wwk_debug("x=%d\n",x);
			wwk_itochn2(buf,x,last_u);
			wwk_strbuf_push(buf,unit1[i].s.data,unit1[i].s.len);
			v=v%(unit1[i].v);
			//last_u=unit1[i].v;
			wwk_itochn2(buf,v,unit1[i].v);
			return;
		}
	}
	for(i=0;i<sizeof(unit2)/sizeof(wwk_iunit_t);++i)
	{
		x=v/(unit2[i].v);
		if(x>0 && x<10)
		{
			//wwk_debug("x=%d\n",x);
			//wwk_debug("last=%d/%d\n",last_u,unit2[i].v);
			if(last_u/unit2[i].v>10)
			{
				wwk_strbuf_push(buf,num[0].data,num[0].len);
			}
			if(x==1 && buf->pos==0 && unit2[i].v==10)
			{

			}else
			{
				wwk_strbuf_push(buf,num[x].data,num[x].len);
			}
			wwk_strbuf_push(buf,unit2[i].s.data,unit2[i].s.len);
			v=v%(unit2[i].v);
			last_u=unit2[i].v;
			//wwk_debug("[%.*s]\n",buf->pos,buf->data);
		}
	}
	//wwk_debug("v=%d last_u=%d\n",v,last_u);
	if(v>0 && v<10)
	{
		if(last_u>10)
		{
			wwk_strbuf_push(buf,num[0].data,num[0].len);
		}
		wwk_strbuf_push(buf,num[v].data,num[v].len);
	}else if(v==0 && buf->pos==0)
	{
		wwk_strbuf_push(buf,num[v].data,num[v].len);
	}
	//wwk_debug("[%.*s]\n",buf->pos,buf->data);
	//wwk_debug("v=%d\n",v);
	//exit(0);
}

void wwk_itochn(wwk_strbuf_t *buf,int v)
{
	wwk_strbuf_reset(buf);
	wwk_itochn2(buf,v,0);
}

#include "core/wwk_larray.h"

//void wwk_strbuf_real_path(wwk_strbuf_t *buf,char *name,int len)
//{
//enum
//{
//	wwk_RP_INIT,
//};
//	char *s,*e;
//	int state;
//	int n;
//	char c;
//	wwk_larray_t *a;
//	wwk_heap_t *heap;
//	wwk_string_t *v;
//	wwk_string_t **strs;
//	int i;
//
//	//wwk_debug("[%.*s]\n",len,name);
//	heap=wwk_heap_new(4096);
//	a=wwk_larray_new(len/3,sizeof(void*));
//	wwk_strbuf_reset(buf);
//	s=name;
//	e=s+len;
//	state=wwk_RP_INIT;
//	while(s<e)
//	{
//		c=*s;
//		n=wwk_utf8_bytes(c);
//		switch(state)
//		{
//		case wwk_RP_INIT:
//			if(n==1 && *s=='/')
//			{
//				//wwk_debug("[%.*s]\n",buf->pos,buf->data);
//				if(buf->pos>0)
//				{
//					if(wwk_str_equal_s(buf->data,buf->pos,".."))
//					{
//						if(a->nslot>0)
//						{
//							--a->nslot;
//						}
//					}else if(wwk_str_equal_s(buf->data,buf->pos,"."))
//					{
//						if(a->nslot==0)
//						{
//							v=wwk_heap_dup_string(heap,buf->data,buf->pos);
//							wwk_larray_push2(a,&v);
//						}
//					}else
//					{
//						v=wwk_heap_dup_string(heap,buf->data,buf->pos);
//						wwk_larray_push2(a,&v);
//					}
//					wwk_strbuf_reset(buf);
//				}
//			}else
//			{
//				wwk_strbuf_push(buf,s,n);
//			}
//			break;
//		}
//		s+=n;
//	}
//	if(buf->pos>0)
//	{
//		if(wwk_str_equal_s(buf->data,buf->pos,".."))
//		{
//			if(a->nslot>0)
//			{
//				--a->nslot;
//			}
//		}else if(wwk_str_equal_s(buf->data,buf->pos,"."))
//		{
//			if(a->nslot==0)
//			{
//				v=wwk_heap_dup_string(heap,buf->data,buf->pos);
//				wwk_larray_push2(a,&v);
//			}
//		}else
//		{
//			v=wwk_heap_dup_string(heap,buf->data,buf->pos);
//			wwk_larray_push2(a,&v);
//		}
//	}
//	strs=(wwk_string_t**)(a->slot);
//	n=a->nslot;
//	//wwk_debug("n=%d\n",a->nslot);
//	wwk_strbuf_reset(buf);
//	for(i=0;i<n;++i)
//	{
//		//wwk_debug("[%.*s]\n",strs[i]->len,strs[i]->data);
//		if(buf->pos>0)
//		{
//			wwk_strbuf_push_s(buf,"/");
//		}
//		wwk_strbuf_push(buf,strs[i]->data,strs[i]->len);
//	}
//	//wwk_debug("[%.*s]\n",buf->pos,buf->data);
//	wwk_heap_delete(heap);
//	wwk_larray_delete(a);
//	//exit(0);
//}
//
//void wwk_strbuf_control_cache(wwk_strbuf_t *buf,int max_cache)
//{
//	char *data;
//
//	if(buf->length>max_cache && buf->pos<=max_cache)
//	{
//		data=wwk_malloc(max_cache);
//		memcpy(data,buf->data,buf->pos);
//		wwk_free(buf->data);
//		buf->data=data;
//		buf->length=max_cache;
//	}
//}
//




