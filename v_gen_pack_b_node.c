#include <stdlib.h>
#include <stdio.h>
#include "v_cmd_gen.h"
#if !defined(V_GENERATE_FUNC_MODE)
#include "verse.h"
#include "v_cmd_buf.h"
#include "v_network_que.h"
#include "v_network.h"
#include "v_connection.h"

void verse_send_b_init_dimensions(VNodeID node_id, uint16 width, uint16 height, uint16 depth)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 80);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_b_init_dimensions(node_id = %u width = %u height = %u depth = %u );\n", node_id, width, height, depth);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], width);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], height);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], depth);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_b_init_dimensions(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_b_init_dimensions)(void *user_data, VNodeID node_id, uint16 width, uint16 height, uint16 depth);
	VNodeID node_id;
	uint16 width;
	uint16 height;
	uint16 depth;
	
	func_b_init_dimensions = user_func;
	if(buffer_length < 10)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &width);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &height);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &depth);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_b_init_dimensions(node_id = %u width = %u height = %u depth = %u ); callback = %p\n", node_id, width, height, depth, user_func);
	#endif
	if(func_b_init_dimensions != NULL)
		func_b_init_dimensions(user_data, node_id, width, height, depth);

	return buffer_pos;
}
void verse_send_b_layer_create(VNodeID node_id, VLayerID layer_id, const char *name, VNBLayerType type)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 81);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_b_layer_create(node_id = %u layer_id = %u name = %s type = %u );\n", node_id, layer_id, name, type);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_b_layer_destroy(VNodeID node_id, VLayerID layer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 81);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_b_layer_destroy(node_id = %u layer_id = %u );\n", node_id, layer_id);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)-1);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_b_layer_create(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_b_layer_create)(void *user_data, VNodeID node_id, VLayerID layer_id, const char *name, VNBLayerType type);
	VNodeID node_id;
	VLayerID layer_id;
	char name[16];
	uint8 type;
	
	func_b_layer_create = user_func;
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
	if(buffer_length < 1 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &type);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_b_layer_create(node_id = %u layer_id = %u name = %s type = %u ); callback = %p\n", node_id, layer_id, name, type, user_func);
	#endif
	if(name[0] == 0)
	{
		void (* alias_b_layer_destroy)(void *user_data, VNodeID node_id, VLayerID layer_id);
		alias_b_layer_destroy = v_fs_get_alias_user_func(81);
		if(alias_b_layer_destroy != NULL)
			alias_b_layer_destroy(v_fs_get_alias_user_data(81), node_id, layer_id);
		return buffer_pos;
	}
	if(func_b_layer_create != NULL)
		func_b_layer_create(user_data, node_id, layer_id, name, (VNBLayerType)type);

	return buffer_pos;
}
void verse_send_b_layer_subscribe(VNodeID node_id, VLayerID layer_id, uint8 level)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 82);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_b_layer_subscribe(node_id = %u layer_id = %u level = %u );\n", node_id, layer_id, level);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], level);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_b_layer_unsubscribe(VNodeID node_id, VLayerID layer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 82);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_b_layer_unsubscribe(node_id = %u layer_id = %u );\n", node_id, layer_id);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], -1);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_b_layer_subscribe(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_b_layer_subscribe)(void *user_data, VNodeID node_id, VLayerID layer_id, uint8 level);
	VNodeID node_id;
	VLayerID layer_id;
	uint8 level;
	
	func_b_layer_subscribe = user_func;
	if(buffer_length < 7)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &level);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_b_layer_subscribe(node_id = %u layer_id = %u level = %u ); callback = %p\n", node_id, layer_id, level, user_func);
	#endif
	if(level == 255)
	{
		void (* alias_b_layer_unsubscribe)(void *user_data, VNodeID node_id, VLayerID layer_id);
		alias_b_layer_unsubscribe = v_fs_get_alias_user_func(82);
		if(alias_b_layer_unsubscribe != NULL)
			alias_b_layer_unsubscribe(v_fs_get_alias_user_data(82), node_id, layer_id);
		return buffer_pos;
	}
	if(func_b_layer_subscribe != NULL)
		func_b_layer_subscribe(user_data, node_id, layer_id, level);

	return buffer_pos;
}
void verse_send_b_layer_set_tile(VNodeID node_id, VLayerID layer_id, uint16 tile_x, uint16 tile_y, uint16 z, VNBLayerType type, void *data)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 83);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_b_layer_set_tile(node_id = %u layer_id = %u tile_x = %u tile_y = %u z = %u type = %u data = %p );\n", node_id, layer_id, tile_x, tile_y, z, type, data);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], tile_x);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], tile_y);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], z);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	{
		unsigned int i;
		switch(type)
		{
			case VN_B_LAYER_INTEGER1 :
				for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE / 8; i++)
					buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8 *)data)[i]);
			break;
			case VN_B_LAYER_INTEGER8 :
				for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
					buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8 *)data)[i]);
			break;
			case VN_B_LAYER_INTEGER16 :
				for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
					buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], ((uint16 *)data)[i]);
			break;
			case VN_B_LAYER_REAL32 :
				for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
					buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], ((float *)data)[i]);
			break;
			case VN_B_LAYER_REAL64 :
				for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
					buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], ((double *)data)[i]);
			break;
		}
	}
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_b_layer_set_tile(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_b_layer_set_tile)(void *user_data, VNodeID node_id, VLayerID layer_id, uint16 tile_x, uint16 tile_y, uint16 z, VNBLayerType type, void *data);
	VNodeID node_id;
	VLayerID layer_id;
	uint16 tile_x;
	uint16 tile_y;
	uint16 z;
	uint8 type;
	void *data;
	
	func_b_layer_set_tile = user_func;
	if(buffer_length < 13)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &tile_x);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &tile_y);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &z);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &type);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_b_layer_set_tile(node_id = %u layer_id = %u tile_x = %u tile_y = %u z = %u type = %u ); callback = %p\n", node_id, layer_id, tile_x, tile_y, z, type, user_func);
	#endif
	switch(type)
	{
		case VN_B_LAYER_INTEGER1 :
		{
			unsigned int i;
			uint8 tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE / 8];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE / 8; i++)
				buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &tile[i]);
			if(func_b_layer_set_tile != NULL)
				func_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);
			return buffer_pos;
		}
		case VN_B_LAYER_INTEGER8 :
		{
			unsigned int i;
			uint8 tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
				buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &tile[i]);
			if(func_b_layer_set_tile != NULL)
				func_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);
			return buffer_pos;
		}
		break;
		case VN_B_LAYER_INTEGER16 :
		{
			unsigned int i;
			uint16 tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
				buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &tile[i]);
			if(func_b_layer_set_tile != NULL)
				func_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);
			return buffer_pos;
		}
		break;
		case VN_B_LAYER_REAL32 :
		{
			unsigned int i;
			float tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
				buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &tile[i]);
			if(func_b_layer_set_tile != NULL)
				func_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);
			return buffer_pos;
		}
		break;
		case VN_B_LAYER_REAL64 :
		{
			unsigned int i;
			double tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE];
			for(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)
				buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &tile[i]);
			if(func_b_layer_set_tile != NULL)
				func_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);
			return buffer_pos;
		}
		break;
		func_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, data);
		return buffer_pos;
	}

	if(func_b_layer_set_tile != NULL)
		func_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, (VNBLayerType)type, data);

	return buffer_pos;
}
#endif

