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

void verse_send_g_layer_create(VNodeID node_id, VLayerID layer_id, const char *name, VNGLayerType type, uint32 def_uint, real64 def_real)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 48);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_layer_create(node_id = %u layer_id = %u name = %s type = %u def_uint = %u def_real = %f );\n", node_id, layer_id, name, type, def_uint, def_real);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], def_uint);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], def_real);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_g_layer_destroy(VNodeID node_id, VLayerID layer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 48);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_layer_destroy(node_id = %u layer_id = %u );\n", node_id, layer_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)-1);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], -1);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_layer_create(const char *buf, size_t buffer_length)
{
	char enum_temp;
	unsigned int buffer_pos = 0;
	void (* func_g_layer_create)(void *user_data, VNodeID node_id, VLayerID layer_id, const char *name, VNGLayerType type, uint32 def_uint, real64 def_real);
	VNodeID node_id;
	VLayerID layer_id;
	char name[16];
	VNGLayerType type;
	uint32 def_uint;
	real64 def_real;
	
	func_g_layer_create = v_fs_get_user_func(48);
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
	if(buffer_length < 13 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &enum_temp);
	type = (VNGLayerType)enum_temp;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &def_uint);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &def_real);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(name[0] == 0)
		printf("receive: verse_send_g_layer_destroy(node_id = %u layer_id = %u ); callback = %p\n", node_id, layer_id, v_fs_get_alias_user_func(48));
	else
		printf("receive: verse_send_g_layer_create(node_id = %u layer_id = %u name = %s type = %u def_uint = %u def_real = %f ); callback = %p\n", node_id, layer_id, name, type, def_uint, def_real, v_fs_get_user_func(48));
#endif
	if(name[0] == 0)
	{
		void (* alias_g_layer_destroy)(void *user_data, VNodeID node_id, VLayerID layer_id);
		alias_g_layer_destroy = v_fs_get_alias_user_func(48);
		if(alias_g_layer_destroy != NULL)
			alias_g_layer_destroy(v_fs_get_alias_user_data(48), node_id, layer_id);
		return buffer_pos;
	}
	if(func_g_layer_create != NULL)
		func_g_layer_create(v_fs_get_user_data(48), node_id, layer_id, name, (VNGLayerType)type, def_uint, def_real);

	return buffer_pos;
}

void verse_send_g_layer_subscribe(VNodeID node_id, VLayerID layer_id, VNRealFormat type)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 49);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_layer_subscribe(node_id = %u layer_id = %u type = %u );\n", node_id, layer_id, type);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_g_layer_unsubscribe(VNodeID node_id, VLayerID layer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 49);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_layer_unsubscribe(node_id = %u layer_id = %u );\n", node_id, layer_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)-1);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_layer_subscribe(const char *buf, size_t buffer_length)
{
	char enum_temp;
	unsigned int buffer_pos = 0;
	void (* func_g_layer_subscribe)(void *user_data, VNodeID node_id, VLayerID layer_id, VNRealFormat type);
	VNodeID node_id;
	VLayerID layer_id;
	VNRealFormat type;
	
	func_g_layer_subscribe = v_fs_get_user_func(49);
	if(buffer_length < 7)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &enum_temp);
	type = (VNRealFormat)enum_temp;
#if defined V_PRINT_RECEIVE_COMMANDS
	if(type > VN_FORMAT_REAL64)
		printf("receive: verse_send_g_layer_unsubscribe(node_id = %u layer_id = %u ); callback = %p\n", node_id, layer_id, v_fs_get_alias_user_func(49));
	else
		printf("receive: verse_send_g_layer_subscribe(node_id = %u layer_id = %u type = %u ); callback = %p\n", node_id, layer_id, type, v_fs_get_user_func(49));
#endif
	if(type > VN_FORMAT_REAL64)
	{
		void (* alias_g_layer_unsubscribe)(void *user_data, VNodeID node_id, VLayerID layer_id);
		alias_g_layer_unsubscribe = v_fs_get_alias_user_func(49);
		if(alias_g_layer_unsubscribe != NULL)
			alias_g_layer_unsubscribe(v_fs_get_alias_user_data(49), node_id, layer_id);
		return buffer_pos;
	}
	if(func_g_layer_subscribe != NULL)
		func_g_layer_subscribe(v_fs_get_user_data(49), node_id, layer_id, (VNRealFormat)type);

	return buffer_pos;
}

