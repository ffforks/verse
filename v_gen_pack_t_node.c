#include <stdlib.h>
#include <stdio.h>
#include "v_cmd_gen.h"
#if !defined(V_GENERATE_FUNC_MODE)
#include "verse.h"
#include "v_cmd_buf.h"
#include "v_network_que.h"
#include "v_network.h"
#include "v_connection.h"

void verse_send_t_set_language(VNodeID node_id, const char *language)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 96);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_t_set_language(node_id = %u language = %s );\n", node_id, language);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], language, 512);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_t_set_language(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_t_set_language)(void *user_data, VNodeID node_id, const char *language);
	VNodeID node_id;
	char language[512];
	
	func_t_set_language = user_func;
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], language, 512, buffer_length - buffer_pos);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_t_set_language(node_id = %u language = %s ); callback = %p\n", node_id, language, user_func);
	#endif
	if(func_t_set_language != NULL)
		func_t_set_language(user_data, node_id, language);

	return buffer_pos;
}
void verse_send_t_buffer_create(VNodeID node_id, VNMBufferID buffer_id, uint16 index, const char *name)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 97);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_t_buffer_create(node_id = %u buffer_id = %u index = %u name = %s );\n", node_id, buffer_id, index, name);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], buffer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], index);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_t_buffer_destroy(VNodeID node_id, VNMBufferID buffer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 97);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_t_buffer_destroy(node_id = %u buffer_id = %u );\n", node_id, buffer_id);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], buffer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], -1);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_t_buffer_create(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_t_buffer_create)(void *user_data, VNodeID node_id, VNMBufferID buffer_id, uint16 index, const char *name);
	VNodeID node_id;
	VNMBufferID buffer_id;
	uint16 index;
	char name[16];
	
	func_t_buffer_create = user_func;
	if(buffer_length < 8)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &buffer_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &index);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_t_buffer_create(node_id = %u buffer_id = %u index = %u name = %s ); callback = %p\n", node_id, buffer_id, index, name, user_func);
	#endif
	if(name[0] == 0)
	{
		void (* alias_t_buffer_destroy)(void *user_data, VNodeID node_id, VNMBufferID buffer_id);
		alias_t_buffer_destroy = v_fs_get_alias_user_func(97);
		if(alias_t_buffer_destroy != NULL)
			alias_t_buffer_destroy(v_fs_get_alias_user_data(97), node_id, buffer_id);
		return buffer_pos;
	}
	if(func_t_buffer_create != NULL)
		func_t_buffer_create(user_data, node_id, buffer_id, index, name);

	return buffer_pos;
}
void verse_send_t_buffer_subscribe(VNodeID node_id, VNMBufferID buffer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 98);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_t_buffer_subscribe(node_id = %u buffer_id = %u );\n", node_id, buffer_id);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], buffer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], TRUE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_t_buffer_unsubscribe(VNodeID node_id, VNMBufferID buffer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 98);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_t_buffer_unsubscribe(node_id = %u buffer_id = %u );\n", node_id, buffer_id);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], buffer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], FALSE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_t_buffer_subscribe(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_t_buffer_subscribe)(void *user_data, VNodeID node_id, VNMBufferID buffer_id);
	VNodeID node_id;
	VNMBufferID buffer_id;
	char alias_bool;

	func_t_buffer_subscribe = user_func;
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &buffer_id);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_t_buffer_subscribe(node_id = %u buffer_id = %u ); callback = %p\n", node_id, buffer_id, user_func);
	#endif
	if(buffer_length < buffer_pos + 1)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &alias_bool);
	if(!alias_bool)
	{
		void (* alias_t_buffer_unsubscribe)(void *user_data, VNodeID node_id, VNMBufferID buffer_id);
		alias_t_buffer_unsubscribe = v_fs_get_alias_user_func(98);
		if(alias_t_buffer_unsubscribe != NULL)
			alias_t_buffer_unsubscribe(v_fs_get_alias_user_data(98), node_id, buffer_id);
		return buffer_pos;
	}
	if(func_t_buffer_subscribe != NULL)
		func_t_buffer_subscribe(user_data, node_id, buffer_id);

	return buffer_pos;
}
void verse_send_t_insert_line(VNodeID node_id, VNMBufferID buffer_id, uint32 line, uint16 index, const char *text)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 99);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_t_insert_line(node_id = %u buffer_id = %u line = %u index = %u text = %s );\n", node_id, buffer_id, line, index, text);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], buffer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], line);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], index);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], text, 512);
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_t_delete_line(VNodeID node_id, VNMBufferID buffer_id, uint32 line)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 99);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_t_delete_line(node_id = %u buffer_id = %u line = %u );\n", node_id, buffer_id, line);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], buffer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], line);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], -1);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 512);
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_t_insert_line(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_t_insert_line)(void *user_data, VNodeID node_id, VNMBufferID buffer_id, uint32 line, uint16 index, const char *text);
	VNodeID node_id;
	VNMBufferID buffer_id;
	uint32 line;
	uint16 index;
	char text[512];
	
	func_t_insert_line = user_func;
	if(buffer_length < 12)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &buffer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &line);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &index);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], text, 512, buffer_length - buffer_pos);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_t_insert_line(node_id = %u buffer_id = %u line = %u index = %u text = %s ); callback = %p\n", node_id, buffer_id, line, index, text, user_func);
	#endif
	if(text[0] == 0)
	{
		void (* alias_t_delete_line)(void *user_data, VNodeID node_id, VNMBufferID buffer_id, uint32 line);
		alias_t_delete_line = v_fs_get_alias_user_func(99);
		if(alias_t_delete_line != NULL)
			alias_t_delete_line(v_fs_get_alias_user_data(99), node_id, buffer_id, line);
		return buffer_pos;
	}
	if(func_t_insert_line != NULL)
		func_t_insert_line(user_data, node_id, buffer_id, line, index, text);

	return buffer_pos;
}
#endif

