/*
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "verse_header.h"

#include "v_cmd_buf.h"
#include "v_cmd_gen.h"
#include "v_connection.h"
#include "v_network.h"
#include "v_pack.h"

#include "v_network_que.h"

#if !defined(V_GENERATE_FUNC_MODE)

#define STD_QUE_SIZE		64
#define V_NQ_MAX_PACKET_SIZE	500

#define V_NQ_WINDOW_SIZE	10000

#define V_NQ_OPTIMIZATION_SLOTS	4096

unsigned int std_queue_size = STD_QUE_SIZE;

typedef struct{
	void		*next;
	char		*data;
	unsigned int	size;
} NetPacked;

struct VNetQueue {
	NetPacked	*packed;
	NetPacked	*last;
	VCMDBufHead	*unsent[V_NQ_OPTIMIZATION_SLOTS];
	VCMDBufHead	*history[V_NQ_OPTIMIZATION_SLOTS];
	VCMDBufHead	*ack_nak;
	unsigned int	unsent_size;
	unsigned int	sent_size;
	unsigned int	packet_id;
	unsigned int	slot;
};

unsigned int verse_session_get_size(void)
{
	VNetQueue *queue;
	queue = v_con_get_network_queue();
	return queue->unsent_size +	queue->sent_size;
}

VNetQueue * v_nq_create_network_queue(void)
{
	VNetQueue *queue;
	unsigned int i;

	queue = malloc(sizeof *queue);
	for(i = 0; i < V_NQ_OPTIMIZATION_SLOTS; i++)
	{
		queue->unsent[i] = NULL;
		queue->history[i] = NULL;
	}
	queue->unsent_size = 0;
	queue->sent_size = 0;
	queue->packet_id = 0;
	queue->slot = 0;
	queue->packed = NULL;
	queue->last = NULL;
	queue->ack_nak = NULL;
	return queue;
}

char * v_nq_get_packed(VNetQueue *queue, unsigned int *length)
{
	NetPacked	*p;
	char		*data;

	if(queue->packed == NULL)
		return NULL;
	p = queue->packed;
	queue->packed = p->next;
	if(queue->packed == NULL)
		queue->last = NULL;
	data = p->data;
	*length = p->size;
	free(p);
	return data;
}

void v_nq_store_packed(VNetQueue *queue, const char *data, unsigned int length)
{
	NetPacked	*p;

	p = malloc(sizeof *p);
	if(queue->packed == NULL)
		queue->packed = p;
	else
		queue->last->next = p;
	queue->last = p;
	p->next = NULL;
	p->size = length;
	p->data = malloc(length * sizeof *p->data);
	memcpy(p->data, data, length);
}

void v_nq_destroy_network_queue(VNetQueue *queue)
{
/*	VCMDBufHead *buf;
	for(buf = queue->history; buf != NULL; buf = buf->next);
		v_cmd_buf_free(buf);
	for(buf = queue->unsent_end; buf != NULL && buf != queue->unsent_start; buf = buf->next);
		v_cmd_buf_free(buf);*/
	free(queue);
}
/*
boolean v_nq_send_queue(VNetQueue *queue, void *address)
{
	VCMDBufHead *buf;
	unsigned int i, size;
	char data[1500];
	if(queue->unsent_size == 0)	
		return FALSE;
	if(queue->sent_size > V_NQ_WINDOW_SIZE)
		return FALSE;
	while(queue->unsent_size != 0 && queue->sent_size < V_NQ_WINDOW_SIZE)
	{
		size = vnp_raw_pack_uint32(data, queue->packet_id);
		for(buf = queue->unsent; buf != NULL; buf = queue->unsent)
		{
			if(buf->size + size > V_NQ_MAX_PACKET_SIZE)
				break;
			queue->unsent = buf->next;
			buf->next = queue->history;
			queue->history = buf;
			buf->packet = queue->packet_id;
			for(i = 0; i < buf->size; i++)
				data[size++] = ((VCMDBuffer1500 *)buf)->buf[i];
		}
		queue->unsent_size -= size - 4;
		queue->sent_size += size - 4;
		v_n_send_data(address, data, size);
		queue->packet_id++;
	}
	return TRUE;
}

*/

boolean v_nq_send_queue(VNetQueue *queue, void *address)
{
	VCMDBufHead *buf;
	unsigned int i, size;
	char data[1500];
	size = vnp_raw_pack_uint32(data, queue->packet_id);
	buf = queue->ack_nak;
	while(buf != NULL && buf->size < V_NQ_MAX_PACKET_SIZE)
	{
		queue->ack_nak = buf->next;
		buf->next = queue->history[queue->slot];
		queue->history[queue->slot] = buf;
		buf->packet = queue->packet_id;
		for(i = 0; i < buf->size; i++)
			data[size++] = ((VCMDBuffer1500 *)buf)->buf[i];
		queue->sent_size += buf->size;
		buf = queue->ack_nak;
	}
	if(queue->unsent_size == 0 || queue->sent_size > V_NQ_WINDOW_SIZE)
	{
		if(size > 5)
		{
			v_n_send_data(address, data, size);
			queue->packet_id++;
			return TRUE;
		}
		return FALSE;
	}
	while(queue->unsent_size != 0 && queue->sent_size < V_NQ_WINDOW_SIZE)
	{
		while(queue->unsent_size != 0)
		{
			queue->slot = ((1 + queue->slot) % V_NQ_OPTIMIZATION_SLOTS);
			buf = queue->unsent[queue->slot];
			if(buf != NULL)
			{
				if(buf->size + size > V_NQ_MAX_PACKET_SIZE)
					break;
				queue->unsent[queue->slot] = buf->next;
				buf->next = queue->history[queue->slot];
				queue->history[queue->slot] = buf;
				buf->packet = queue->packet_id;
				for(i = 0; i < buf->size; i++)
					data[size++] = ((VCMDBuffer1500 *)buf)->buf[i];
				queue->unsent_size -= buf->size;
				queue->sent_size += buf->size;
			}
		}
		v_n_send_data(address, data, size);
		queue->packet_id++;
		size = vnp_raw_pack_uint32(data, queue->packet_id);
	}
	return TRUE;
}

