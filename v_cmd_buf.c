/*
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "verse_header.h"
#include "v_pack.h"
#include "v_cmd_buf.h"

#define VCMDBUF_CHUNK_SIZE	256	/* If you think memory is cheap, set this to a high value. */

static struct {
	VCMDBufHead	*buffers[VCMDBS_COUNT][VCMDBUF_CHUNK_SIZE];
	unsigned int available[VCMDBS_COUNT];
} VCMDBufData;

static boolean v_cmd_buf_initialized = FALSE;

void v_cmd_buf_init(void)
{
	unsigned int i, j;

	for(i = 0; i < VCMDBS_COUNT; i++)
	{
		for(j = 0; j < VCMDBUF_CHUNK_SIZE; j++)
		VCMDBufData.buffers[i][j] = NULL;
		VCMDBufData.available[i] = 0;
	}
	v_cmd_buf_initialized = TRUE;
}

VCMDBufHead * v_cmd_buf_allocate(VCMDBufSize buf_size)
{
	VCMDBufHead	*output;

	if(!v_cmd_buf_initialized)
		v_cmd_buf_init();
	if(VCMDBufData.available[buf_size] > 0)
		output = VCMDBufData.buffers[buf_size][--VCMDBufData.available[buf_size]];
	else
	{
		switch(buf_size)
		{
			case VCMDBS_1500 :
				output = malloc(sizeof(VCMDBuffer1500));
			break;
			case VCMDBS_500 :
				output = malloc(sizeof(VCMDBuffer500));
			break;
			case VCMDBS_100 :
				output = malloc(sizeof(VCMDBuffer100));
			break;
			case VCMDBS_50 :
				output = malloc(sizeof(VCMDBuffer50));
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
	return output;
}

void v_cmd_buf_free(VCMDBufHead *head)
{
	if(VCMDBufData.available[head->buf_size] < VCMDBUF_CHUNK_SIZE)
		VCMDBufData.buffers[head->buf_size][VCMDBufData.available[head->buf_size]++] = head;
	else
		free(head);
}

void v_cmd_buf_set_address_size(VCMDBufHead *head, unsigned int address_size, unsigned int size)
{
	unsigned int i;

	head->address_size = address_size;
	head->size = size;
	head->address_sum = 0;
	for(i = 0; i < address_size; i++)
		head->address_sum += i * i * (uint32)(((VCMDBuffer1500 *)head)->buf[i]);
}

void v_cmd_buf_set_unique_size(VCMDBufHead *head, unsigned int size)
{
	static unsigned int i = 0;

	head->address_size = size;
	head->size = size;
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
