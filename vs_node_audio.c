/*
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "vs_server.h"

typedef struct {
	void				**data;
	unsigned int		length;
	char				name[16];
	VNALayerType		type;
	real64			frequency;
	VSSubscriptionList  *subscribers;
} VSNLayer;

typedef struct {
	char				name[16];
	VSSubscriptionList  *subscribers;
} VSNStream;

typedef struct{
	VSNodeHead		head;
	VSNLayer		*layers;
	unsigned int	layer_count;
	VSNStream		*streams;
	unsigned int	stream_count;
} VSNodeAudio;

VSNodeAudio * vs_a_create_node(unsigned int owner)
{
	VSNodeAudio *node;
	char name[48];
	unsigned int i;

	node = malloc(sizeof *node);
	vs_add_new_node(&node->head, V_NT_AUDIO);
	sprintf(name, "Audio_Node_%u", node->head.id);
	create_node_head(&node->head, name, owner);
	node->layer_count = 16;
	node->layers = malloc((sizeof *node->layers) * node->layer_count);
	for(i = 0; i < node->layer_count; i++)
		node->layers[i].name[0] = 0;
	node->stream_count = 16;
	node->streams = malloc((sizeof *node->streams) * node->stream_count);
	for(i = 0; i < node->stream_count; i++)
		node->streams[i].name[0] = 0;

	return node;
}

void vs_a_destroy_node(VSNodeAudio *node)
{
	unsigned int i, j;
	destroy_node_head(&node->head);
	
	for(i = 0; i < node->layer_count; i++)
	{
		if(node->layers[i].name[0] != 0)
		{
			for(j = 0; j < node->layers[i].length; j++)
				if(node->layers[i].data[j] != NULL)
					free(node->layers[i].data[j]);
			free(node->layers[i].data);
		}
	}
	free(node->layers);
	free(node->streams);
	free(node);
}

void vs_a_subscribe(VSNodeAudio *node)
{
	unsigned int i;
	if(node == NULL)
		return;
	for(i = 0; i < node->layer_count; i++)
		if(node->layers[i].name[0] != 0)
			verse_send_a_layer_create(node->head.id, i, node->layers[i].type,
						  node->layers[i].frequency, node->layers[i].name);
	for(i = 0; i < node->stream_count; i++)
		if(node->streams[i].name[0] != 0)
			verse_send_a_stream_create(node->head.id, i, node->streams[i].name);
}

void vs_a_unsubscribe(VSNodeAudio *node)
{
	unsigned int i;
	for(i = 0; i < node->layer_count; i++)
		if(node->layers[i].name[0] != 0)	
			vs_remove_subscriptor(node->layers[i].subscribers);
	for(i = 0; i < node->stream_count; i++)
		if(node->streams[i].name[0] != 0)	
			vs_remove_subscriptor(node->streams[i].subscribers);
}

static void callback_send_a_stream_create(void *user, VNodeID node_id, VLayerID stream_id, const char *name)
{
	VSNodeAudio *node;
	unsigned int i, j, count;

	node = (VSNodeAudio *) vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL)
		return;

	for(i = 0; i < node->stream_count; i++)
	{
		if(stream_id != i)
		{
			for(j = 0; name[j] == node->streams[i].name[j] && name[j] != 0; j++);
			if(name[j] == node->streams[i].name[j])
				return;
		}
	}
	if(stream_id >= node->stream_count || node->streams[stream_id].name[0] == 0)
	{
		for(stream_id = 0; stream_id < node->stream_count && node->streams[stream_id].name[0] != 0; stream_id++);
		if(stream_id == node->stream_count)
		{
			stream_id = node->stream_count;
			node->stream_count += 16;
			node->streams = realloc(node->streams, (sizeof *node->streams) * node->stream_count);
			for(i = stream_id; i < node->stream_count; i++)
				node->streams[i].name[0] = 0;		
		}
		node->streams[stream_id].subscribers = vs_create_subscription_list();


	}
	for(i = 0; name[i] != 0 && i < 15; i++)
		node->streams[stream_id].name[i] = name[i];
	node->streams[stream_id].name[i] = 0;
	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_a_stream_create(node_id, stream_id, name);
	}
	vs_reset_subscript_session();
}

static void callback_send_a_stream_destroy(void *user, VNodeID node_id, VLayerID stream_id)
{
	VSNodeAudio *node;
	unsigned int i, count;

	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL || stream_id >= node->stream_count || node->streams[stream_id].name[0] == 0)
		return;
	vs_remove_subscriptor(node->streams[stream_id].subscribers);
	node->streams[stream_id].name[0] = 0;
	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_a_stream_destroy(node_id, stream_id);
	}
	vs_reset_subscript_session();
}

static void callback_send_a_layer_create(void *user, VNodeID node_id, VLayerID layer_id,
					 VNALayerType type, real64 frequency, const char *name)
{
	VSNodeAudio *node;
	unsigned int i, j, count;

	if(frequency < 0.0)
		return;
	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL)
		return;

	for(i = 0; i < node->layer_count; i++)
	{
		if(layer_id != i)
		{
			for(j = 0; name[j] == node->layers[i].name[j] && name[j] != 0; j++);
			if(name[j] == node->layers[i].name[j])
				return;
		}
	}

	if(layer_id < node->layer_count && node->layers[layer_id].name[0] != 0 && type != node->layers[layer_id].type)
	{
		free(node->layers[layer_id].data);
		vs_destroy_subscription_list(node->layers[layer_id].subscribers);
		node->layers[layer_id].name[0] = 0;
	}

	if(layer_id >= node->layer_count || node->layers[layer_id].name[0] == 0)
	{
		for(layer_id = 0; layer_id < node->layer_count && node->layers[layer_id].name[0] != 0; layer_id++);
		if(layer_id == node->layer_count)
		{
			layer_id = node->layer_count;
			node->layer_count += 16;
			node->layers = realloc(node->layers, (sizeof *node->layers) * node->layer_count);
			for(i = layer_id; i < node->layer_count; i++)
				node->layers[i].name[0] = 0;		
		}
		node->layers[layer_id].subscribers = vs_create_subscription_list();
		node->layers[layer_id].type = type;
		node->layers[layer_id].frequency = frequency;
		node->layers[layer_id].length = 64;
		node->layers[layer_id].data = malloc(sizeof(*node->layers[layer_id].data) * node->layers[layer_id].length);
		for(i = 0; i < node->layers[layer_id].length; i++)
			node->layers[layer_id].data[i] = NULL;	
	}
	for(i = 0; name[i] != 0 && i < 15; i++)
		node->layers[layer_id].name[i] = name[i];
	node->layers[layer_id].name[i] = 0;

	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_a_layer_create(node_id, layer_id, type, frequency, name);
	}
	vs_reset_subscript_session();
}

static void callback_send_a_layer_destroy(void *user, VNodeID node_id, VLayerID layer_id)
{
	VSNodeAudio *node;
	unsigned int i, count;

	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL || layer_id >= node->layer_count || node->layers[layer_id].name[0] == 0)
		return;
	vs_remove_subscriptor(node->layers[layer_id].subscribers);
	node->layers[layer_id].name[0] = 0;
	free(node->layers[layer_id].data);
	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_a_layer_destroy(node_id, layer_id);
	}
	vs_reset_subscript_session();
}

static void callback_send_a_layer_subscribe(void *user, VNodeID node_id, VLayerID layer_id)
{
	VSNodeAudio *node;
	unsigned int i;

	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL)
		return;
	if(node->layer_count <= layer_id)
		return;
	if(node->layers[layer_id].name[0] == 0)
		return;
	vs_add_new_subscriptor(node->layers[layer_id].subscribers);
	for(i = 0; i < node->layers[layer_id].length; i++)
	{
		if(node->layers[layer_id].data[i] != NULL)
			verse_send_a_block_set(node_id, layer_id, i, node->layers[layer_id].type, node->layers[layer_id].data[i]);
	}
}

static void callback_send_a_layer_unsubscribe(void *user, VNodeID node_id, VLayerID layer_id)
{
	VSNodeAudio *node;
	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL)
		return;
	if(node->layer_count <= layer_id)
		return;
	if(node->layers[layer_id].name[0] == 0)
		return;
	vs_remove_subscriptor(node->layers[layer_id].subscribers);
}

static void callback_send_a_block_set(void *user, VNodeID node_id, VLayerID layer_id, uint32 id, VNALayerType type, void *data)
{
	static const size_t	blocksize[] = {
		VN_A_BLOCK_SIZE_INT8   * sizeof (int8),
		VN_A_BLOCK_SIZE_INT16  * sizeof (int16),
		VN_A_BLOCK_SIZE_INT24  * 3 * sizeof (int8),
		VN_A_BLOCK_SIZE_INT32  * sizeof (int32),
		VN_A_BLOCK_SIZE_REAL32 * sizeof (real32),
		VN_A_BLOCK_SIZE_REAL64 * sizeof (real64) 
	};
	VSNodeAudio *node;
	unsigned int i, count;

	if(type < 0 || type > VN_A_LAYER_REAL64)	/* Protect blocksize array. */
		return;

	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL)
		return;
	if(node->layers[layer_id].name[0] == 0)
		return;
	if(id > node->layers[layer_id].length)
	{
		node->layers[layer_id].data = realloc(node->layers[layer_id].data, (sizeof *node->layers[layer_id].data) * (id + 64));
		for(i = node->layers[layer_id].length; i < id + 64; i++)
			node->layers[layer_id].data[i] = NULL;
		node->layers[layer_id].length = id + 64;
	}

	if(node->layers[layer_id].data[id] == NULL)
		node->layers[layer_id].data[id] = malloc(blocksize[type]);
	if(node->layers[layer_id].data[id] != NULL)
	{
		memcpy(node->layers[layer_id].data[id], data, blocksize[type]);
		count =	vs_get_subscript_count(node->layers[layer_id].subscribers);
		for(i = 0; i < count; i++)
		{
			vs_set_subscript_session(node->layers[layer_id].subscribers, i);
			verse_send_a_block_set(node_id, layer_id, id, type, data);
		}
		vs_reset_subscript_session();
	}
}

