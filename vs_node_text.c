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
	char			name[16];
	char			**lines;
	unsigned int		allocated;
	VSSubscriptionList	*subscribers;
} VSTextBuffer;

typedef struct {
	VSNodeHead	head;
	char		language[512];
	VSTextBuffer	*buffer;
	unsigned int	buffer_count;
} VSNodeText;

VSNodeText * vs_t_create_node(unsigned int owner)
{
	VSNodeText *node;
	char name[48];
	unsigned int i;
	node = malloc(sizeof *node);
	vs_add_new_node(&node->head, V_NT_TEXT);
	sprintf(name, "Text_Node_%u", node->head.id);
	create_node_head(&node->head, name, owner);
	node->language[0] = 0;
	node->buffer_count = 16;
	node->buffer = malloc((sizeof *node->buffer) * node->buffer_count);
	for(i = 0; i < node->buffer_count; i++)
		node->buffer[i].name[0] = 0;

	return node;
}

void vs_t_destroy_node(VSNodeText *node)
{
	unsigned int i, j;
	destroy_node_head(&node->head);
	free(node);
	for(i = 0; i < node->buffer_count; i++)
	{
		if(node->buffer[i].name[0] != 0)
		{
			for(j = 0; j < node->buffer[i].allocated && node->buffer[i].lines[j] != NULL; j++)
				free(node->buffer[i].lines[j]);
			free(node->buffer[i].lines);
			vs_destroy_subscription_list(node->buffer[i].subscribers);
		}
	}
	free(node->buffer);
}

void vs_t_subscribe(VSNodeText *node)
{
	unsigned int i;
	verse_send_t_set_language(node->head.id, node->language);
	for(i = 0; i < node->buffer_count; i++)
		if(node->buffer[i].name[0] != 0)
			verse_send_t_buffer_create(node->head.id, i, 0, node->buffer[i].name);
}

static void callback_send_t_unsubscribe(void *user, VNodeID node_id)
{
	VSNodeText *node;
	unsigned int i;
	node = (VSNodeText *)vs_get_node(node_id, V_NT_TEXT);
	if(node == NULL)
		return;
	vs_remove_subscriptor(node->head.subscribers);
	for(i = 0; i < node->buffer_count; i++)
		if(node->buffer[i].name[0] != 0)
			vs_remove_subscriptor(node->buffer[i].subscribers);
}

static void callback_send_t_set_language(void *user, VNodeID node_id, char *language)
{
	VSNodeText *node;
	unsigned int i, count;
	node = (VSNodeText *)vs_get_node(node_id, V_NT_TEXT);
	if(node == NULL)
		return;
	for(i = 0; i < 511 && language[i]; i++)
		node->language[i] = language[i];
	node->language[i] = 0;
	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_t_set_language(node_id, language);
	}
	vs_reset_subscript_session();

}

static void callback_send_t_buffer_create(void *user, VNodeID node_id, VNMBufferID buffer_id, uint16 index, char *name)
{
	VSNodeText *node;
	unsigned int i, count;
	node = (VSNodeText *)vs_get_node(node_id, V_NT_TEXT);
	if(node == NULL)
		return;

	if(buffer_id < node->buffer_count && node->buffer[buffer_id].name[0] != 0)
	{
		for(buffer_id = 0; buffer_id < node->buffer_count && node->buffer[buffer_id].name[0] != 0; buffer_id++)
		if(buffer_id == node->buffer_count)
		{
			node->buffer_count += 16; 
			node->buffer = realloc(node->buffer, (sizeof *node->buffer) * node->buffer_count);
			for(i = node->buffer_count - 16; i < node->buffer_count; i++)
				node->buffer[i].name[0] = 0;
		}
	}	
	for(i = 0; i < 15 && node->buffer[buffer_id].name[i]; i++)
		node->buffer[buffer_id].name[i] = node->buffer[buffer_id].name[i];
	node->buffer[buffer_id].name[i] = 0;
	if(node->buffer[buffer_id].name[0] == 0)
	{
		node->buffer[buffer_id].allocated = 256;
		node->buffer[buffer_id].lines = malloc((sizeof *node->buffer[buffer_id].lines) * 256);
		for(i = 0; i < 256; i++)
			node->buffer[buffer_id].lines[i] = NULL;
		node->buffer[buffer_id].subscribers = vs_create_subscription_list();
	}

	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_t_buffer_create(node_id, buffer_id, 0, name);
	}
	vs_reset_subscript_session();
}

void callback_send_t_buffer_destroy(void *user, VNodeID node_id, VNMBufferID buffer_id)
{
	VSNodeText *node;
	unsigned int i, count;
	node = (VSNodeText *)vs_get_node(node_id, V_NT_TEXT);
	if(node == NULL)
		return;

	if(buffer_id >= node->buffer_count || node->buffer[buffer_id].name[0] == 0)
		return;

	node->buffer[buffer_id].name[0] = 0;
	for(i = 0; i < node->buffer[buffer_id].allocated; i++)
		if(node->buffer[buffer_id].lines[i] != NULL)
			free(node->buffer[buffer_id].lines[i]);
	free(node->buffer[buffer_id].lines);
	vs_destroy_subscription_list(node->buffer[i].subscribers);

	count =	vs_get_subscript_count(node->head.subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->head.subscribers, i);
		verse_send_t_buffer_destroy(node_id, buffer_id);
	}
	vs_reset_subscript_session();
}

