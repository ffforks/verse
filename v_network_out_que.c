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
#include "v_encryption.h"
#include "v_network_out_que.h"

#if !defined(V_GENERATE_FUNC_MODE)

#define STD_QUE_SIZE		64

#define V_NOQ_OPTIMIZATION_SLOTS	4096

#define V_NOQ_INITIAL_BANDWIDTH		500000
#define V_NOQ_BANDWIDTH_FACTOR_ACK	1.005
#define V_NOQ_BANDWIDTH_FACTOR_NAK	0.98

typedef struct{
	void	*next;
	char	*data;
	size_t	size;
} NetPacked;

struct VNetOutQueue{
	NetPacked		*packed;
	NetPacked		*last;
	VCMDBufHead		*unsent[V_NOQ_OPTIMIZATION_SLOTS];
	VCMDBufHead		*history[V_NOQ_OPTIMIZATION_SLOTS];
	VCMDBufHead		*ack_nak;
	size_t			unsent_size;
	size_t			sent_size;
	unsigned int	packet_id;
	unsigned int	slot;
	double			send_rate;
	uint32			seconds;
	uint32			fractions;
};

size_t verse_session_get_size(void)
{
	const VNetOutQueue *queue;

	queue = v_con_get_network_queue();
	return queue->unsent_size + queue->sent_size;
}

VNetOutQueue * v_noq_create_network_queue(void)
{
	VNetOutQueue *queue;
	unsigned int i;

	queue = malloc(sizeof *queue);
	for(i = 0; i < V_NOQ_OPTIMIZATION_SLOTS; i++)
	{
		queue->unsent[i] = NULL;
		queue->history[i] = NULL;
	}
	queue->unsent_size = 0;
	queue->sent_size = 0;
	queue->packet_id = 2;
	queue->slot = 0;
	queue->packed = NULL;
	queue->last = NULL;
	queue->ack_nak = NULL;
	queue->send_rate = V_NOQ_INITIAL_BANDWIDTH;
	v_n_get_current_time(&queue->seconds, &queue->fractions);
	return queue;
}

unsigned int v_noq_get_next_out_packet_id(VNetOutQueue *queue)
{
	return queue->packet_id++;
}

void v_noq_destroy_network_queue(VNetOutQueue *queue)
{
/*	VCMDBufHead *buf;
	for(buf = queue->history; buf != NULL; buf = buf->next);
		v_cmd_buf_free(buf);
	for(buf = queue->unsent_end; buf != NULL && buf != queue->unsent_start; buf = buf->next);
		v_cmd_buf_free(buf);*/
	free(queue);
}

static unsigned int ack_balance = 0;
static unsigned int nak_balance = 0;
static unsigned int in_balance = 1;
static unsigned int out_balance = 1;


