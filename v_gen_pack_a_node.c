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
#include "v_network_out_que.h"
#include "v_network.h"
#include "v_connection.h"

void verse_send_a_layer_create(VNodeID node_id, VLayerID layer_id, const char *name, VNALayerType type, real64 frequency)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_80);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 160);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_layer_create(node_id = %u layer_id = %u name = %s type = %u frequency = %f );\n", node_id, layer_id, name, type, frequency);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], frequency);
	if(node_id == (uint32)(-1) || layer_id == (uint16)(-1))
		v_cmd_buf_set_unique_address_size(head, 7);
	else
		v_cmd_buf_set_address_size(head, 7);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_a_layer_destroy(VNodeID node_id, VLayerID layer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_80);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 160);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_layer_destroy(node_id = %u layer_id = %u );\n", node_id, layer_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)-1);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], V_REAL64_MAX);
	if(node_id == (uint32)(-1) || layer_id == (uint16)(-1))
		v_cmd_buf_set_unique_address_size(head, 7);
	else
		v_cmd_buf_set_address_size(head, 7);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_a_layer_create(const char *buf, size_t buffer_length)
{
	uint8 enum_temp;
	unsigned int buffer_pos = 0;
	void (* func_a_layer_create)(void *user_data, VNodeID node_id, VLayerID layer_id, const char *name, VNALayerType type, real64 frequency);
	VNodeID node_id;
	VLayerID layer_id;
	char name[16];
	VNALayerType type;
	real64 frequency;
	
	func_a_layer_create = v_fs_get_user_func(160);
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
	if(buffer_length < 9 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &enum_temp);
	type = (VNALayerType)enum_temp;
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &frequency);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(name[0] == 0)
		printf("receive: verse_send_a_layer_destroy(node_id = %u layer_id = %u ); callback = %p\n", node_id, layer_id, v_fs_get_alias_user_func(160));
	else
		printf("receive: verse_send_a_layer_create(node_id = %u layer_id = %u name = %s type = %u frequency = %f ); callback = %p\n", node_id, layer_id, name, type, frequency, v_fs_get_user_func(160));
#endif
	if(name[0] == 0)
	{
		void (* alias_a_layer_destroy)(void *user_data, VNodeID node_id, VLayerID layer_id);
		alias_a_layer_destroy = v_fs_get_alias_user_func(160);
		if(alias_a_layer_destroy != NULL)
			alias_a_layer_destroy(v_fs_get_alias_user_data(160), node_id, layer_id);
		return buffer_pos;
	}
	if(func_a_layer_create != NULL)
		func_a_layer_create(v_fs_get_user_data(160), node_id, layer_id, name, (VNALayerType)type, frequency);

	return buffer_pos;
}

void verse_send_a_layer_subscribe(VNodeID node_id, VLayerID layer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 161);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_layer_subscribe(node_id = %u layer_id = %u );\n", node_id, layer_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], TRUE);
	if(node_id == (uint32)(-1) || layer_id == (uint16)(-1))
		v_cmd_buf_set_unique_address_size(head, 7);
	else
		v_cmd_buf_set_address_size(head, 7);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_a_layer_unsubscribe(VNodeID node_id, VLayerID layer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 161);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_layer_unsubscribe(node_id = %u layer_id = %u );\n", node_id, layer_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], FALSE);
	if(node_id == (uint32)(-1) || layer_id == (uint16)(-1))
		v_cmd_buf_set_unique_address_size(head, 7);
	else
		v_cmd_buf_set_address_size(head, 7);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_a_layer_subscribe(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_a_layer_subscribe)(void *user_data, VNodeID node_id, VLayerID layer_id);
	VNodeID node_id;
	VLayerID layer_id;
	uint8	alias_bool;

	func_a_layer_subscribe = v_fs_get_user_func(161);
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	if(buffer_length < buffer_pos + 1)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &alias_bool);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(!alias_bool)
		printf("receive: verse_send_a_layer_unsubscribe(node_id = %u layer_id = %u ); callback = %p\n", node_id, layer_id, v_fs_get_alias_user_func(161));
	else
		printf("receive: verse_send_a_layer_subscribe(node_id = %u layer_id = %u ); callback = %p\n", node_id, layer_id, v_fs_get_user_func(161));