void verse_send_g_vertex_set_real32_xyz(VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real32 x, real32 y, real32 z)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 50);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_vertex_set_real32_xyz(node_id = %u layer_id = %u vertex_id = %u x = %f y = %f z = %f );\n", node_id, layer_id, vertex_id, x, y, z);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], vertex_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], x);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], y);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], z);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_g_vertex_delete_real32(VNodeID node_id, uint32 vertex_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 50);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_vertex_delete_real32(node_id = %u vertex_id = %u );\n", node_id, vertex_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], 0);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], vertex_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL32_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL32_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL32_MAX);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_vertex_set_real32_xyz(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_vertex_set_real32_xyz)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real32 x, real32 y, real32 z);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 vertex_id;
	real32 x;
	real32 y;
	real32 z;
	
	func_g_vertex_set_real32_xyz = v_fs_get_user_func(50);
	if(buffer_length < 22)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &vertex_id);
	buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &x);
	buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &y);
	buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &z);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(x == V_REAL32_MAX || y == V_REAL32_MAX || z == V_REAL32_MAX)
		printf("receive: verse_send_g_vertex_delete_real32(node_id = %u vertex_id = %u ); callback = %p\n", node_id, vertex_id, v_fs_get_alias_user_func(50));
	else
		printf("receive: verse_send_g_vertex_set_real32_xyz(node_id = %u layer_id = %u vertex_id = %u x = %f y = %f z = %f ); callback = %p\n", node_id, layer_id, vertex_id, x, y, z, v_fs_get_user_func(50));
#endif
	if(x == V_REAL32_MAX || y == V_REAL32_MAX || z == V_REAL32_MAX)
	{
		void (* alias_g_vertex_delete_real32)(void *user_data, VNodeID node_id, uint32 vertex_id);
		alias_g_vertex_delete_real32 = v_fs_get_alias_user_func(50);
		if(alias_g_vertex_delete_real32 != NULL)
			alias_g_vertex_delete_real32(v_fs_get_alias_user_data(50), node_id, layer_id);
		return buffer_pos;
	}
	if(func_g_vertex_set_real32_xyz != NULL)
		func_g_vertex_set_real32_xyz(v_fs_get_user_data(50), node_id, layer_id, vertex_id, x, y, z);

	return buffer_pos;
}

void verse_send_g_vertex_set_real64_xyz(VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real64 x, real64 y, real64 z)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 51);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_vertex_set_real64_xyz(node_id = %u layer_id = %u vertex_id = %u x = %f y = %f z = %f );\n", node_id, layer_id, vertex_id, x, y, z);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], vertex_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], x);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], y);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], z);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_g_vertex_delete_real64(VNodeID node_id, uint32 vertex_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 51);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_vertex_delete_real64(node_id = %u vertex_id = %u );\n", node_id, vertex_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], 0);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], vertex_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_vertex_set_real64_xyz(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_vertex_set_real64_xyz)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real64 x, real64 y, real64 z);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 vertex_id;
	real64 x;
	real64 y;
	real64 z;
	
	func_g_vertex_set_real64_xyz = v_fs_get_user_func(51);
	if(buffer_length < 34)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &vertex_id);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &x);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &y);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &z);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(x == V_REAL64_MAX || y == V_REAL64_MAX || z == V_REAL64_MAX)
		printf("receive: verse_send_g_vertex_delete_real64(node_id = %u vertex_id = %u ); callback = %p\n", node_id, vertex_id, v_fs_get_alias_user_func(51));
	else
		printf("receive: verse_send_g_vertex_set_real64_xyz(node_id = %u layer_id = %u vertex_id = %u x = %f y = %f z = %f ); callback = %p\n", node_id, layer_id, vertex_id, x, y, z, v_fs_get_user_func(51));