void v_nq_send_ack_nak_buf(VNetQueue *queue, VCMDBufHead *buf)
{
	buf->next = queue->ack_nak;
	queue->ack_nak = buf;
}

void v_nq_send_buf(VNetQueue *queue, VCMDBufHead *buf)
{
	VCMDBufHead *b, *last = NULL;
	unsigned int slot;
	slot = buf->address_sum % V_NQ_OPTIMIZATION_SLOTS;
	queue->unsent_size += buf->size;
	if(queue->unsent[slot] != NULL)
	{
		for(b = queue->unsent[slot]; !v_cmd_buf_compare(buf, b) && b->next != NULL; b = b->next)
			last = b;
		if(v_cmd_buf_compare(buf, b)) /* fond a command to replace */
		{
			queue->unsent_size -= b->size;
			if(last != NULL) /* if its not the first */
				last->next = buf;
			else
				queue->unsent[slot] = buf;
			buf->next = b->next;
			v_cmd_buf_free(b);
		}else /* inserting the command last in queue */
		{
			buf->next = NULL;
			b->next = buf;
		}
	}else /* inserting the first command */
	{
		queue->unsent[slot] = buf;
		buf->next = NULL;
	}
	if(queue->history[slot] != NULL) /* if there is a history clear it fom any commnds with same address */
	{
		last = NULL;
		for(b = queue->history[slot]; b != NULL && !v_cmd_buf_compare(buf, b); b = b->next)
			last = b;
		if(b != NULL) /* fond a command to replace */
		{
			if(last == NULL)
				queue->history[slot] = b->next;
			else
				last->next = b->next;
			queue->sent_size -= b->size;
			v_cmd_buf_free(b);
		}
	}
	if(queue->unsent_size > V_NQ_MAX_PACKET_SIZE - 4)
		v_nq_send_queue(queue, v_con_get_network_address());
	else
		v_con_network_listen();
}

void callback_send_packet_ack(void *user, uint32 packet_id)
{
	VNetQueue *queue;
	VCMDBufHead *buf, *last;
	unsigned int slot;
	queue = v_con_get_network_queue();
	for(slot = 0; slot < V_NQ_OPTIMIZATION_SLOTS; slot++)
	{
		last = NULL;
		for(buf = queue->history[slot]; buf != NULL && buf->packet != packet_id; buf = buf->next)
			last = buf;

		if(buf != NULL)
		{
			if(last == NULL)
			{
				while(queue->history[slot] != NULL && queue->history[slot]->packet == packet_id)
				{
					queue->sent_size -= queue->history[slot]->size;
					buf = queue->history[slot]->next;
					v_cmd_buf_free(queue->history[slot]);
					queue->history[slot] = buf;
				}
			}else
			{
				for(; buf != NULL && buf->packet == packet_id; buf = last->next)
				{
					queue->sent_size -= buf->size;
					last->next = buf->next;
					v_cmd_buf_free(buf);
				}
			}
		}
	}
}

void callback_send_packet_nak(void *user, uint32 packet_id)
{
	VNetQueue *queue;
	VCMDBufHead *buf, *last;
	unsigned int slot;
	queue = v_con_get_network_queue();
	for(slot = 0; slot < V_NQ_OPTIMIZATION_SLOTS; slot++)
	{
		last = NULL;
		for(buf = queue->history[slot]; buf != NULL && buf->packet != packet_id; buf = buf->next)
			last = buf;
		if(buf != NULL)
		{
			if(last == NULL)
			{
				for(; queue->history[slot] != NULL && queue->history[slot]->packet == packet_id; queue->history[slot] = buf)
				{
					queue->unsent_size += queue->history[slot]->size;
					queue->sent_size -= queue->history[slot]->size;
					buf = queue->history[slot]->next;
					queue->history[slot]->next = queue->unsent[slot];
					queue->unsent[slot] = queue->history[slot];
				}
			}else
			{
				for(; last->next != NULL && ((VCMDBufHead *)last->next)->packet == packet_id;)
				{
					queue->unsent_size += ((VCMDBufHead *)last->next)->size;
					queue->sent_size -= ((VCMDBufHead *)last->next)->size;
					buf = last->next;
					last->next = buf->next;
					buf->next = queue->unsent[slot];
					queue->unsent[slot] = buf;
				}
			}
		}
	}
}

#endif
