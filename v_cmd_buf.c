/*
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "verse_header.h"
#include "v_pack.h"
#include "v_cmd_buf.h"


unsigned int vcmdbuf_chunk_size[] = {10000, 10000, 10000, 10000, 8000, 5000, 500};	/* If you think memory is cheap, set this to a high value. */
#define VCMDBUF_INIT_CHUNK_FACTOR 0.5

static struct {
	VCMDBufHead	*buffers[VCMDBS_COUNT];
	unsigned int available[VCMDBS_COUNT];
} VCMDBufData;

static boolean v_cmd_buf_initialized = FALSE;

void cmd_buf_init(void)
{
	unsigned int i, j;
	VCMDBufHead *buf = NULL, *b;

	for(i = 0; i < VCMDBS_COUNT; i++)
	{
		VCMDBufData.buffers[i] = NULL;
		VCMDBufData.available[i] = (unsigned int)((float)vcmdbuf_chunk_size[i] * VCMDBUF_INIT_CHUNK_FACTOR);
		buf = NULL;
		for(j = 0; j < (unsigned int)((float)vcmdbuf_chunk_size[i] * VCMDBUF_INIT_CHUNK_FACTOR); j++)
		{
			b = v_cmd_buf_allocate(i);
			b->next = buf;
			buf = b;
		}
		VCMDBufData.buffers[i] = buf;
	}
	v_cmd_buf_initialized = TRUE;
}

VCMDBufHead * v_cmd_buf_allocate(VCMDBufSize buf_size)
{
	VCMDBufHead	*output;
	if(VCMDBufData.buffers[buf_size] != NULL)
	{
		output = VCMDBufData.buffers[buf_size];
		VCMDBufData.buffers[buf_size] = output->next;
		VCMDBufData.available[buf_size]--;
	}else
	{
		switch(buf_size)
		{
			case VCMDBS_1500 :
				output = malloc(sizeof(VCMDBuffer1500));
			break;
			case VCMDBS_320 :
				output = malloc(sizeof(VCMDBuffer320));
			break;
			case VCMDBS_160 :
				output = malloc(sizeof(VCMDBuffer160));
			break;
			case VCMDBS_80 :
				output = malloc(sizeof(VCMDBuffer80));
			break;
			case VCMDBS_30 :
				output = malloc(sizeof(VCMDBuffer30));
			break;
			case VCMDBS_20 :
				output = malloc(sizeof(VCMDBuffer20));
			break;
			case VCMDBS_10 :
				output = malloc(sizeof(VCMDBuffer10));
			break;
		default:
			fprintf(stderr, "v_cmd_buf.c: Can't handle buffer size %d\n", buf_size);
			return NULL;
		}
		output->buf_size = buf_size;
	}
	output->next = NULL;	
	output->packet = 0;
	output->size = 0;
	output->address_size = -1;
	return output;
}

void v_cmd_buf_free(VCMDBufHead *head)
{
	if(VCMDBufData.available[head->buf_size] < vcmdbuf_chunk_size[head->buf_size])
	{
		head->next = VCMDBufData.buffers[head->buf_size];
		VCMDBufData.buffers[head->buf_size] = head;
		VCMDBufData.available[head->buf_size]++;
	}else
		free(head);
}

void v_cmd_buf_set_size(VCMDBufHead *head, unsigned int size)
{
	if(head->address_size > size);
		head->address_size = size;
	head->size = size;
}

void v_cmd_buf_set_address_size(VCMDBufHead *head, unsigned int size)
{
	unsigned int i;
	head->address_size = size;
	head->address_sum = 0;
	for(i = 1; i < size + 1; i++)
		head->address_sum += i * i * (uint32)(((VCMDBuffer1500 *)head)->buf[i - 1]);
}

void v_cmd_buf_set_unique_address_size(VCMDBufHead *head, unsigned int size)
{
	static unsigned int i = 0;
	head->address_size = size;
	head->address_sum = i++;
}

boolean	v_cmd_buf_compare(VCMDBufHead *a, VCMDBufHead *b)
{
	unsigned int i;

	if(a->address_sum != b->address_sum)
		return FALSE;
	if(a->address_size != b->address_size)
		return FALSE;
	for(i = 0; i < a->address_size; i++)
		if(((VCMDBuffer1500 *)a)->buf[i] != ((VCMDBuffer1500 *)b)->buf[i])
			return FALSE;
	return TRUE;
}