#endif
	if(x == V_REAL64_MAX || y == V_REAL64_MAX || z == V_REAL64_MAX)
	{
		void (* alias_g_vertex_delete_real64)(void *user_data, VNodeID node_id, uint32 vertex_id);
		alias_g_vertex_delete_real64 = v_fs_get_alias_user_func(51);
		if(alias_g_vertex_delete_real64 != NULL)
			alias_g_vertex_delete_real64(v_fs_get_alias_user_data(51), node_id, layer_id);
		return buffer_pos;
	}
	if(func_g_vertex_set_real64_xyz != NULL)
		func_g_vertex_set_real64_xyz(v_fs_get_user_data(51), node_id, layer_id, vertex_id, x, y, z);

	return buffer_pos;
}

void verse_send_g_vertex_set_uint32(VNodeID node_id, VLayerID layer_id, uint32 vertex_id, uint32 value)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 52);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_vertex_set_uint32(node_id = %u layer_id = %u vertex_id = %u value = %u );\n", node_id, layer_id, vertex_id, value);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], vertex_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], value);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_vertex_set_uint32(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_vertex_set_uint32)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 vertex_id, uint32 value);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 vertex_id;
	uint32 value;
	
	func_g_vertex_set_uint32 = v_fs_get_user_func(52);
	if(buffer_length < 14)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &vertex_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &value);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_vertex_set_uint32(node_id = %u layer_id = %u vertex_id = %u value = %u ); callback = %p\n", node_id, layer_id, vertex_id, value, v_fs_get_user_func(52));
#endif
	if(func_g_vertex_set_uint32 != NULL)
		func_g_vertex_set_uint32(v_fs_get_user_data(52), node_id, layer_id, vertex_id, value);

	return buffer_pos;
}

void verse_send_g_vertex_set_real64(VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real64 value)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 53);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_vertex_set_real64(node_id = %u layer_id = %u vertex_id = %u value = %f );\n", node_id, layer_id, vertex_id, value);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], vertex_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], value);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_vertex_set_real64(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_vertex_set_real64)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real64 value);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 vertex_id;
	real64 value;
	
	func_g_vertex_set_real64 = v_fs_get_user_func(53);
	if(buffer_length < 18)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &vertex_id);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &value);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_vertex_set_real64(node_id = %u layer_id = %u vertex_id = %u value = %f ); callback = %p\n", node_id, layer_id, vertex_id, value, v_fs_get_user_func(53));
#endif
	if(func_g_vertex_set_real64 != NULL)
		func_g_vertex_set_real64(v_fs_get_user_data(53), node_id, layer_id, vertex_id, value);

	return buffer_pos;
}

void verse_send_g_vertex_set_real32(VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real32 value)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 54);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_vertex_set_real32(node_id = %u layer_id = %u vertex_id = %u value = %f );\n", node_id, layer_id, vertex_id, value);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], vertex_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], value);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_vertex_set_real32(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_vertex_set_real32)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real32 value);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 vertex_id;
	real32 value;
	
	func_g_vertex_set_real32 = v_fs_get_user_func(54);
	if(buffer_length < 14)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &vertex_id);
	buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &value);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_vertex_set_real32(node_id = %u layer_id = %u vertex_id = %u value = %f ); callback = %p\n", node_id, layer_id, vertex_id, value, v_fs_get_user_func(54));
#endif
	if(func_g_vertex_set_real32 != NULL)
		func_g_vertex_set_real32(v_fs_get_user_data(54), node_id, layer_id, vertex_id, value);

	return buffer_pos;
}

void verse_send_g_polygon_set_corner_uint32(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint32 v0, uint32 v1, uint32 v2, uint32 v3)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 55);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_polygon_set_corner_uint32(node_id = %u layer_id = %u polygon_id = %u v0 = %u v1 = %u v2 = %u v3 = %u );\n", node_id, layer_id, polygon_id, v0, v1, v2, v3);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], polygon_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], v0);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], v1);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], v2);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], v3);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_g_polygon_delete(VNodeID node_id, uint32 polygon_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 55);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_polygon_delete(node_id = %u polygon_id = %u );\n", node_id, polygon_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], 0);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], polygon_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], -1);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], -1);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], -1);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], -1);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_polygon_set_corner_uint32(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_polygon_set_corner_uint32)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint32 v0, uint32 v1, uint32 v2, uint32 v3);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 polygon_id;
	uint32 v0;
	uint32 v1;
	uint32 v2;
	uint32 v3;
	
	func_g_polygon_set_corner_uint32 = v_fs_get_user_func(55);
	if(buffer_length < 26)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &polygon_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &v0);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &v1);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &v2);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &v3);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(v0 == -1 || v1 == -1 || v2 == -1)
		printf("receive: verse_send_g_polygon_delete(node_id = %u polygon_id = %u ); callback = %p\n", node_id, polygon_id, v_fs_get_alias_user_func(55));
	else
		printf("receive: verse_send_g_polygon_set_corner_uint32(node_id = %u layer_id = %u polygon_id = %u v0 = %u v1 = %u v2 = %u v3 = %u ); callback = %p\n", node_id, layer_id, polygon_id, v0, v1, v2, v3, v_fs_get_user_func(55));
