/*
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "vs_server.h"

static unsigned int number_of_tiles = 0;
static unsigned int got_number_of_tiles = 0;

typedef struct {
	VNBLayerType		type;
	char			name[16];
	void			*layer;
	VSSubscriptionList	*subscribers;
} VSNBLayers;

typedef struct {
	VSNodeHead	head;
	uint16		size[3];
	VSNBLayers	*layers;
	unsigned int	layer_count;
} VSNodeBitmap;

VSNodeBitmap * vs_b_create_node(unsigned int owner)
{
	VSNodeBitmap *node;
	char name[48];
	node = malloc(sizeof *node);
	vs_add_new_node(&node->head, V_NT_BITMAP);
	sprintf(name, "Bitmap_Node_%u", node->head.id);
	create_node_head(&node->head, name, owner);

	node->size[0] = 0;
	node->size[1] = 0;
	node->size[2] = 0;
	node->layers = NULL;
	node->layer_count = 0;

	return node;
}

void vs_b_destroy_node(VSNodeBitmap *node)
{
	unsigned int i;
	destroy_node_head(&node->head);
	if(node->layers != NULL)
	{
		for(i = 0; i < node->layer_count; i++)
			if(node->layers[i].layer != NULL)
				free(node->layers[i].layer);
		free(node->layers);
	}
	free(node);
}

void vs_b_subscribe(VSNodeBitmap *node)
{
	unsigned int i;
	verse_send_b_init_dimensions(node->head.id, node->size[0], node->size[1], node->size[2]);
	for(i = 0; i < node->layer_count; i++)
		if(node->layers[i].layer != NULL)
			verse_send_b_layer_create(node->head.id, (uint16)i, node->layers[i].name, (uint8)node->layers[i].type);
}

void callback_send_b_unsubscribe(void *user, VNodeID node_id)
{
	VSNodeBitmap *node;
	node = (VSNodeBitmap *)vs_get_node(node_id, V_NT_BITMAP);
	if(node == NULL)
		return;
	vs_remove_subscriptor(node->head.subscribers);
}

static void callback_send_b_init_dimensions(void *user, VNodeID node_id, uint16 width, uint16 height, uint16 depth)
{
	VSNodeBitmap *node;
	unsigned int i, count;
	if((node = (VSNodeBitmap *)vs_get_node(node_id, V_NT_BITMAP)) == NULL)
		return;
	if(node->size[0] != 0 || width == 0 || height == 0 || depth == 0)
		return;
	node->size[0] = width;
	node->size[1] = height;
	node->size[2] = depth;
	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_b_init_dimensions(node_id, width, height, depth);
	}
	vs_reset_subscript_session();
	number_of_tiles = 0;
	got_number_of_tiles = 0;
}

static void callback_send_b_layer_create(void *user, VNodeID node_id, VLayerID layer_id, char *name, uint8 type)
{
	VSNodeBitmap *node;
	unsigned int i, count;
	if((node = (VSNodeBitmap *)vs_get_node(node_id, V_NT_BITMAP)) == NULL)
		return;
	if(node->size[0] == 0)
		return;

	if(node->layer_count <= layer_id|| node->layers[layer_id].name[0] == 0)
	{
		for(layer_id = 0; layer_id < node->layer_count && node->layers[layer_id].layer != NULL; layer_id++);
		if(layer_id == node->layer_count)
		{
			node->layers = realloc(node->layers, (sizeof *node->layers) * (node->layer_count + 16));
			for(i = node->layer_count; i < node->layer_count + 16; i++)
			{
				node->layers[i].layer = NULL;
				node->layers[i].type = 0;
				node->layers[i].name[0] = 0;
				node->layers[i].subscribers = NULL;
			}
			node->layer_count += 16;
		}
		node->layers[layer_id].subscribers = vs_create_subscription_list();
		node->layers[layer_id].type = type + 1;
	}
	for(i = 0; i < 16; i++)
		node->layers[layer_id].name[i] = name[i];
	if(node->layers[layer_id].type != type)
	{
		node->layers[layer_id].type = type;
		count = ((node->size[0] + VN_B_TILE_SIZE - 1) / VN_B_TILE_SIZE) * ((node->size[1] + VN_B_TILE_SIZE - 1) / VN_B_TILE_SIZE);
		count *= VN_B_TILE_SIZE * VN_B_TILE_SIZE * node->size[2];
		switch(type)
		{
			case VN_B_LAYER_INTEGER1 :
				node->layers[layer_id].layer = malloc(sizeof(uint8) * count / 8);
				for(i = 0; i < count / 8; i++)
					((uint8 *)node->layers[layer_id].layer)[i] = 0;
			break;
			case VN_B_LAYER_INTEGER8 :
				node->layers[layer_id].layer = malloc(sizeof(uint8) * count);
				for(i = 0; i < count; i++)
					((uint8 *)node->layers[layer_id].layer)[i] = 0;
			break;
			case VN_B_LAYER_INTEGER16 :
				node->layers[layer_id].layer = malloc(sizeof(uint16) * count);
				for(i = 0; i < count; i++)
					((uint16 *)node->layers[layer_id].layer)[i] = 0;
			break;
			case VN_B_LAYER_REAL32 :
				node->layers[layer_id].layer = malloc(sizeof(uint32) * count);
				for(i = 0; i < count; i++)
					((uint32 *)node->layers[layer_id].layer)[i] = 0;
			break;
			case VN_B_LAYER_REAL64 :
				node->layers[layer_id].layer = malloc(sizeof(double) * count);
				for(i = 0; i < count; i++)
					((double *)node->layers[layer_id].layer)[i] = 0.0;
			break;
		}
	}
	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_b_layer_create(node_id, layer_id, name, type);
	}
	vs_reset_subscript_session();
}

static void callback_send_b_layer_destroy(void *user, VNodeID node_id, VLayerID layer_id)
{
	VSNodeBitmap *node;
	unsigned int i, count;
	if((node = (VSNodeBitmap *)vs_get_node(node_id, V_NT_BITMAP)) == NULL)
		return;
	if(layer_id >= node->layer_count || node->layers[layer_id].layer == NULL)
		return;
	free(node->layers[layer_id].layer);
	node->layers[layer_id].layer = NULL;
	node->layers[layer_id].type = 0;
	node->layers[layer_id].name[0] = 0;
	vs_destroy_subscription_list(node->layers[layer_id].subscribers);
	node->layers[layer_id].subscribers = NULL;

	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_b_layer_destroy(node_id, layer_id);
	}
	vs_reset_subscript_session();
}

static void callback_send_b_layer_subscribe(void *user, VNodeID node_id, VLayerID layer_id, uint8 level)
{
	VSNodeBitmap *node;
	unsigned int i, j, k, tile[3];
	if((node = (VSNodeBitmap *)vs_get_node(node_id, V_NT_BITMAP)) == NULL)
		return;
	if(layer_id >= node->layer_count || node->layers[layer_id].layer == NULL)
		return;
	tile[0] = ((node->size[0] + VN_B_TILE_SIZE - 1) / VN_B_TILE_SIZE);
	tile[1] = ((node->size[1] + VN_B_TILE_SIZE - 1) / VN_B_TILE_SIZE);
	tile[2] = node->size[2];
	switch(node->layers[layer_id].type)
	{
		case VN_B_LAYER_INTEGER1 :	
			for(i = 0; i < tile[0]; i++)
				for(j = 0; j < tile[1]; j++)
					for(k = 0; k < tile[2]; k++)
						verse_send_b_layer_set_tile(node_id, layer_id, (uint16)i, (uint16)j, (uint16)k, VN_B_LAYER_INTEGER1, &((uint8 *)node->layers[layer_id].layer)[(tile[0]*tile[1]*k + j*tile[0] + i)*VN_B_TILE_SIZE*VN_B_TILE_SIZE/8]);
		break;
		case VN_B_LAYER_INTEGER8 :
			for(i = 0; i < tile[0]; i++)
				for(j = 0; j < tile[1]; j++)
					for(k = 0; k < tile[2]; k++)
						verse_send_b_layer_set_tile(node_id, layer_id, (uint16)i, (uint16)j, (uint16)k, VN_B_LAYER_INTEGER8, &((uint8 *)node->layers[layer_id].layer)[(tile[0]*tile[1]*k + j*tile[0] + i)*VN_B_TILE_SIZE*VN_B_TILE_SIZE]);
		break;
		case VN_B_LAYER_INTEGER16 :
			for(i = 0; i < tile[0]; i++)
				for(j = 0; j < tile[1]; j++)
					for(k = 0; k < tile[2]; k++)
						verse_send_b_layer_set_tile(node_id, layer_id, (uint16)i, (uint16)j, (uint16)k, VN_B_LAYER_INTEGER16, &((uint16 *)node->layers[layer_id].layer)[(tile[0]*tile[1]*k + j*tile[0] + i)*VN_B_TILE_SIZE*VN_B_TILE_SIZE]);
		break;
		case VN_B_LAYER_REAL32 :
			for(i = 0; i < tile[0]; i++)
				for(j = 0; j < tile[1]; j++)
					for(k = 0; k < tile[2]; k++)
						verse_send_b_layer_set_tile(node_id, layer_id, (uint16)i, (uint16)j, (uint16)k, VN_B_LAYER_REAL32, &((float *)node->layers[layer_id].layer)[(tile[0]*tile[1]*k + j*tile[0] + i)*VN_B_TILE_SIZE*VN_B_TILE_SIZE]);
		break;
		case VN_B_LAYER_REAL64 :
			for(i = 0; i < tile[0]; i++)
				for(j = 0; j < tile[1]; j++)
					for(k = 0; k < tile[2]; k++)
						verse_send_b_layer_set_tile(node_id, layer_id, (uint16)i, (uint16)j, (uint16)k, VN_B_LAYER_REAL64, &((double *)node->layers[layer_id].layer)[(tile[0]*tile[1]*k + j*tile[0] + i)*VN_B_TILE_SIZE*VN_B_TILE_SIZE]);
		break;
	}
}

static void callback_send_b_layer_unsubscribe(void *user, VNodeID node_id, VLayerID layer_id)
{
	VSNodeBitmap *node;
	if((node = (VSNodeBitmap *)vs_get_node(node_id, V_NT_BITMAP)) == NULL)
		return;
	if(layer_id >= node->layer_count || node->layers[layer_id].layer == NULL)
		return;
	vs_remove_subscriptor(node->layers[layer_id].subscribers);
}

static void callback_send_b_layer_set_tile(void *user, VNodeID node_id, VLayerID layer_id, uint16 tile_x, uint16 tile_y, uint16 tile_z, uint8 type, void *data)
{
	VSNodeBitmap *node;
	unsigned int i, count, tile[3];
	if((node = (VSNodeBitmap *)vs_get_node(node_id, V_NT_BITMAP)) == NULL)
		return;
	if(layer_id >= node->layer_count || node->layers[layer_id].layer == NULL || node->layers[layer_id].type != type)
		return;
	got_number_of_tiles++;
	if(tile_x >= ((node->size[0] + VN_B_TILE_SIZE - 1) / VN_B_TILE_SIZE) || tile_y >= ((node->size[1] + VN_B_TILE_SIZE - 1) / VN_B_TILE_SIZE) || tile_z >= node->size[2])
		return;
	number_of_tiles++;
	tile[0] = ((node->size[0] + VN_B_TILE_SIZE - 1) / VN_B_TILE_SIZE);
	tile[1] = ((node->size[1] + VN_B_TILE_SIZE - 1) / VN_B_TILE_SIZE);
	tile[2] = node->size[2];

	switch(node->layers[layer_id].type)
	{
		case VN_B_LAYER_INTEGER1 :
		{
			uint8 *p;
			p = &((uint8 *)node->layers[layer_id].layer)[(tile[0] * tile[1] * tile_z + tile_y * tile[0] + tile_x) * VN_B_TILE_SIZE * VN_B_TILE_SIZE/8];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE / 8; i++)
				p[i] = ((uint8 *)data)[i];
		}
		break;
		case VN_B_LAYER_INTEGER8 :
		{
			uint8 *p;
			p = &((uint8 *)node->layers[layer_id].layer)[(tile[0] * tile[1] * tile_z + tile_y * tile[0] + tile_x) * VN_B_TILE_SIZE * VN_B_TILE_SIZE];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
				p[i] = ((uint8 *)data)[i];
		}
		break;
		case VN_B_LAYER_INTEGER16 :
		{
			uint16 *p;
			p = &((uint16 *)node->layers[layer_id].layer)[(tile[0] * tile[1] * tile_z + tile_y * tile[0] + tile_x) * VN_B_TILE_SIZE * VN_B_TILE_SIZE * 2];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
				p[i] = ((uint16 *)data)[i];
		}
		break;
		case VN_B_LAYER_REAL32 :
		{
			float *p;
			p = &((float *)node->layers[layer_id].layer)[(tile[0] * tile[1] * tile_z + tile_y * tile[0] + tile_x) * VN_B_TILE_SIZE * VN_B_TILE_SIZE * 4];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
				p[i] = ((float *)data)[i];
		}
		break;
		case VN_B_LAYER_REAL64 :
		{
			double *p;
			p = &((double *)node->layers[layer_id].layer)[(tile[0] * tile[1] * tile_z + tile_y * tile[0] + tile_x) * VN_B_TILE_SIZE * VN_B_TILE_SIZE * 8];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
				p[i] = ((double *)data)[i];
		}
		break;
	}
	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_b_layer_set_tile(node_id, layer_id, tile_x, tile_y, tile_z, type, data);
	}
	vs_reset_subscript_session();
}

void vs_b_callback_init()
{
	verse_callback_set(verse_send_b_init_dimensions, callback_send_b_init_dimensions, NULL);
	verse_callback_set(verse_send_b_layer_create, callback_send_b_layer_create, NULL);
	verse_callback_set(verse_send_b_layer_destroy, callback_send_b_layer_destroy, NULL);
	verse_callback_set(verse_send_b_layer_subscribe, callback_send_b_layer_subscribe, NULL);
	verse_callback_set(verse_send_b_layer_unsubscribe, callback_send_b_layer_unsubscribe, NULL);
	verse_callback_set(verse_send_b_layer_set_tile, callback_send_b_layer_set_tile, NULL);
}

#endif