#endif
	if(!alias_bool)
	{
		void (* alias_a_layer_unsubscribe)(void *user_data, VNodeID node_id, VLayerID layer_id);
		alias_a_layer_unsubscribe = v_fs_get_alias_user_func(161);
		if(alias_a_layer_unsubscribe != NULL)
			alias_a_layer_unsubscribe(v_fs_get_alias_user_data(161), node_id, layer_id);
		return buffer_pos;
	}
	if(func_a_layer_subscribe != NULL)
		func_a_layer_subscribe(v_fs_get_user_data(161), node_id, layer_id);

	return buffer_pos;
}

void verse_send_a_block_set(VNodeID node_id, VLayerID layer_id, uint32 block_index, VNALayerType type, const VNASample *data)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 162);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_block_set(node_id = %u layer_id = %u block_index = %u type = %u data = %p );\n", node_id, layer_id, block_index, type, data);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], block_index);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	{
		unsigned int i;
		switch(type)
		{
			case VN_A_LAYER_INT8 :
				for(i = 0; i < VN_A_BLOCK_SIZE_INT8; i++)
					buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8*)data)[i]);
			break;
			case VN_A_LAYER_INT16 :
				for(i = 0; i < VN_A_BLOCK_SIZE_INT16; i++)
					buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], ((uint16*)data)[i]);
			break;
			case VN_A_LAYER_INT24 :
				for(i = 0; i < VN_A_BLOCK_SIZE_INT24; i++)
					buffer_pos += vnp_raw_pack_uint24(&buf[buffer_pos], ((uint32*)data)[i]);
			break;
			case VN_A_LAYER_INT32 :
				for(i = 0; i < VN_A_BLOCK_SIZE_INT32; i++)
					buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((uint32*)data)[i]);
			break;
			case VN_A_LAYER_REAL32 :
				for(i = 0; i < VN_A_BLOCK_SIZE_REAL32; i++)
					buffer_pos += vnp_raw_pack_real32(&buf[buffer_pos], ((real32*)data)[i]);
			break;
			case VN_A_LAYER_REAL64 :
				for(i = 0; i < VN_A_BLOCK_SIZE_REAL64; i++)
					buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], ((real64*)data)[i]);
			break;
		}
	}
	if(node_id == (uint32)(-1) || layer_id == (uint16)(-1) || block_index == (uint32)(-1))
		v_cmd_buf_set_unique_address_size(head, 11);
	else
		v_cmd_buf_set_address_size(head, 11);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_a_block_clear(VNodeID node_id, VLayerID layer_id, uint32 block_index)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 162);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_block_clear(node_id = %u layer_id = %u block_index = %u );\n", node_id, layer_id, block_index);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], block_index);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)-1);
	if(node_id == (uint32)(-1) || layer_id == (uint16)(-1) || block_index == (uint32)(-1))
		v_cmd_buf_set_unique_address_size(head, 11);
	else
		v_cmd_buf_set_address_size(head, 11);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_a_block_set(const char *buf, size_t buffer_length)
{
	uint8 enum_temp;
	unsigned int buffer_pos = 0;
	void (* func_a_block_set)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 block_index, VNALayerType type, const VNASample *data);
	VNodeID node_id;
	VLayerID layer_id;
	uint32 block_index;
	VNALayerType type;
	const VNASample *data;
	
	func_a_block_set = v_fs_get_user_func(162);
	if(buffer_length < 10)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &block_index);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &enum_temp);
	type = (VNALayerType)enum_temp;
#if defined V_PRINT_RECEIVE_COMMANDS
	if(type > VN_A_LAYER_REAL64)
		printf("receive: verse_send_a_block_clear(node_id = %u layer_id = %u block_index = %u ); callback = %p\n", node_id, layer_id, block_index, v_fs_get_alias_user_func(162));
	else
		printf("receive: verse_send_a_block_set(node_id = %u layer_id = %u block_index = %u type = %u ); callback = %p\n", node_id, layer_id, block_index, type, v_fs_get_user_func(162));