#endif
	if(v0 == -1 || v1 == -1 || v2 == -1)
	{
		void (* alias_g_polygon_delete)(void *user_data, VNodeID node_id, uint32 polygon_id);
		alias_g_polygon_delete = v_fs_get_alias_user_func(55);
		if(alias_g_polygon_delete != NULL)
			alias_g_polygon_delete(v_fs_get_alias_user_data(55), node_id, layer_id);
		return buffer_pos;
	}
	if(func_g_polygon_set_corner_uint32 != NULL)
		func_g_polygon_set_corner_uint32(v_fs_get_user_data(55), node_id, layer_id, polygon_id, v0, v1, v2, v3);

	return buffer_pos;
}

void verse_send_g_polygon_set_corner_real64(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real64 v0, real64 v1, real64 v2, real64 v3)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 56);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_polygon_set_corner_real64(node_id = %u layer_id = %u polygon_id = %u v0 = %f v1 = %f v2 = %f v3 = %f );\n", node_id, layer_id, polygon_id, v0, v1, v2, v3);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], polygon_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], v0);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], v1);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], v2);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], v3);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_polygon_set_corner_real64(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_polygon_set_corner_real64)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real64 v0, real64 v1, real64 v2, real64 v3);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 polygon_id;
	real64 v0;
	real64 v1;
	real64 v2;
	real64 v3;
	
	func_g_polygon_set_corner_real64 = v_fs_get_user_func(56);
	if(buffer_length < 42)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &polygon_id);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &v0);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &v1);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &v2);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &v3);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_polygon_set_corner_real64(node_id = %u layer_id = %u polygon_id = %u v0 = %f v1 = %f v2 = %f v3 = %f ); callback = %p\n", node_id, layer_id, polygon_id, v0, v1, v2, v3, v_fs_get_user_func(56));
#endif
	if(func_g_polygon_set_corner_real64 != NULL)
		func_g_polygon_set_corner_real64(v_fs_get_user_data(56), node_id, layer_id, polygon_id, v0, v1, v2, v3);

	return buffer_pos;
}

void verse_send_g_polygon_set_corner_real32(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real32 v0, real32 v1, real32 v2, real32 v3)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 57);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_polygon_set_corner_real32(node_id = %u layer_id = %u polygon_id = %u v0 = %f v1 = %f v2 = %f v3 = %f );\n", node_id, layer_id, polygon_id, v0, v1, v2, v3);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], polygon_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], v0);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], v1);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], v2);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], v3);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_polygon_set_corner_real32(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_polygon_set_corner_real32)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real32 v0, real32 v1, real32 v2, real32 v3);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 polygon_id;
	real32 v0;
	real32 v1;
	real32 v2;
	real32 v3;
	
	func_g_polygon_set_corner_real32 = v_fs_get_user_func(57);
	if(buffer_length < 26)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &polygon_id);
	buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &v0);
	buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &v1);
	buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &v2);
	buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &v3);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_polygon_set_corner_real32(node_id = %u layer_id = %u polygon_id = %u v0 = %f v1 = %f v2 = %f v3 = %f ); callback = %p\n", node_id, layer_id, polygon_id, v0, v1, v2, v3, v_fs_get_user_func(57));
#endif
	if(func_g_polygon_set_corner_real32 != NULL)
		func_g_polygon_set_corner_real32(v_fs_get_user_data(57), node_id, layer_id, polygon_id, v0, v1, v2, v3);

	return buffer_pos;
}