static void callback_send_a_block_clear(void *user, VNodeID node_id, VLayerID layer_id, uint32 id)
{
	VSNodeAudio *node;
	unsigned int i, count;
	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL)
		return;
	if(node->layer_count <= layer_id)
		return;
	if(node->layers[layer_id].name[0] == 0)
		return;
	if(id >= node->layers[layer_id].length)
		return;
	if(node->layers[layer_id].data[id] == NULL)
		return;
	free(node->layers[layer_id].data[id]);
	node->layers[layer_id].data[id] = NULL;
	count =	vs_get_subscript_count(node->layers[layer_id].subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->layers[layer_id].subscribers, i);
		verse_send_a_block_clear(node_id, layer_id, id);
	}
	vs_reset_subscript_session();
}

static void callback_send_a_stream_subscribe(void *user, VNodeID node_id, VLayerID stream_id)
{
	VSNodeAudio *node;
	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL)
		return;
	if(node->stream_count <= stream_id)
		return;
	if(node->streams[stream_id].name[0] == 0)
		return;
	vs_add_new_subscriptor(node->streams[stream_id].subscribers);
}

static void callback_send_a_stream_unsubscribe(void *user, VNodeID node_id, VLayerID stream_id)
{
	VSNodeAudio *node;
	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL)
		return;
	if(node->stream_count <= stream_id)
		return;
	if(node->streams[stream_id].name[0] == 0)
		return;
	vs_remove_subscriptor(node->streams[stream_id].subscribers);
}

