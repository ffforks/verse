/*
**
*/

#include <stdlib.h>
#include <stdio.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "v_cmd_buf.h"
#include "v_network_out_que.h"
#include "v_network.h"
#include "v_connection.h"
#include "v_encryption.h"

unsigned int v_unpack_connect(const char *buf, unsigned int buffer_length)
{
	return -1; /* this command is illegal to send */
}

unsigned int v_unpack_connect_accept(const char *buf, unsigned int buffer_length)
{
	return -1; /* this command is illegal to send */
}

extern void v_callback_connect_terminate(const char *bye);

unsigned int v_unpack_connect_terminate(const char *buf, unsigned int buffer_length)
{
	unsigned int buffer_pos = 0;
	char bye[512];
	buffer_pos = vnp_raw_unpack_string(buf, bye, sizeof bye, buffer_length);
	v_callback_connect_terminate(bye);
	return buffer_pos;
}

typedef struct{
	VNodeID		node_id;
	VNMBufferID buffer_id;
	uint32		pos;
	uint32		length;
	uint16		index; 
	char		*text;
	void		*next;
}VTempLine;

typedef struct{
	VTempLine	*text_temp;
	uint16		text_send_id;
	uint16		text_receive_id;
}VOrderedStorage;

VOrderedStorage *v_create_ordered_storage(void)
{
	VOrderedStorage *s;
	s = malloc(sizeof *s);
	s->text_temp = NULL;
	s->text_send_id = 0;
	s->text_receive_id = 0;
	return s;
}

void v_destroy_ordered_storage(VOrderedStorage *s)
{
	VTempLine *line, *next;
	line = s->text_temp;
	while(line != NULL)
	{
		next = line->next;
		if(line->text != NULL)
			free(line->text);
		free(line);
		line = next;
	}
	free(s);
}


void verse_send_t_text_set(VNodeID node_id, VNMBufferID buffer_id, uint32 pos, uint32 length, const char *text)
{
	uint8 *buf;
	VOrderedStorage *s;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 99);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_t_line_insert(node_id = %u buffer_id = %u pos = %u length = %u text = %s );\n", node_id, buffer_id, pos, length, text);
#endif
	s = v_con_get_ordered_storage();
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], buffer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], pos);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], length);	
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], s->text_send_id++);
	if(text == NULL)
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0);
	else
		buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], text, VN_T_MAX_TEXT_CMD_SIZE);
	v_cmd_buf_set_unique_address_size(head, buffer_pos);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

void v_call_line(VTempLine *line)
{
	char null = 0, *t;
	void (* func_t_line_insert)(void *user_data, VNodeID node_id, VNMBufferID buffer_id, uint32 pos, uint16 length, char *text);
	func_t_line_insert = v_fs_get_user_func(99);
	#if defined V_PRINT_RECEIVE_COMMANDS
		printf("receive: verse_send_t_line_insert(node_id = %u buffer_id = %u pos = %u length = %u text = %s ); callback = %p\n", line->node_id, line->buffer_id, line->pos, line->length, line->text, v_fs_get_user_func(99));
	#endif
	if(line->text == NULL)
		t = &null;
	else
		t = line->text;
	if(func_t_line_insert != NULL)
		func_t_line_insert(v_fs_get_user_data(99), line->node_id, line->buffer_id, line->pos, line->length, t);
}

unsigned int v_unpack_t_text_set(const char *buf, size_t buffer_length)
{
	unsigned int i, buffer_pos = 0;
	VOrderedStorage *s;
	VTempLine l, *line, *past = NULL;
	char text[512];

	if(buffer_length < 12)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &l.node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &l.buffer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &l.pos);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &l.length);	
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &l.index);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], text, sizeof text, buffer_length - buffer_pos);
	if(text[0] == 0)
		l.text = NULL;
	else
		l.text = text;
	s = v_con_get_ordered_storage();
	if(s->text_receive_id == l.index)
	{
		v_call_line(&l);
		s->text_receive_id++;
		line = s->text_temp;
		while(line != NULL)
		{
			if(line->index == s->text_receive_id)
			{
				if(past == NULL)
					s->text_temp = line->next;
				else
					past->next = line->next;
				if(line->text != NULL)
				{
					free(line->text);
					line->text = NULL;
				}
				past = NULL;
				line = s->text_temp;
				s->text_receive_id++;
			}else
			{
				past = line;
				line = line->next;
			}
		}
	}
	else
	{
		line = malloc(sizeof *line);
		*line = l;
		line->next = s->text_temp;
		s->text_temp = line;
		for(i = 0; text[i] != 0; i++);
		if(i > 0)
		{   
			line->text = malloc((sizeof *line->text) * ++i);
			for(i = 0; text[i] != 0; i++)
				line->text[i] = text[i];
			line->text[i] = 0;
		}else
			line->text = NULL;
	}
	return buffer_pos;
}

#endif