void verse_send_g_polygon_set_face_uint8(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint8 value)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 58);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_polygon_set_face_uint8(node_id = %u layer_id = %u polygon_id = %u value = %u );\n", node_id, layer_id, polygon_id, value);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], polygon_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], value);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_polygon_set_face_uint8(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_polygon_set_face_uint8)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint8 value);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 polygon_id;
	uint8 value;
	
	func_g_polygon_set_face_uint8 = v_fs_get_user_func(58);
	if(buffer_length < 11)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &polygon_id);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &value);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_polygon_set_face_uint8(node_id = %u layer_id = %u polygon_id = %u value = %u ); callback = %p\n", node_id, layer_id, polygon_id, value, v_fs_get_user_func(58));
#endif
	if(func_g_polygon_set_face_uint8 != NULL)
		func_g_polygon_set_face_uint8(v_fs_get_user_data(58), node_id, layer_id, polygon_id, value);

	return buffer_pos;
}

void verse_send_g_polygon_set_face_uint32(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint32 value)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 59);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_polygon_set_face_uint32(node_id = %u layer_id = %u polygon_id = %u value = %u );\n", node_id, layer_id, polygon_id, value);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], polygon_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], value);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_polygon_set_face_uint32(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_polygon_set_face_uint32)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint32 value);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 polygon_id;
	uint32 value;
	
	func_g_polygon_set_face_uint32 = v_fs_get_user_func(59);
	if(buffer_length < 14)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &polygon_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &value);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_polygon_set_face_uint32(node_id = %u layer_id = %u polygon_id = %u value = %u ); callback = %p\n", node_id, layer_id, polygon_id, value, v_fs_get_user_func(59));
#endif
	if(func_g_polygon_set_face_uint32 != NULL)
		func_g_polygon_set_face_uint32(v_fs_get_user_data(59), node_id, layer_id, polygon_id, value);

	return buffer_pos;
}

void verse_send_g_polygon_set_face_real64(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real64 value)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 60);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_polygon_set_face_real64(node_id = %u layer_id = %u polygon_id = %u value = %f );\n", node_id, layer_id, polygon_id, value);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], polygon_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], value);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_polygon_set_face_real64(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_polygon_set_face_real64)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real64 value);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 polygon_id;
	real64 value;
	
	func_g_polygon_set_face_real64 = v_fs_get_user_func(60);
	if(buffer_length < 18)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &polygon_id);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &value);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_polygon_set_face_real64(node_id = %u layer_id = %u polygon_id = %u value = %f ); callback = %p\n", node_id, layer_id, polygon_id, value, v_fs_get_user_func(60));
#endif
	if(func_g_polygon_set_face_real64 != NULL)
		func_g_polygon_set_face_real64(v_fs_get_user_data(60), node_id, layer_id, polygon_id, value);

	return buffer_pos;
}

void verse_send_g_polygon_set_face_real32(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real32 value)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 61);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_polygon_set_face_real32(node_id = %u layer_id = %u polygon_id = %u value = %f );\n", node_id, layer_id, polygon_id, value);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], polygon_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], value);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_polygon_set_face_real32(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_polygon_set_face_real32)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real32 value);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 polygon_id;
	real32 value;
	
	func_g_polygon_set_face_real32 = v_fs_get_user_func(61);
	if(buffer_length < 14)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &polygon_id);
	buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &value);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_polygon_set_face_real32(node_id = %u layer_id = %u polygon_id = %u value = %f ); callback = %p\n", node_id, layer_id, polygon_id, value, v_fs_get_user_func(61));
#endif
	if(func_g_polygon_set_face_real32 != NULL)
		func_g_polygon_set_face_real32(v_fs_get_user_data(61), node_id, layer_id, polygon_id, value);

	return buffer_pos;
}

void verse_send_g_crease_set_vertex(VNodeID node_id, const char *layer, uint32 def_crease)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 62);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_crease_set_vertex(node_id = %u layer = %s def_crease = %u );\n", node_id, layer, def_crease);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], layer, 16);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], def_crease);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_crease_set_vertex(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_crease_set_vertex)(void *user_data, VNodeID node_id, const char *layer, uint32 def_crease);
	VNodeID node_id;
	char layer[16];
	uint32 def_crease;
	
	func_g_crease_set_vertex = v_fs_get_user_func(62);
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], layer, 16, buffer_length - buffer_pos);
	if(buffer_length < 4 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &def_crease);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_crease_set_vertex(node_id = %u layer = %s def_crease = %u ); callback = %p\n", node_id, layer, def_crease, v_fs_get_user_func(62));
