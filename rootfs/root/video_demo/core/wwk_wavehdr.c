/*************************************************************************
	> File Name: core/wwk_wavehdr.c
	> Author: 
	> Mail: 
	> Created Time: Tue 12 Nov 2019 06:59:11 PM PST
 ************************************************************************/


#include <string.h>
#include <stdio.h>
#include "wwk_wavehdr.h"

void wavehdr_init(WaveHeader* header)
{
	if (header)
	{
		memcpy(header->riff_id, "RIFF", 4);
		memcpy(header->riff_type, "WAVE", 4);
		memcpy(header->fmt_id, "fmt ", 4);
		header->fmt_datasize = 16;
		header->fmt_compression_code = 1;
		memcpy(header->data_id, "data", 4);
	}
}

void wavehdr_set_fmt(WaveHeader* header, int channels, int sampleRate,
		int bytesPerSample)
{
	if (header)
	{
		header->fmt_channels = channels;
		header->fmt_sample_rate = sampleRate;
		header->fmt_block_align = channels * bytesPerSample;
		header->fmt_avg_bytes_per_sec = header->fmt_block_align * sampleRate;
		header->fmt_bit_per_sample = bytesPerSample << 3;
	}
}

void wavehdr_set_size(WaveHeader* header, int rawDataSize)
{
	if (header)
	{
		header->riff_datasize = rawDataSize + sizeof(WaveHeader) - 8;
		header->data_datasize = rawDataSize;///header->fmt_channels;
	}
}


void wavehdr_print(WaveHeader* header)
{
	printf("RIFF: %.*s\n",4,header->riff_id);
	printf("RIFF DATA_SIZE: %d\n",header->riff_datasize);
	printf("channels:\t%d\n", header->fmt_channels);
	printf("avg_bytes_per_sec:\t%d\n", header->fmt_avg_bytes_per_sec);
	printf("block align:\t%d\n", header->fmt_block_align);
	printf("sample rate:\t%d\n", header->fmt_sample_rate);
	printf("audio data size:\t%d\n", header->data_datasize);
	printf("fmt data size:\t%d\n",header->fmt_datasize);
}

void wave_write_file(char *fn,int rate,char *data,int bytes)
{
	WaveHeader hdr;
	FILE *f;

	wavehdr_init(&hdr);
	wavehdr_set_fmt(&hdr,1,rate,2);
	wavehdr_set_size(&hdr,bytes);
	f=fopen(fn,"wb");
	fwrite((void*)&hdr,1,sizeof(hdr),f);
	fwrite((void*)data,1,bytes,f);
	fclose(f);
}

float wwk_float_abs_max(float *a,int n)
{
    int i;
	float f,t;

	//f=fabs(a[0]);
	f=a[0]>0?a[0]:-a[0];
	for(i=1;i<n;++i)
	{
		t=a[i]>0?a[i]:-a[i];
		if(t>f)
		{
			f=t;
		}
	}
	return f;
}

void wave_write_file_float(char *fn,int rate,float *data,int len)
{
	short *pv;
	int i;
	float f;

	pv=(short*)wwk_malloc(len*sizeof(short));
	f=wwk_float_abs_max(data,len);
	f=32000.0/f;
	for(i=0;i<len;++i)
	{
		pv[i]=data[i]*f;
	}
	wave_write_file(fn,rate,(char*)pv,len*2);
	wwk_free(pv);
}

void wave_write_file_float3(char *fn,int channel,int rate,wwk_strbuf_t **bufs)
{
	wwk_strbuf_t **xbuf;
	int i,j,n;
	float mx;
	short v;
	float *fp;

	xbuf=(wwk_strbuf_t**)wwk_calloc(channel,sizeof(wwk_strbuf_t*));
	for(i=0;i<channel;++i)
	{
		xbuf[i]=wwk_strbuf_new(1024,1);
	}
	for(i=0;i<channel;++i)
	{
		n=bufs[i]->pos/sizeof(float);
		fp=(float*)(bufs[i]->data);
		mx=wwk_float_abs_max(fp,n)*2;
		for(j=0;j<n;++j)
		{
			v=32700*fp[j]/mx;
			wwk_strbuf_push(xbuf[i],(char*)&v,2);
		}
	}
	wave_write_file3(fn,channel,rate,xbuf);
	for(i=0;i<channel;++i)
	{
		wwk_strbuf_delete(xbuf[i]);
	}
	wwk_free(xbuf);
}

