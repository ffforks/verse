
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "verse_header.h"

#include "v_cmd_buf.h"
#include "v_cmd_gen.h"
#include "v_connection.h"
#include "v_internal_verse.h"
#include "v_network.h"
#include "v_pack.h"

#define V_CONNECTON_TIME_OUT 20

#if !defined(V_GENERATE_FUNC_MODE)

#include "v_network_in_que.h"

typedef struct{
	void	*newer;
	void	*older;
	char	data[1500];
	size_t	size;
}NetPacked;

static NetPacked *v_niq_temp = NULL;

void v_niq_clear(VNetInQueue *queue)
{
	queue->oldest = NULL;
	queue->newest = NULL;
	queue->packet_id = 2;
	v_n_get_current_time(&queue->seconds, NULL);
}

boolean v_niq_time_out(const VNetInQueue *queue)
{
	uint32 seconds;
	v_n_get_current_time(&seconds, NULL);
	return queue->seconds + V_CONNECTON_TIME_OUT < seconds;
}

char *v_niq_get(VNetInQueue *queue, size_t *length)
{
	NetPacked	*p;
	if(queue->oldest == NULL)
	{
		*length = 0;
		return NULL;
	}
	p = queue->oldest;
	queue->oldest = p->newer;
	if(queue->oldest == NULL)
		queue->newest = NULL;
	else
		((NetPacked *)queue->oldest)->older = NULL;

	*length = p->size;

	p->older = v_niq_temp;
	v_niq_temp = p;
	return p->data;
}

char *v_niq_store(VNetInQueue *queue, size_t length, unsigned int packet_id)
{
	NetPacked	*p;
	v_n_get_current_time(&queue->seconds, NULL);

	if(packet_id < queue->packet_id)
		return NULL;
	while(queue->packet_id < packet_id)
		verse_send_packet_nak(queue->packet_id++);
	queue->packet_id++;
	verse_send_packet_ack(packet_id);

	if(v_niq_temp == NULL)
		p = malloc(sizeof *p);
	else
	{
		p = v_niq_temp;
		v_niq_temp = p->older;
	}
	p->older = queue->newest;
	p->newer = NULL;

	if(queue->newest == NULL)
		queue->oldest = p;
	else
		((NetPacked *)queue->newest)->newer = p;
	queue->newest = p;
	p->size = length;
	return p->data;
}

#endif
