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

void verse_send_a_layer_create(VNodeID node_id, VLayerID layer_id, const char *name)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 160);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_layer_create(node_id = %u layer_id = %u name = %s );\n", node_id, layer_id, name);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_a_layer_destroy(VNodeID node_id, VLayerID layer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 160);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_layer_destroy(node_id = %u layer_id = %u );\n", node_id, layer_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_a_layer_create(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_a_layer_create)(void *user_data, VNodeID node_id, VLayerID layer_id, const char *name);
	VNodeID node_id;
	VLayerID layer_id;
	char name[16];
	
	func_a_layer_create = v_fs_get_user_func(160);
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(name[0] == 0)
		printf("receive: verse_send_a_layer_destroy(node_id = %u layer_id = %u ); callback = %p\n", node_id, layer_id, v_fs_get_alias_user_func(160));
	else
		printf("receive: verse_send_a_layer_create(node_id = %u layer_id = %u name = %s ); callback = %p\n", node_id, layer_id, name, v_fs_get_user_func(160));
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
		func_a_layer_create(v_fs_get_user_data(160), node_id, layer_id, name);

	return buffer_pos;
}

void verse_send_a_layer_subscribe(VNodeID node_id, VLayerID layer_id, VNATransferType transfer)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 161);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_layer_subscribe(node_id = %u layer_id = %u transfer = %u );\n", node_id, layer_id, transfer);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)transfer);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_a_layer_unsubscribe(VNodeID node_id, VLayerID layer_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 161);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_layer_unsubscribe(node_id = %u layer_id = %u );\n", node_id, layer_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)-1);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_a_layer_subscribe(const char *buf, size_t buffer_length)
{
	uint8 enum_temp;
	unsigned int buffer_pos = 0;
	void (* func_a_layer_subscribe)(void *user_data, VNodeID node_id, VLayerID layer_id, VNATransferType transfer);
	VNodeID node_id;
	VLayerID layer_id;
	VNATransferType transfer;
	
	func_a_layer_subscribe = v_fs_get_user_func(161);
	if(buffer_length < 7)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &enum_temp);
	transfer = (VNATransferType)enum_temp;
#if defined V_PRINT_RECEIVE_COMMANDS
	if(transfer > VN_A_COMPRESSED_UNSUSTAINED)
		printf("receive: verse_send_a_layer_unsubscribe(node_id = %u layer_id = %u ); callback = %p\n", node_id, layer_id, v_fs_get_alias_user_func(161));
	else
		printf("receive: verse_send_a_layer_subscribe(node_id = %u layer_id = %u transfer = %u ); callback = %p\n", node_id, layer_id, transfer, v_fs_get_user_func(161));
#endif
	if(transfer > VN_A_COMPRESSED_UNSUSTAINED)
	{
		void (* alias_a_layer_unsubscribe)(void *user_data, VNodeID node_id, VLayerID layer_id);
		alias_a_layer_unsubscribe = v_fs_get_alias_user_func(161);
		if(alias_a_layer_unsubscribe != NULL)
			alias_a_layer_unsubscribe(v_fs_get_alias_user_data(161), node_id, layer_id);
		return buffer_pos;
	}
	if(func_a_layer_subscribe != NULL)
		func_a_layer_subscribe(v_fs_get_user_data(161), node_id, layer_id, (VNATransferType)transfer);

	return buffer_pos;
}

void verse_send_a_audio(VNodeID node_id, VLayerID layer_id, uint16 id, uint32 time, uint16 length, VNATransferType transfer, VNALayerType type, void *data)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 162);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_a_audio(node_id = %u layer_id = %u id = %u time = %u length = %u transfer = %u type = %u data = %p );\n", node_id, layer_id, id, time, length, transfer, type, data);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], layer_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], time);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], length);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)transfer);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	{
		unsigned int i;
		switch(type)
		{
			case VN_A_LAYER_INT8 :
				for(i = 0; i < length; i++)
					buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8*)data)[i]);
			break;
			case VN_A_LAYER_INT16 :
				for(i = 0; i < length; i++)
					buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], ((uint16*)data)[i]);
			break;
			case VN_A_LAYER_INT24 :
				for(i = 0; i < length; i++)
					buffer_pos += vnp_raw_pack_uint24(&buf[buffer_pos], ((uint32*)data)[i]);
			break;
			case VN_A_LAYER_INT32 :
				for(i = 0; i < length; i++)
					buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((uint32*)data)[i]);
			break;
			case VN_A_LAYER_REAL32 :
				for(i = 0; i < length; i++)
					buffer_pos += vnp_raw_pack_real32(&buf[buffer_pos], ((real32*)data)[i]);
			break;
			case VN_A_LAYER_REAL64 :
				for(i = 0; i < length; i++)
					buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], ((real64*)data)[i]);
			break;
		}
	}
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_a_audio(const char *buf, size_t buffer_length)
{
	uint8 enum_temp;
	unsigned int buffer_pos = 0;
	void (* func_a_audio)(void *user_data, VNodeID node_id, VLayerID layer_id, uint16 id, uint32 time, uint16 length, VNATransferType transfer, VNALayerType type, void *data);
	VNodeID node_id;
	VLayerID layer_id;
	uint16 id;
	uint32 time;
	uint16 length;
	VNATransferType transfer;
	VNALayerType type;
	void *data;
	
	func_a_audio = v_fs_get_user_func(162);
	if(buffer_length < 16)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &layer_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &time);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &length);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &enum_temp);
	transfer = (VNATransferType)enum_temp;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &enum_temp);
	type = (VNALayerType)enum_temp;
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_a_audio(node_id = %u layer_id = %u id = %u time = %u length = %u transfer = %u type = %u ); callback = %p\n", node_id, layer_id, id, time, length, transfer, type, v_fs_get_user_func(162));
#endif
	{
		unsigned int i;
		switch(type)
		{
			case VN_A_LAYER_INT8 :
			{
				uint8 data[1500];
				for(i = 0; i < length && length < 1500; i++)
					buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &data[i]);
				if(func_a_audio != NULL)
					func_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);
				return buffer_pos;
			}
			case VN_A_LAYER_INT16 :
			{
				uint16 data[750];
				for(i = 0; i < length && length < 750; i++)
					buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &data[i]);
				if(func_a_audio != NULL)
					func_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);
				return buffer_pos;
			}
			case VN_A_LAYER_INT24 :
			{
				uint32 data[325];
				for(i = 0; i < length && length < 325; i++)
					buffer_pos += vnp_raw_unpack_uint24(&buf[buffer_pos], &data[i]);
				if(func_a_audio != NULL)
					func_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);
				return buffer_pos;
			}
			case VN_A_LAYER_INT32 :
			{
				uint32 data[325];
				for(i = 0; i < length && length < 325; i++)
					buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &data[i]);
				if(func_a_audio != NULL)
					func_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);
				return buffer_pos;
			}
			case VN_A_LAYER_REAL32 :
			{
				real32 data[1500];
				for(i = 0; i < length && length < 325; i++)
					buffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &data[i]);
				if(func_a_audio != NULL)
					func_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);
				return buffer_pos;
			}
			case VN_A_LAYER_REAL64 :
			{
				real64 data[162];
				for(i = 0; i < length && length < 162; i++)
					buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &data[i]);
				if(func_a_audio != NULL)
					func_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);
				return buffer_pos;
			}
		}
	}

	if(func_a_audio != NULL)
		func_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);

	return buffer_pos;
}

#endif