#endif
	{
		unsigned int i;
		switch(type)
		{
			case VN_A_LAYER_INT8 :
			{
				uint8 data[1500];
				for(i = 0; i < VN_A_BLOCK_SIZE_INT8; i++)
					buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &data[i]);
				if(func_a_block_set != NULL)
					func_a_block_set(v_fs_get_user_data(162), node_id, layer_id, block_index, (VNALayerType)type, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_INT16 :
			{
				uint16 data[750];
				for(i = 0; i < VN_A_BLOCK_SIZE_INT16; i++)
					buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &data[i]);
				if(func_a_block_set != NULL)
					func_a_block_set(v_fs_get_user_data(162), node_id, layer_id, block_index, (VNALayerType)type, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_INT24 :
			{
				uint32 data[325];
				for(i = 0; i < VN_A_BLOCK_SIZE_INT24; i++)
					buffer_pos += vnp_raw_unpack_uint24(&buf[buffer_pos], &data[i]);
				if(func_a_block_set != NULL)
					func_a_block_set(v_fs_get_user_data(162), node_id, layer_id, block_index, (VNALayerType)type, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_INT32 :
			{
				uint32 data[325];
				for(i = 0; i < VN_A_BLOCK_SIZE_INT32; i++)
					buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &data[i]);
				if(func_a_block_set != NULL)
					func_a_block_set(v_fs_get_user_data(162), node_id, layer_id, block_index, (VNALayerType)type, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_REAL32 :
			{
				real32 data[1500];
				for(i = 0; i < VN_A_BLOCK_SIZE_REAL32; i++)
					buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &data[i]);
				if(func_a_block_set != NULL)
					func_a_block_set(v_fs_get_user_data(162), node_id, layer_id, block_index, (VNALayerType)type, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_REAL64 :
			{
				real64 data[162];
				for(i = 0; i < VN_A_BLOCK_SIZE_REAL64; i++)
					buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &data[i]);
				if(func_a_block_set != NULL)
					func_a_block_set(v_fs_get_user_data(162), node_id, layer_id, block_index, (VNALayerType)type, (VNASample *) data);
				return buffer_pos;
			}
		}
	}

	if(type > VN_A_LAYER_REAL64)
	{
		void (* alias_a_block_clear)(void *user_data, VNodeID node_id, VLayerID layer_id, uint32 block_index);
		alias_a_block_clear = v_fs_get_alias_user_func(162);
		if(alias_a_block_clear != NULL)
			alias_a_block_clear(v_fs_get_alias_user_data(162), node_id, layer_id, block_index);
		return buffer_pos;
	}
	if(func_a_block_set != NULL)
		func_a_block_set(v_fs_get_user_data(162), node_id, layer_id, block_index, (VNALayerType)type, data);

	return buffer_pos;
}

void verse_send_a_stream_create(VNodeID node_id, VLayerID stream_id, const char *name)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_30);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 163);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_stream_create(node_id = %u stream_id = %u name = %s );\n", node_id, stream_id, name);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], stream_id);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	if(node_id == (uint32)(-1) || stream_id == (uint16)(-1))
		v_cmd_buf_set_unique_address_size(head, 7);
	else
		v_cmd_buf_set_address_size(head, 7);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_a_stream_destroy(VNodeID node_id, VLayerID stream_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_30);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 163);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_stream_destroy(node_id = %u stream_id = %u );\n", node_id, stream_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], stream_id);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	if(node_id == (uint32)(-1) || stream_id == (uint16)(-1))
		v_cmd_buf_set_unique_address_size(head, 7);
	else
		v_cmd_buf_set_address_size(head, 7);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_a_stream_create(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_a_stream_create)(void *user_data, VNodeID node_id, VLayerID stream_id, const char *name);
	VNodeID node_id;
	VLayerID stream_id;
	char name[16];
	
	func_a_stream_create = v_fs_get_user_func(163);
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &stream_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(name[0] == 0)
		printf("receive: verse_send_a_stream_destroy(node_id = %u stream_id = %u ); callback = %p\n", node_id, stream_id, v_fs_get_alias_user_func(163));
	else
		printf("receive: verse_send_a_stream_create(node_id = %u stream_id = %u name = %s ); callback = %p\n", node_id, stream_id, name, v_fs_get_user_func(163));
#endif
	if(name[0] == 0)
	{
		void (* alias_a_stream_destroy)(void *user_data, VNodeID node_id, VLayerID stream_id);
		alias_a_stream_destroy = v_fs_get_alias_user_func(163);
		if(alias_a_stream_destroy != NULL)
			alias_a_stream_destroy(v_fs_get_alias_user_data(163), node_id, stream_id);
		return buffer_pos;
	}
	if(func_a_stream_create != NULL)
		func_a_stream_create(v_fs_get_user_data(163), node_id, stream_id, name);

	return buffer_pos;
}