void wave_write_file_float4(char *fn,int channel,int rate,wwk_strbuf_t **bufs,wwk_strbuf_t **pad,int nx)
{
	wwk_strbuf_t **xbuf;
	int i,j,n;
	float mx;
	short v;
	float *fp;

	xbuf=(wwk_strbuf_t**)wwk_calloc(channel+nx,sizeof(wwk_strbuf_t*));
	for(i=0;i<(channel+nx);++i)
	{
		xbuf[i]=wwk_strbuf_new(1024,1);
	}
	for(i=0;i<channel;++i)
	{
		n=bufs[i]->pos/sizeof(float);
		fp=(float*)(bufs[i]->data);
		mx=wwk_float_abs_max(fp,n)*2;
		for(j=0;j<n;++j)
		{
			v=32700*fp[j]/mx;
			wwk_strbuf_push(xbuf[i],(char*)&v,2);
		}
	}
	for(i=0;i<nx;++i)
	{
		wwk_strbuf_push(xbuf[i+channel],pad[i]->data,pad[i]->pos);
	}
	wave_write_file3(fn,channel+nx,rate,xbuf);
	for(i=0;i<(channel+nx);++i)
	{
		wwk_strbuf_delete(xbuf[i]);
	}
	wwk_free(xbuf);
}

void wave_write_file_float2(char *fn,int rate,float *data,int len)
{
	short *pv;
	int i;
	float f;

	pv=(short*)wwk_malloc(len*sizeof(short));
	f=wwk_float_abs_max(data,len);
	//f=1.0;
	wwk_debug("maxf=%f\n",f);
	if(f<0.1)
	{
		f=0.1;
	}
	f=32000.0/(f*2);
	for(i=0;i<len;++i)
	{
		pv[i]=data[i]*f;
	}
	wave_write_file(fn,rate,(char*)pv,len*2);
	wwk_free(pv);
}

void wave_write_file2(char *fn,int channel,int rate,char *data,int bytes)
{
	WaveHeader hdr;
	FILE *f;

	wavehdr_init(&hdr);
	wavehdr_set_fmt(&hdr,channel,rate,2);
	wavehdr_set_size(&hdr,bytes);
	f=fopen(fn,"wb");
	fwrite((void*)&hdr,1,sizeof(hdr),f);
	fwrite((void*)data,1,bytes,f);
	fclose(f);
}

void wave_write_file22(char *fn,int channel,int rate,int bytes_per_sample,char *data,int bytes)
{
	WaveHeader hdr;
	FILE *f;

	wavehdr_init(&hdr);
	wavehdr_set_fmt(&hdr,channel,rate,bytes_per_sample);
	wavehdr_set_size(&hdr,bytes);
	f=fopen(fn,"wb");
	fwrite((void*)&hdr,1,sizeof(hdr),f);
	fwrite((void*)data,1,bytes,f);
	fclose(f);
}


void wave_write_file3(char *fn,int channel,int rate,wwk_strbuf_t **bufs)
{
	wwk_strbuf_t *output;
	int i,j;
	int len;

	output=wwk_strbuf_new(bufs[0]->pos*channel,1);
	len=bufs[0]->pos;
	for(i=1;i<channel;++i)
	{
		if(bufs[i]->pos<len)
		{
			len=bufs[i]->pos;
		}
	}
	len/=2;
	for(i=0;i<len;++i)
	{
		for(j=0;j<channel;++j)
		{
			wwk_strbuf_push(output,(bufs[j]->data+i*2),2);
		}
	}
	wave_write_file2(fn,channel,rate,output->data,output->pos);
	wwk_strbuf_delete(output);
}


void wave_write_file32(char *fn,int channel,int rate,wwk_strbuf_t **bufs,int skip)
{
	wwk_strbuf_t *output;
	int i,j;
	int len;

	output=wwk_strbuf_new(bufs[0]->pos*channel,1);
	len=bufs[0]->pos;
	for(i=1;i<channel;++i)
	{
		if(bufs[i]->pos<len)
		{
			len=bufs[i]->pos;
		}
	}
	len/=2;
	len-=skip;
	for(i=0;i<len;++i)
	{
		for(j=0;j<channel;++j)
		{
			wwk_strbuf_push(output,(bufs[j]->data+skip*2+i*2),2);
		}
	}
	wave_write_file2(fn,channel,rate,output->data,output->pos);
	wwk_strbuf_delete(output);
}

void wave_write_file4(char *fn,int channel,int rate,char **data,int len)
{
	wwk_strbuf_t *output;
	int i,j;

	output=wwk_strbuf_new(len*channel,1);
	len/=2;
	for(i=0;i<len;++i)
	{
		for(j=0;j<channel;++j)
		{
			wwk_strbuf_push(output,(data[j]+i*2),2);
		}
	}
	wave_write_file2(fn,channel,rate,output->data,output->pos);
	wwk_strbuf_delete(output);
}

void wave_write_file5(char *fn,int channel,int rate,short **data,int len)
{
	wwk_strbuf_t *output;
	int i,j;

	output=wwk_strbuf_new(len*channel,1);
	for(i=0;i<len;++i)
	{
		for(j=0;j<channel;++j)
		{
			wwk_strbuf_push(output,(char*)(data[j]+i),2);
		}
	}
	wave_write_file2(fn,channel,rate,output->data,output->pos);
	wwk_strbuf_delete(output);
}