static void callback_send_t_buffer_subscribe(void *user, VNodeID node_id, VNMBufferID buffer_id)
{
	VSNodeText *node;
	unsigned int i;
	node = (VSNodeText *)vs_get_node(node_id, V_NT_TEXT);
	if(node == NULL)
		return;
	if(buffer_id >= node->buffer_count || node->buffer[buffer_id].name[0] == 0)
		return;
	vs_add_new_subscriptor(node->buffer[buffer_id].subscribers);
	for(i = 0; i < node->buffer[buffer_id].allocated && node->buffer[buffer_id].lines[i] == NULL; i++)
		verse_send_t_insert_line(node_id, buffer_id, i, 0, node->buffer[buffer_id].lines[i]);
}

static void callback_send_t_buffer_unsubscribe(void *user, VNodeID node_id, VNMBufferID buffer_id)
{
	VSNodeText *node;
	node = (VSNodeText *)vs_get_node(node_id, V_NT_TEXT);
	if(node == NULL)
		return;
	if(buffer_id >= node->buffer_count || node->buffer[buffer_id].name[0] == 0)
		return;
	vs_remove_subscriptor(node->buffer[buffer_id].subscribers);
}

static void callback_send_t_insert_line(void *user, VNodeID node_id, VNMBufferID buffer_id, uint32 line, uint16 index, char *name)
{
	VSNodeText *node;
	unsigned int i, count;
	char *buf;
	node = (VSNodeText *)vs_get_node(node_id, V_NT_TEXT);
	if(node == NULL)
		return;

	if(buffer_id >= node->buffer_count || node->buffer[buffer_id].name[0] == 0)
		return;

	for(i = 0; name[i] != 0; i++);
	buf = malloc((sizeof *buf) * i);
	for(i = 0; name[i] != 0; i++)
		buf[i] = name[i];
	buf[i] = 0;

	if(node->buffer[buffer_id].lines[node->buffer[buffer_id].allocated - 1] != NULL)
	{
		node->buffer[buffer_id].lines = realloc(node->buffer[buffer_id].lines, (sizeof node->buffer[buffer_id].lines) * (node->buffer[buffer_id].allocated + 256));
		for(i = node->buffer[buffer_id].allocated; i < node->buffer[buffer_id].allocated + 256; i++)
			node->buffer[buffer_id].lines[i] = NULL;
		node->buffer[buffer_id].allocated += 256;
	}

	for(i = node->buffer[buffer_id].allocated - 1; node->buffer[buffer_id].lines[i] == NULL; i--);
	for(; i != line ; i--)
		node->buffer[buffer_id].lines[i] = node->buffer[buffer_id].lines[i - 1];
	node->buffer[buffer_id].lines[i] = buf;

	count =	vs_get_subscript_count(node->buffer[buffer_id].subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->buffer[buffer_id].subscribers, i);
		verse_send_t_insert_line(node_id, buffer_id, line, 0, name);
	}
	vs_reset_subscript_session();
}

static void callback_send_t_delete_line(void *user, VNodeID node_id, VNMBufferID buffer_id, uint32 line)
{
	VSNodeText *node;
	unsigned int i, count;
	node = (VSNodeText *)vs_get_node(node_id, V_NT_TEXT);
	if(node == NULL)
		return;

	if(buffer_id >= node->buffer_count || node->buffer[buffer_id].name[0] == 0)
		return;
	if(line >= node->buffer[buffer_id].allocated || node->buffer[buffer_id].lines[line] == NULL)
		return;

	free(node->buffer[buffer_id].lines[line]);
	for(i = line + 1; i < node->buffer[buffer_id].allocated && node->buffer[buffer_id].lines[i - 1] != NULL; i++)
		node->buffer[buffer_id].lines[i - 1] = node->buffer[buffer_id].lines[i];

	count =	vs_get_subscript_count(node->buffer[buffer_id].subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->buffer[buffer_id].subscribers, i);
		verse_send_t_delete_line(node_id, buffer_id, line);
	}
	vs_reset_subscript_session();
}

void vs_t_callback_init()
{
	verse_callback_set(verse_send_t_set_language,		callback_send_t_set_language, NULL);
	verse_callback_set(verse_send_t_buffer_create,		callback_send_t_buffer_create, NULL);
	verse_callback_set(verse_send_t_buffer_destroy,		callback_send_t_buffer_destroy, NULL);
	verse_callback_set(verse_send_t_buffer_subscribe,	callback_send_t_buffer_subscribe, NULL);
	verse_callback_set(verse_send_t_buffer_unsubscribe,	callback_send_t_buffer_unsubscribe, NULL);
	verse_callback_set(verse_send_t_insert_line,		callback_send_t_insert_line, NULL);
	verse_callback_set(verse_send_t_delete_line,		callback_send_t_delete_line, NULL);
}

#endif
