/*
** This is automatically generated source code -- do not edit.
** Changes are affected either by editing the corresponding protocol
** definition file (v_cmd_def_X.c where X=node type), or by editing
** the code generator itself, in v_cmd_gen.c.
*/

#include <stdlib.h>
#include <stdio.h>

#include "v_cmd_gen.h"
#if !defined(V_GENERATE_FUNC_MODE)
#include "verse.h"
#include "v_cmd_buf.h"
#include "v_network_que.h"
#include "v_network.h"
#include "v_connection.h"

void verse_send_c_curve_create(VNodeID node_id, VLayerID curve_id, const char *name)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 128);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: c_curve_create(node_id = %u curve_id = %u name = %s );\n", node_id, curve_id, name);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], curve_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_c_curve_destroy(VNodeID node_id, VLayerID curve_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 128);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: c_curve_destroy(node_id = %u curve_id = %u );\n", node_id, curve_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], curve_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_c_curve_create(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_c_curve_create)(void *user_data, VNodeID node_id, VLayerID curve_id, const char *name);
	VNodeID node_id;
	VLayerID curve_id;
	char name[16];
	
	func_c_curve_create = user_func;
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &curve_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: c_curve_create(node_id = %u curve_id = %u name = %s ); callback = %p\n", node_id, curve_id, name, user_func);
#endif
	if(name[0] == 0)
	{
		void (* alias_c_curve_destroy)(void *user_data, VNodeID node_id, VLayerID curve_id);
		alias_c_curve_destroy = v_fs_get_alias_user_func(128);
		if(alias_c_curve_destroy != NULL)
			alias_c_curve_destroy(v_fs_get_alias_user_data(128), node_id, curve_id);
		return buffer_pos;
	}
	if(func_c_curve_create != NULL)
		func_c_curve_create(user_data, node_id, curve_id, name);

	return buffer_pos;
}

void verse_send_c_curve_subscribe(VNodeID node_id, VLayerID curve_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 129);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: c_curve_subscribe(node_id = %u curve_id = %u );\n", node_id, curve_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], curve_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], TRUE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_c_curve_unsubscribe(VNodeID node_id, VLayerID curve_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 129);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: c_curve_unsubscribe(node_id = %u curve_id = %u );\n", node_id, curve_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], curve_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], FALSE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_c_curve_subscribe(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_c_curve_subscribe)(void *user_data, VNodeID node_id, VLayerID curve_id);
	VNodeID node_id;
	VLayerID curve_id;
	char alias_bool;

	func_c_curve_subscribe = user_func;
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &curve_id);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: c_curve_subscribe(node_id = %u curve_id = %u ); callback = %p\n", node_id, curve_id, user_func);
#endif
	if(buffer_length < buffer_pos + 1)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &alias_bool);
	if(!alias_bool)
	{
		void (* alias_c_curve_unsubscribe)(void *user_data, VNodeID node_id, VLayerID curve_id);
		alias_c_curve_unsubscribe = v_fs_get_alias_user_func(129);
		if(alias_c_curve_unsubscribe != NULL)
			alias_c_curve_unsubscribe(v_fs_get_alias_user_data(129), node_id, curve_id);
		return buffer_pos;
	}
	if(func_c_curve_subscribe != NULL)
		func_c_curve_subscribe(user_data, node_id, curve_id);

	return buffer_pos;
}

void verse_send_c_curve_key_set(VNodeID node_id, VLayerID curve_id, uint32 key_id, real64 pre_value, uint32 pre_pos, real64 value, real64 pos, real64 post_value, uint32 post_pos)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_100);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 130);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: c_curve_key_set(node_id = %u curve_id = %u key_id = %u pre_value = %f pre_pos = %u value = %f pos = %f post_value = %f post_pos = %u );\n", node_id, curve_id, key_id, pre_value, pre_pos, value, pos, post_value, post_pos);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], curve_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], key_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], pre_value);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], pre_pos);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], value);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], pos);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], post_value);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], post_pos);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_c_curve_key_destroy(VNodeID node_id, VLayerID curve_id, uint32 key_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_100);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 130);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: c_curve_key_destroy(node_id = %u curve_id = %u key_id = %u );\n", node_id, curve_id, key_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], curve_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], key_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], -1);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], -1);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_c_curve_key_set(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_c_curve_key_set)(void *user_data, VNodeID node_id, VLayerID curve_id, uint32 key_id, real64 pre_value, uint32 pre_pos, real64 value, real64 pos, real64 post_value, uint32 post_pos);
	VNodeID node_id;
	VLayerID curve_id;
	uint32 key_id;
	real64 pre_value;
	uint32 pre_pos;
	real64 value;
	real64 pos;
	real64 post_value;
	uint32 post_pos;
	
	func_c_curve_key_set = user_func;
	if(buffer_length < 50)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &curve_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &key_id);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &pre_value);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &pre_pos);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &value);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &pos);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &post_value);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &post_pos);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: c_curve_key_set(node_id = %u curve_id = %u key_id = %u pre_value = %f pre_pos = %u value = %f pos = %f post_value = %f post_pos = %u ); callback = %p\n", node_id, curve_id, key_id, pre_value, pre_pos, value, pos, post_value, post_pos, user_func);
#endif
	if(pre_value == 0)
	{
		void (* alias_c_curve_key_destroy)(void *user_data, VNodeID node_id, VLayerID curve_id, uint32 key_id);
		alias_c_curve_key_destroy = v_fs_get_alias_user_func(130);
		if(alias_c_curve_key_destroy != NULL)
			alias_c_curve_key_destroy(v_fs_get_alias_user_data(130), node_id, curve_id, key_id);
		return buffer_pos;
	}
	if(func_c_curve_key_set != NULL)
		func_c_curve_key_set(user_data, node_id, curve_id, key_id, pre_value, pre_pos, value, pos, post_value, post_pos);

	return buffer_pos;
}

#endif