void verse_send_a_stream_subscribe(VNodeID node_id, VLayerID stream_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 164);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_stream_subscribe(node_id = %u stream_id = %u );\n", node_id, stream_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], stream_id);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], TRUE);
	if(node_id == (uint32)(-1) || stream_id == (uint16)(-1))
		v_cmd_buf_set_unique_address_size(head, 7);
	else
		v_cmd_buf_set_address_size(head, 7);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_a_stream_unsubscribe(VNodeID node_id, VLayerID stream_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 164);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_stream_unsubscribe(node_id = %u stream_id = %u );\n", node_id, stream_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], stream_id);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], FALSE);
	if(node_id == (uint32)(-1) || stream_id == (uint16)(-1))
		v_cmd_buf_set_unique_address_size(head, 7);
	else
		v_cmd_buf_set_address_size(head, 7);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_a_stream_subscribe(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_a_stream_subscribe)(void *user_data, VNodeID node_id, VLayerID stream_id);
	VNodeID node_id;
	VLayerID stream_id;
	uint8	alias_bool;

	func_a_stream_subscribe = v_fs_get_user_func(164);
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &stream_id);
	if(buffer_length < buffer_pos + 1)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &alias_bool);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(!alias_bool)
		printf("receive: verse_send_a_stream_unsubscribe(node_id = %u stream_id = %u ); callback = %p\n", node_id, stream_id, v_fs_get_alias_user_func(164));
	else
		printf("receive: verse_send_a_stream_subscribe(node_id = %u stream_id = %u ); callback = %p\n", node_id, stream_id, v_fs_get_user_func(164));
#endif
	if(!alias_bool)
	{
		void (* alias_a_stream_unsubscribe)(void *user_data, VNodeID node_id, VLayerID stream_id);
		alias_a_stream_unsubscribe = v_fs_get_alias_user_func(164);
		if(alias_a_stream_unsubscribe != NULL)
			alias_a_stream_unsubscribe(v_fs_get_alias_user_data(164), node_id, stream_id);
		return buffer_pos;
	}
	if(func_a_stream_subscribe != NULL)
		func_a_stream_subscribe(v_fs_get_user_data(164), node_id, stream_id);

	return buffer_pos;
}

void verse_send_a_stream(VNodeID node_id, VLayerID stream_id, uint32 time_s, uint32 time_f, VNALayerType type, real64 frequency, const VNASample *data)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 165);	/* Pack the command. */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_stream(node_id = %u stream_id = %u time_s = %u time_f = %u type = %u frequency = %f data = %p );\n", node_id, stream_id, time_s, time_f, type, frequency, data);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], stream_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], time_s);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], time_f);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], frequency);
	{
		unsigned int i;
		switch(type)
		{
			case VN_A_LAYER_INT8 :
				buffer_pos += vnp_raw_pack_uint8_vector(&buf[buffer_pos], ((uint8*)data), VN_A_BLOCK_SIZE_INT8);
			break;
			case VN_A_LAYER_INT16 :
				buffer_pos += vnp_raw_pack_uint16_vector(&buf[buffer_pos], ((uint16*)data), VN_A_BLOCK_SIZE_INT16);
			break;
			case VN_A_LAYER_INT24 :
				buffer_pos += vnp_raw_pack_uint24_vector(&buf[buffer_pos], ((uint32*)data), VN_A_BLOCK_SIZE_INT24);
			break;
			case VN_A_LAYER_INT32 :
				buffer_pos += vnp_raw_pack_uint32_vector(&buf[buffer_pos], ((uint32*)data), VN_A_BLOCK_SIZE_INT32);
			break;
			case VN_A_LAYER_REAL32 :
				buffer_pos += vnp_raw_pack_real32_vector(&buf[buffer_pos], ((real32*)data), VN_A_BLOCK_SIZE_REAL32);
			break;
			case VN_A_LAYER_REAL64 :
				buffer_pos += vnp_raw_pack_real64_vector(&buf[buffer_pos], ((real64*)data), VN_A_BLOCK_SIZE_REAL64);
			break;
		}
	}
	if(node_id == (uint32)(-1) || stream_id == (uint16)(-1))
		v_cmd_buf_set_unique_address_size(head, 7);
	else
		v_cmd_buf_set_address_size(head, 7);
	v_cmd_buf_set_size(head, buffer_pos);
	v_noq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_a_stream(const char *buf, size_t buffer_length)
{
	uint8 enum_temp;
	unsigned int buffer_pos = 0;
	void (* func_a_stream)(void *user_data, VNodeID node_id, VLayerID stream_id, uint32 time_s, uint32 time_f, VNALayerType type, real64 frequency, const VNASample *data);
	VNodeID node_id;
	VLayerID stream_id;
	uint32 time_s;
	uint32 time_f;
	VNALayerType type;
	real64 frequency;
	const VNASample *data;
	
	func_a_stream = v_fs_get_user_func(165);
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &stream_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &time_s);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &time_f);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &enum_temp);
	type = (VNALayerType)enum_temp;
	buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &frequency);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_a_stream(node_id = %u stream_id = %u time_s = %u time_f = %u type = %u frequency = %f ); callback = %p\n", node_id, stream_id, time_s, time_f, type, frequency, v_fs_get_user_func(165));
