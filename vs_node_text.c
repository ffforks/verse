/*
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "vs_server.h"

#define VS_TEXT_CHUNK_SIZE 4096

typedef struct {
	char				name[16];
	char				*text;
	unsigned int		length;
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
	unsigned int i;
	destroy_node_head(&node->head);
	free(node);
	for(i = 0; i < node->buffer_count; i++)
	{
		if(node->buffer[i].name[0] != 0)
		{
			free(node->buffer[i].text);
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
			node->buffer = realloc(node->buffer, (sizeof *node->buffer) * node->buffer_count);
			for(i = node->buffer_count; i < node->buffer_count + 16; i++)
				node->buffer[i].name[0] = 0;
			node->buffer_count = i; 
		}
	}	

	if(node->buffer[buffer_id].name[0] == 0)
	{
		node->buffer[buffer_id].allocated = VS_TEXT_CHUNK_SIZE;
		node->buffer[buffer_id].text = malloc((sizeof *node->buffer[buffer_id].text) * node->buffer[buffer_id].allocated);
		node->buffer[buffer_id].length = 0;
		node->buffer[buffer_id].subscribers = vs_create_subscription_list();
	}
	for(i = 0; i < 15 && name[i] != 0; i++)
		node->buffer[buffer_id].name[i] = name[i];
	node->buffer[buffer_id].name[i] = 0;

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
	free(node->buffer[buffer_id].text);
	vs_destroy_subscription_list(node->buffer[buffer_id].subscribers);

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
	for(i = 0; i < node->buffer[buffer_id].length; i += VN_T_MAX_TEXT_CMD_SIZE)
	{	
		if(i + VN_T_MAX_TEXT_CMD_SIZE < node->buffer[buffer_id].length)
			verse_send_t_text_set(node_id, buffer_id, i, node->buffer[buffer_id].length - i, &node->buffer[buffer_id].text[i]);
		else
			verse_send_t_text_set(node_id, buffer_id, i, VN_T_MAX_TEXT_CMD_SIZE, &node->buffer[buffer_id].text[i]);
	}
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

static void callback_send_t_text_set(void *user, VNodeID node_id, VNMBufferID buffer_id, uint32 pos, uint32 length, const char *text)
{
	VSNodeText *node;
	unsigned int i, count, text_length;
	char *buf;
	node = (VSNodeText *)vs_get_node(node_id, V_NT_TEXT);
	if(node == NULL)
		return;
	if(buffer_id >= node->buffer_count || node->buffer[buffer_id].name[0] == 0)
		return;

	for(text_length = 0; text[text_length] != 0; text_length++);


	if(pos > node->buffer[buffer_id].length)
		pos = node->buffer[buffer_id].length;

	if(pos + length > node->buffer[buffer_id].length)
		length = node->buffer[buffer_id].length - pos;

	buf = node->buffer[buffer_id].text;

	if(node->buffer[buffer_id].length + text_length - length > node->buffer[buffer_id].allocated)
	{
		buf = realloc(buf, node->buffer[buffer_id].length + text_length - length + VS_TEXT_CHUNK_SIZE);
		node->buffer[buffer_id].allocated = node->buffer[buffer_id].length + text_length - length + VS_TEXT_CHUNK_SIZE;
	}


	if(text_length < length)
	{
		for(i = 0; text[i] != 0; i++)
			buf[pos + text_length + i] = buf[pos + length + i];

		for(i = 0; text[i] != 0; i++)
			buf[pos + i] = text[i];
	}else
	{
		for(i = node->buffer[buffer_id].length; i != pos + text_length; i--)
			buf[i + text_length - length] = buf[i];
		buf[i + text_length - length] = buf[i];

		for(i = 0; text[i] != 0; i++)
			buf[pos + i] = text[i];
	}

	node->buffer[buffer_id].length += text_length - length;

	if(node->buffer[buffer_id].allocated > VS_TEXT_CHUNK_SIZE * 8 && node->buffer[buffer_id].allocated * 2 > node->buffer[buffer_id].length)
	{
		buf = realloc(buf, node->buffer[buffer_id].length + VS_TEXT_CHUNK_SIZE);
		node->buffer[buffer_id].allocated = node->buffer[buffer_id].length + VS_TEXT_CHUNK_SIZE;
	}

	node->buffer[buffer_id].length += text_length - length;
	node->buffer[buffer_id].text = buf;

	count =	vs_get_subscript_count(node->buffer[buffer_id].subscribers);
	for(i = 0; i < count; i++)
	{
		vs_set_subscript_session(node->buffer[buffer_id].subscribers, i);
		verse_send_t_text_set(node_id, buffer_id, pos, length, text);
	}
	vs_reset_subscript_session();
}


void vs_t_callback_init(void)
{
	verse_callback_set(verse_send_t_set_language,		callback_send_t_set_language, NULL);
	verse_callback_set(verse_send_t_buffer_create,		callback_send_t_buffer_create, NULL);
	verse_callback_set(verse_send_t_buffer_destroy,		callback_send_t_buffer_destroy, NULL);
	verse_callback_set(verse_send_t_buffer_subscribe,	callback_send_t_buffer_subscribe, NULL);
	verse_callback_set(verse_send_t_buffer_unsubscribe,	callback_send_t_buffer_unsubscribe, NULL);
	verse_callback_set(verse_send_t_text_set,			callback_send_t_text_set, NULL);
}

#endif