boolean v_noq_send_queue(VNetOutQueue *queue, void *address)
{
	VCMDBufHead *buf;
	unsigned int i, size;
	char data[1500];
	uint32 seconds, fractions;
	double delta;

	static double *save = NULL;
	static unsigned int count = 0;
	if(save == NULL)
		save = malloc((sizeof *save) * 10000);

	size = vnp_raw_pack_uint32(data, queue->packet_id);
	buf = queue->ack_nak;
	v_n_get_current_time(&seconds, &fractions);
	delta = (double)(seconds - queue->seconds) + ((double)fractions - (double)queue->fractions) / (double)(0xffffffff);
/*	printf("delta %f %f\n", delta, delta * queue->send_rate);*/
	if(queue->unsent_size == 0 && delta < 1 && (queue->ack_nak == NULL || queue->ack_nak->next == NULL))
		return TRUE;

	while(buf != NULL && size + buf->size < V_NOQ_MAX_PACKET_SIZE)
	{
		queue->ack_nak = buf->next;
		buf->next = queue->history[queue->slot];
		queue->history[queue->slot] = buf;
		buf->packet = queue->packet_id;
		v_e_encrypt_command(data, size, ((VCMDBuffer1500 *)buf)->buf, buf->size, v_con_get_data_key());
		size += buf->size;
		queue->sent_size += buf->size;
		buf = queue->ack_nak;
	}
	if(queue->unsent_size == 0)
	{
		if(size > 5)
		{
			v_n_send_data(address, data, size);
			queue->seconds = seconds;
			queue->fractions = fractions;
			queue->packet_id++;
/*			printf("balance %u %u %f\n", ack_balance, nak_balance, (float)nak_balance / ((float)ack_balance + (float)nak_balance) * 100);
			printf("in balance %u %u %f\n", in_balance, out_balance, (float)out_balance / ((float)in_balance + (float)out_balance) * 100);
			{
				static uint32 send_seconds, send_fractions;
				FILE *f;
				printf("timer %f\n",(double)(seconds - send_seconds) + ((double)fractions - (double)send_fractions) / (double)(0xffffffff));
				send_seconds = seconds; 
				send_fractions = fractions;
				f = fopen("performance.txt", "wt");
				for(i = 0; i < count / 4; i++)
					fprintf(f, "%f %f %f %f\n", save[i * 4], save[i * 4 + 1], save[i * 4 + 2], save[i * 4 + 3]);
				fclose(f);
			}
*/			return TRUE;
		}
		return FALSE;
	}

	if(queue->unsent_size == 0)
		return FALSE;

	if(count < 10000)
	{
		save[count++] = delta;
		save[count++] = seconds;
		save[count++] = queue->send_rate;
		save[count++] = fractions / (double)(0xffffffff);
	}

	if(delta * queue->send_rate < 1500)
		out_balance++;
	else
		in_balance++;
		

	if(delta * queue->send_rate < 1500)
		return FALSE;

	{
/*		static unsigned int s, new_sec, count;
		v_n_get_current_time(&new_sec, NULL);
		if(s != new_sec)
		{
			s = new_sec;
			printf("this second sent %u pack_id %u rate %u a %u n %u\n", count, queue->packet_id, (uint32)(queue->send_rate / 1500), ack_balance, nak_balance);
			ack_balance = 0;
			nak_balance = 0;
			count = 0;
		}*/
		count++;
	}


/*	while(queue->unsent_size != 0 && delta * queue->send_rate < 1500)
	for(i = 0; queue->unsent_size != 0 && i < 2; i++)
*/	{
		while(queue->unsent_size != 0)
		{
			queue->slot = ((1 + queue->slot) % V_NOQ_OPTIMIZATION_SLOTS);
			buf = queue->unsent[queue->slot];
			if(buf != NULL)
			{
				if(buf->size + size > V_NOQ_MAX_PACKET_SIZE)
					break;
				queue->unsent[queue->slot] = buf->next;
				buf->next = queue->history[queue->slot];
				queue->history[queue->slot] = buf;
				buf->packet = queue->packet_id;

				v_e_encrypt_command(data, size, ((VCMDBuffer1500 *)buf)->buf, buf->size, v_con_get_data_key());

				size += buf->size;

				queue->unsent_size -= buf->size;
				queue->sent_size += buf->size;
			}
		}
		delta -= (double)size / queue->send_rate;
		v_n_send_data(address, data, size);
			
		queue->packet_id++;
		size = vnp_raw_pack_uint32(data, queue->packet_id);
	}
	queue->seconds = seconds;
	queue->fractions = fractions;	
/*	if(delta < 0)
	{
		queue->fractions += delta * (double)(0x7fffffff);
		if(queue->fractions < fractions)
			queue->seconds++;
	}*/
	return TRUE;
}

void v_noq_send_ack_nak_buf(VNetOutQueue *queue, VCMDBufHead *buf)
{
	buf->next = queue->ack_nak;
	queue->ack_nak = buf;
}

extern void v_con_network_listen(void);

void v_noq_send_buf(VNetOutQueue *queue, VCMDBufHead *buf)
{
	static int count = 0;
	VCMDBufHead *b, *last = NULL;
	unsigned int slot;
	slot = buf->address_sum % V_NOQ_OPTIMIZATION_SLOTS;
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
	if(count++ % 256 == 0)
	{
		v_noq_send_queue(queue, v_con_get_network_address());
		v_con_network_listen();	
	}
}
void v_noq_send_buf_fake(VNetOutQueue *queue, VCMDBufHead *buf)
{
	unsigned int slot;
	slot = buf->address_sum % V_NOQ_OPTIMIZATION_SLOTS;
	queue->unsent_size += buf->size;
	buf->next = queue->unsent[slot];
	queue->unsent[slot] = buf;
}

void callback_send_packet_ack(void *user, uint32 packet_id)
{
	VNetOutQueue *queue;
	VCMDBufHead *buf, *last;
	unsigned int slot;
	queue = v_con_get_network_queue();
	queue->send_rate *= V_NOQ_BANDWIDTH_FACTOR_ACK;
	ack_balance++;
	for(slot = 0; slot < V_NOQ_OPTIMIZATION_SLOTS; slot++)
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
	VNetOutQueue *queue;
	VCMDBufHead *buf, *last;
	unsigned int slot;
	queue = v_con_get_network_queue();
	queue->send_rate *= V_NOQ_BANDWIDTH_FACTOR_NAK;
	nak_balance++;
	for(slot = 0; slot < V_NOQ_OPTIMIZATION_SLOTS; slot++)
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