static void callback_send_a_stream(void *user, VNodeID node_id, VLayerID stream_id, uint16 id, uint32 time_s, uint32 time_f,
				   VNALayerType type, real64 frequency, void *data)
{
	VSNodeAudio *node;
	unsigned int i, count;

	if(frequency < 0)
		return;
	node = (VSNodeAudio *)vs_get_node(node_id, V_NT_AUDIO);
	if(node == NULL)
		return;
	if(node->stream_count <= stream_id)
		return;
	if(node->streams[stream_id].name[0] == 0)
		return;
	count =	vs_get_subscript_count(node->streams[stream_id].subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->streams[stream_id].subscribers, i);
		verse_send_a_stream(node_id, stream_id, id, time_s, time_f, type, frequency, data);
	}
	vs_reset_subscript_session();
}

void vs_a_callback_init(void)
{
	verse_callback_set(verse_send_a_layer_create,		callback_send_a_layer_create,		NULL);
	verse_callback_set(verse_send_a_layer_destroy,		callback_send_a_layer_destroy,		NULL);
	verse_callback_set(verse_send_a_layer_subscribe,	callback_send_a_layer_subscribe,	NULL);
	verse_callback_set(verse_send_a_layer_unsubscribe,	callback_send_a_layer_unsubscribe,	NULL);
	verse_callback_set(verse_send_a_block_set,		callback_send_a_block_set,		NULL);
	verse_callback_set(verse_send_a_block_clear,		callback_send_a_block_clear,		NULL);
	verse_callback_set(verse_send_a_stream_create,		callback_send_a_stream_create,		NULL);
	verse_callback_set(verse_send_a_stream_destroy,		callback_send_a_stream_destroy,		NULL);
	verse_callback_set(verse_send_a_stream_subscribe,	callback_send_a_stream_subscribe,	NULL);
	verse_callback_set(verse_send_a_stream_unsubscribe,	callback_send_a_stream_unsubscribe,	NULL);
	verse_callback_set(verse_send_a_stream,			callback_send_a_stream,			NULL);
}

#endif