#endif
	{
		unsigned int i;
		switch(type)
		{
			case VN_A_LAYER_INT8 :
			{
				uint8 data[VN_A_BLOCK_SIZE_INT8];
				buffer_pos += vnp_raw_unpack_uint8_vector(&buf[buffer_pos], data, VN_A_BLOCK_SIZE_INT8);
				if(func_a_stream != NULL)
					func_a_stream(v_fs_get_user_data(165), node_id, stream_id, time_s, time_f, (VNALayerType)type, frequency, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_INT16 :
			{
				uint16 data[VN_A_BLOCK_SIZE_INT16];
				buffer_pos += vnp_raw_unpack_uint16_vector(&buf[buffer_pos], data, VN_A_BLOCK_SIZE_INT16);
				if(func_a_stream != NULL)
					func_a_stream(v_fs_get_user_data(165), node_id, stream_id, time_s, time_f, (VNALayerType)type, frequency, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_INT24 :
			{
				uint32 data[VN_A_BLOCK_SIZE_INT24];
				buffer_pos += vnp_raw_unpack_uint24_vector(&buf[buffer_pos], data, VN_A_BLOCK_SIZE_INT24);
				if(func_a_stream != NULL)
					func_a_stream(v_fs_get_user_data(165), node_id, stream_id, time_s, time_f, (VNALayerType)type, frequency, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_INT32 :
			{
				uint32 data[VN_A_BLOCK_SIZE_INT32];
				buffer_pos += vnp_raw_unpack_uint32_vector(&buf[buffer_pos], data, VN_A_BLOCK_SIZE_INT32);
				if(func_a_stream != NULL)
					func_a_stream(v_fs_get_user_data(165), node_id, stream_id, time_s, time_f, (VNALayerType)type, frequency, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_REAL32 :
			{
				real32 data[VN_A_BLOCK_SIZE_REAL32];
				buffer_pos += vnp_raw_unpack_real32_vector(&buf[buffer_pos], data, VN_A_BLOCK_SIZE_REAL32);
				if(func_a_stream != NULL)
					func_a_stream(v_fs_get_user_data(165), node_id, stream_id, time_s, time_f, (VNALayerType)type, frequency, (VNASample *) data);
				return buffer_pos;
			}
			case VN_A_LAYER_REAL64 :
			{
				real64 data[VN_A_BLOCK_SIZE_REAL64];
				buffer_pos += vnp_raw_unpack_real64_vector(&buf[buffer_pos], data, VN_A_BLOCK_SIZE_REAL64);
				if(func_a_stream != NULL)
					func_a_stream(v_fs_get_user_data(165), node_id, stream_id, time_s, time_f, (VNALayerType)type, frequency, (VNASample *) data);
				return buffer_pos;
			}
		}
	}

	if(func_a_stream != NULL)
		func_a_stream(v_fs_get_user_data(165), node_id, stream_id, time_s, time_f, (VNALayerType)type, frequency, data);

	return buffer_pos;
}

#endif