#endif
	if(func_g_crease_set_vertex != NULL)
		func_g_crease_set_vertex(v_fs_get_user_data(62), node_id, layer, def_crease);

	return buffer_pos;
}

void verse_send_g_crease_set_edge(VNodeID node_id, const char *layer, uint32 def_crease)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 63);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_crease_set_edge(node_id = %u layer = %s def_crease = %u );\n", node_id, layer, def_crease);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], layer, 16);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], def_crease);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_crease_set_edge(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_crease_set_edge)(void *user_data, VNodeID node_id, const char *layer, uint32 def_crease);
	VNodeID node_id;
	char layer[16];
	uint32 def_crease;
	
	func_g_crease_set_edge = v_fs_get_user_func(63);
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], layer, 16, buffer_length - buffer_pos);
	if(buffer_length < 4 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &def_crease);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_g_crease_set_edge(node_id = %u layer = %s def_crease = %u ); callback = %p\n", node_id, layer, def_crease, v_fs_get_user_func(63));
#endif
	if(func_g_crease_set_edge != NULL)
		func_g_crease_set_edge(v_fs_get_user_data(63), node_id, layer, def_crease);

	return buffer_pos;
}

void verse_send_g_bone_create(VNodeID node_id, uint32 bone_id, const char *name, uint32 parent, real64 pos_x, real64 pos_y, real64 pos_z, real64 rot_x, real64 rot_y, real64 rot_z, real64 rot_w)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_100);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 64);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_bone_create(node_id = %u bone_id = %u name = %s parent = %u pos_x = %f pos_y = %f pos_z = %f rot_x = %f rot_y = %f rot_z = %f rot_w = %f );\n", node_id, bone_id, name, parent, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, rot_w);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], bone_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], parent);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], pos_x);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], pos_y);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], pos_z);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], rot_x);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], rot_y);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], rot_z);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], rot_w);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_g_bone_destroy(VNodeID node_id, uint32 bone_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_100);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 64);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_g_bone_destroy(node_id = %u bone_id = %u );\n", node_id, bone_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], bone_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], -1);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_g_bone_create(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_g_bone_create)(void *user_data, VNodeID node_id, uint32 bone_id, const char *name, uint32 parent, real64 pos_x, real64 pos_y, real64 pos_z, real64 rot_x, real64 rot_y, real64 rot_z, real64 rot_w);
	VNodeID node_id;
	uint32 bone_id;
	char name[16];
	uint32 parent;
	real64 pos_x;
	real64 pos_y;
	real64 pos_z;
	real64 rot_x;
	real64 rot_y;
	real64 rot_z;
	real64 rot_w;
	
	func_g_bone_create = v_fs_get_user_func(64);
	if(buffer_length < 8)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &bone_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
	if(buffer_length < 60 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &parent);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &pos_x);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &pos_y);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &pos_z);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &rot_x);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &rot_y);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &rot_z);
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &rot_w);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(name[0] == 0)
		printf("receive: verse_send_g_bone_destroy(node_id = %u bone_id = %u ); callback = %p\n", node_id, bone_id, v_fs_get_alias_user_func(64));
	else
		printf("receive: verse_send_g_bone_create(node_id = %u bone_id = %u name = %s parent = %u pos_x = %f pos_y = %f pos_z = %f rot_x = %f rot_y = %f rot_z = %f rot_w = %f ); callback = %p\n", node_id, bone_id, name, parent, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, rot_w, v_fs_get_user_func(64));
#endif
	if(name[0] == 0)
	{
		void (* alias_g_bone_destroy)(void *user_data, VNodeID node_id, uint32 bone_id);
		alias_g_bone_destroy = v_fs_get_alias_user_func(64);
		if(alias_g_bone_destroy != NULL)
			alias_g_bone_destroy(v_fs_get_alias_user_data(64), node_id, bone_id);
		return buffer_pos;
	}
	if(func_g_bone_create != NULL)
		func_g_bone_create(v_fs_get_user_data(64), node_id, bone_id, name, parent, pos_x, pos_y, pos_z, rot_x, rot_y, rot_z, rot_w);

	return buffer_pos;
}

#endif

