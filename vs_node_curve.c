/*
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "vs_server.h"

typedef struct {
	real64 pre_value;
	uint32 pre_pos;
	real64 value;
	real64 post_value;
	uint32 post_pos;
} VSNKey;

typedef struct {
	VSNKey		*keys;
	unsigned int	length;
	char		name[16];
	VSSubscriptionList *subscribers;
} VSNCurve;

typedef struct{
	VSNodeHead	head;
	VSNCurve	*curves;
	unsigned int	curve_count;
} VSNodeCurve;

VSNodeCurve * vs_c_create_node(unsigned int owner)
{
	VSNodeCurve *node;
	char name[48];
	unsigned int i;
	node = malloc(sizeof *node);
	vs_add_new_node(&node->head, V_NT_CURVE);
	sprintf(name, "Curve_Node_%u", node->head.id);
	create_node_head(&node->head, name, owner);
	node->curves = malloc((sizeof *node->curves) * 16);
	node->curve_count = 16;
	for(i = 0; i < 16; i++)
		node->curves[i].name[0] = 0;
	return node;
}

void vs_c_destroy_node(VSNodeCurve *node)
{
	destroy_node_head(&node->head);
	free(node);
}

void vs_c_subscribe(VSNodeCurve *node)
{
	unsigned int i;
	if(node == NULL)
		return;
	for(i = 0; i < node->curve_count; i++)
		if(node->curves[i].name[0] == 0)
			verse_send_c_curve_create(node->head.id, i, node->curves[i].name);

}

void callback_send_c_unsubscribe(void *user, VNodeID node_id)
{
	VSNodeCurve *node;
	unsigned int i;
	node = (VSNodeCurve *)vs_get_node(node_id, V_NT_CURVE);
	if(node == NULL)
		return;

	for(i = 0; i < node->curve_count; i++)
		if(node->curves[i].name[0] != 0)	
			vs_remove_subscriptor(node->curves[i].subscribers);
	vs_remove_subscriptor(node->head.subscribers);
}

static void callback_send_c_curve_create(void *user, VNodeID node_id, VLayerID curve_id, const char *name)
{
	VSNodeCurve *node;
	unsigned int i, j, count;
	node = (VSNodeCurve *)vs_get_node(node_id, V_NT_CURVE);
	if(node == NULL || node->curve_count >= curve_id)
		return;

	for(i = 0; i < node->curve_count; i++)
	{
		for(j = 0; name[j] == node->curves[i].name[j] && name[j] != 0; j++);
		if(name[j] == node->curves[i].name[j])
			return;
	}
	if(curve_id >= node->curve_count || node->curves[curve_id].name[0] == 0)
	{
		for(curve_id = 0; curve_id < node->curve_count && node->curves[curve_id].name[0] != 0; curve_id++);
		if(curve_id == node->curve_count)
		{
			curve_id = node->curve_count;
			node->curve_count += 16;
			node->curves = realloc(node->curves, (sizeof *node->curves) * node->curve_count);
			for(i = curve_id; i < node->curve_count; i++)
				node->curves[i].name[0] = 0;		
		}
		node->curves[curve_id].subscribers = vs_create_subscription_list();
		node->curves[curve_id].length = 64;
		node->curves[curve_id].keys = malloc((sizeof *node->curves[curve_id].keys) * 64);
		for(i = 0; name[i] != 0 && i < 64; i++)
			node->curves[curve_id].keys[i].value = V_REAL64_MAX;			
		for(i = 0; name[i] != 0 && i < 15; i++)
			node->curves[curve_id].name[i] = name[i];
		node->curves[curve_id].name[i] = 0;
	}

	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_c_curve_create(node_id, curve_id, name);
	}
	vs_reset_subscript_session();
}

static void callback_send_c_curve_destroy(void *user, VNodeID node_id, VLayerID curve_id)
{
	VSNodeCurve *node;
	unsigned int i, count;
	node = (VSNodeCurve *)vs_get_node(node_id, V_NT_CURVE);
	if(node == NULL || node->curve_count >= curve_id || node->curves[curve_id].name[0] == 0)
		return;
	vs_remove_subscriptor(node->curves[curve_id].subscribers);
	node->curves[curve_id].name[0] = 0;
	free(node->curves[curve_id].keys);
	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_c_curve_destroy(node_id, curve_id);
	}
	vs_reset_subscript_session();
}

static void callback_send_c_curve_key_set(void *user, VNodeID node_id, VLayerID curve_id, uint32 key_id, real64 pre_value, uint32 pre_pos, real64 value, real64 post_value, uint32 post_pos)
{
	VSNodeCurve *node;
	unsigned int i, count;
	node = (VSNodeCurve *)vs_get_node(node_id, V_NT_CURVE);
	if(node == NULL || node->curve_count >= curve_id || node->curves[curve_id].name[0] == 0)
		return;

	if(node->curves[curve_id].length >= key_id || node->curves[curve_id].keys[key_id].value == V_REAL64_MAX)
	{
		for(key_id = 0; key_id < node->curves[curve_id].length && node->curves[curve_id].keys[key_id].value != V_REAL64_MAX; key_id++);
		if(key_id == node->curves[curve_id].length)
			for(i = 0; i < 64; i++)
				node->curves[curve_id].keys[node->curves[curve_id].length++].value = V_REAL64_MAX;
	}

	node->curves[curve_id].keys[key_id].pre_value = pre_value;
	node->curves[curve_id].keys[key_id].pre_pos = pre_pos;
	node->curves[curve_id].keys[key_id].value = value;
	node->curves[curve_id].keys[key_id].post_value = post_value;
	node->curves[curve_id].keys[key_id].post_pos = post_pos;

	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_c_curve_key_set(node_id, curve_id, key_id, pre_value, pre_pos, value, post_value, post_pos);
	}
	vs_reset_subscript_session();
}

static void callback_send_c_curve_key_destroy(void *user, VNodeID node_id, VLayerID curve_id, uint32 key_id)
{
	VSNodeCurve *node;
	unsigned int i, count;
	node = (VSNodeCurve *)vs_get_node(node_id, V_NT_CURVE);
	if(node == NULL || node->curve_count >= curve_id || node->curves[curve_id].name[0] == 0)
		return;

	if(node->curves[curve_id].length <= key_id || node->curves[curve_id].keys[key_id].value == V_REAL64_MAX)
		return;

	node->curves[curve_id].keys[key_id].value = V_REAL64_MAX;

	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_c_curve_key_destroy(node_id, curve_id, key_id);
	}
	vs_reset_subscript_session();
}

static void callback_send_c_curve_subscribe(void *user, VNodeID node_id, VLayerID curve_id)
{
	VSNodeCurve *node;
	unsigned int i;
	node = (VSNodeCurve *)vs_get_node(node_id, V_NT_CURVE);
	if(node == NULL || node->curve_count >= curve_id || node->curves[curve_id].name[0] == 0)
		return;

	vs_add_new_subscriptor(node->curves[curve_id].subscribers);

	for(i = 0; i < node->curves[curve_id].length; i++)
		if(node->curves[curve_id].keys[i].value != V_REAL64_MAX)
			verse_send_c_curve_key_set(node_id, curve_id, i, node->curves[curve_id].keys[i].pre_value, node->curves[curve_id].keys[i].pre_pos, node->curves[curve_id].keys[i].value, node->curves[curve_id].keys[i].post_value, node->curves[curve_id].keys[i].post_pos);
}

static void callback_send_c_curve_unsubscribe(void *user, VNodeID node_id, VLayerID curve_id)
{
	VSNodeCurve *node;
	node = (VSNodeCurve *)vs_get_node(node_id, V_NT_CURVE);
	if(node == NULL || node->curve_count >= curve_id || node->curves[curve_id].name[0] == 0)
		return;
	vs_remove_subscriptor(node->curves[curve_id].subscribers);
}

void vs_c_callback_init()
{
	verse_callback_set(verse_send_c_curve_create,		callback_send_c_curve_create,		NULL);
	verse_callback_set(verse_send_c_curve_destroy,		callback_send_c_curve_destroy,		NULL);
	verse_callback_set(verse_send_c_curve_subscribe,	callback_send_c_curve_subscribe,	NULL);
	verse_callback_set(verse_send_c_curve_unsubscribe,	callback_send_c_curve_unsubscribe,	NULL);
	verse_callback_set(verse_send_c_curve_key_set,		callback_send_c_curve_key_set,		NULL);
	verse_callback_set(verse_send_c_curve_key_destroy,	callback_send_c_curve_key_destroy,	NULL);
}

#endif
