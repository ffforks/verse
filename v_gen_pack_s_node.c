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

void verse_send_connect_deny(void *address)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 2);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_connect_deny(address = %p );\n", address);
#endif
	{
		void *a;
		a = v_n_create_network_address(0, address);
		buffer_pos = vnp_raw_pack_uint16(&buf[buffer_pos], 1);/* a packet id */
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 2);/* Packing the command */
		v_n_send_data(a, buf, buffer_pos);
		v_n_destroy_network_address(a);
		v_cmd_buf_free(head);
		return;
	}
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_connect_deny(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_connect_deny)(void *user_data, void *address);
	void *address;
	
	func_connect_deny = v_fs_get_user_func(2);
	if(buffer_length < 0)
		return -1;
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_connect_deny(); callback = %p\n", v_fs_get_user_func(2));
#endif
	if(func_connect_deny != NULL)
		func_connect_deny(v_fs_get_user_data(2), address);

	return buffer_pos;
}

void verse_send_connect_terminate(const char *bye)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 3);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_connect_terminate(bye = %s );\n", bye);
#endif
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], bye, 512);
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_connect_terminate(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_connect_terminate)(void *user_data, const char *bye);
	char bye[512];
	
	func_connect_terminate = v_fs_get_user_func(3);
	if(buffer_length < 0)
		return -1;
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], bye, 512, buffer_length - buffer_pos);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_connect_terminate(bye = %s ); callback = %p\n", bye, v_fs_get_user_func(3));
#endif
	if(func_connect_terminate != NULL)
		func_connect_terminate(v_fs_get_user_data(3), bye);

	return buffer_pos;
}

void verse_send_get_time(uint32 time)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 4);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_get_time(time = %u );\n", time);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], time);
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_get_time(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_get_time)(void *user_data, uint32 time);
	uint32 time;
	
	func_get_time = v_fs_get_user_func(4);
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &time);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_get_time(time = %u ); callback = %p\n", time, v_fs_get_user_func(4));
#endif
	if(func_get_time != NULL)
		func_get_time(v_fs_get_user_data(4), time);

	return buffer_pos;
}

void verse_send_ping(const char *address, const char *text)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 5);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_ping(address = %s text = %s );\n", address, text);
#endif
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], address, 512);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], text, 512);
	{
		void *a;
		a = v_n_create_network_address(0, address);
		buffer_pos = vnp_raw_pack_uint16(&buf[buffer_pos], 1);/* a packet id */
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 5);/* Packing the command */
		buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], text, 500);
		v_n_send_data(a, buf, buffer_pos);
		v_n_destroy_network_address(a);
		v_cmd_buf_free(head);
		return;
	}
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_ping(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_ping)(void *user_data, const char *address, const char *text);
	char address[512];
	char text[512];
	
	func_ping = v_fs_get_user_func(5);
	if(buffer_length < 0)
		return -1;
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], address, 512, buffer_length - buffer_pos);
	if(buffer_length < 0 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], text, 512, buffer_length - buffer_pos);
	if(buffer_length < 0 + buffer_pos)
		return -1;
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_ping(address = %s text = %s ); callback = %p\n", address, text, v_fs_get_user_func(5));
#endif
	if(func_ping != NULL)
		func_ping(v_fs_get_user_data(5), address, text);

	return buffer_pos;
}

void verse_send_packet_ack(uint32 packet_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 7);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], packet_id);
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_ack_nak_buf(v_con_get_network_queue(), head);
	return;
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_packet_ack(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_packet_ack)(void *user_data, uint32 packet_id);
	uint32 packet_id;
	
	func_packet_ack = v_fs_get_user_func(7);
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &packet_id);
#if defined V_PRINT_RECEIVE_COMMANDS
#endif
	if(func_packet_ack != NULL)
		func_packet_ack(v_fs_get_user_data(7), packet_id);

	return buffer_pos;
}

void verse_send_packet_nak(uint32 packet_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 8);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], packet_id);
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_ack_nak_buf(v_con_get_network_queue(), head);
	return;
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_packet_nak(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_packet_nak)(void *user_data, uint32 packet_id);
	uint32 packet_id;
	
	func_packet_nak = v_fs_get_user_func(8);
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &packet_id);
#if defined V_PRINT_RECEIVE_COMMANDS
#endif
	if(func_packet_nak != NULL)
		func_packet_nak(v_fs_get_user_data(8), packet_id);

	return buffer_pos;
}

void verse_send_node_list(uint32 mask)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 9);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_node_list(mask = %u );\n", mask);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], mask);
	address_size = buffer_pos;
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_node_list(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_node_list)(void *user_data, uint32 mask);
	uint32 mask;
	
	func_node_list = v_fs_get_user_func(9);
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &mask);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_node_list(mask = %u ); callback = %p\n", mask, v_fs_get_user_func(9));
#endif
	if(func_node_list != NULL)
		func_node_list(v_fs_get_user_data(9), mask);

	return buffer_pos;
}

void verse_send_node_create(VNodeID node_id, VNodeType type, VNodeID owner_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 10);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_node_create(node_id = %u type = %u owner_id = %u );\n", node_id, type, owner_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], owner_id);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_node_destroy(VNodeID node_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 10);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_node_destroy(node_id = %u );\n", node_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)-1);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 0);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_node_create(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_node_create)(void *user_data, VNodeID node_id, VNodeType type, VNodeID owner_id);
	VNodeID node_id;
	uint8 type;
	VNodeID owner_id;
	
	func_node_create = v_fs_get_user_func(10);
	if(buffer_length < 9)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &type);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &owner_id);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(owner_id == -1 || type >= V_NT_NUM_TYPES)
		printf("receive: verse_send_node_destroy(node_id = %u ); callback = %p\n", node_id, v_fs_get_alias_user_func(10));
	else
		printf("receive: verse_send_node_create(node_id = %u type = %u owner_id = %u ); callback = %p\n", node_id, type, owner_id, v_fs_get_user_func(10));
#endif
	if(owner_id == -1 || type >= V_NT_NUM_TYPES)
	{
		void (* alias_node_destroy)(void *user_data, VNodeID node_id);
		alias_node_destroy = v_fs_get_alias_user_func(10);
		if(alias_node_destroy != NULL)
			alias_node_destroy(v_fs_get_alias_user_data(10), node_id);
		return buffer_pos;
	}
	if(func_node_create != NULL)
		func_node_create(v_fs_get_user_data(10), node_id, (VNodeType)type, owner_id);

	return buffer_pos;
}

void verse_send_node_subscribe(VNodeID node_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 11);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_node_subscribe(node_id = %u );\n", node_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], TRUE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_node_unsubscribe(VNodeID node_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 11);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_node_unsubscribe(node_id = %u );\n", node_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], FALSE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_node_subscribe(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_node_subscribe)(void *user_data, VNodeID node_id);
	VNodeID node_id;
	char alias_bool;

	func_node_subscribe = v_fs_get_user_func(11);
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(alias_bool)
		printf("receive: verse_send_node_unsubscribe(node_id = %u ); callback = %p\n", node_id, v_fs_get_alias_user_func(11));
	else
		printf("receive: verse_send_node_subscribe(node_id = %u ); callback = %p\n", node_id, v_fs_get_user_func(11));
#endif
	if(buffer_length < buffer_pos + 1)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &alias_bool);
	if(!alias_bool)
	{
		void (* alias_node_unsubscribe)(void *user_data, VNodeID node_id);
		alias_node_unsubscribe = v_fs_get_alias_user_func(11);
		if(alias_node_unsubscribe != NULL)
			alias_node_unsubscribe(v_fs_get_alias_user_data(11), node_id);
		return buffer_pos;
	}
	if(func_node_subscribe != NULL)
		func_node_subscribe(v_fs_get_user_data(11), node_id);

	return buffer_pos;
}

void verse_send_tag_group_create(VNodeID node_id, uint16 group_id, const char *name)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 16);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_tag_group_create(node_id = %u group_id = %u name = %s );\n", node_id, group_id, name);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_tag_group_destroy(VNodeID node_id, uint16 group_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 16);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_tag_group_destroy(node_id = %u group_id = %u );\n", node_id, group_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_tag_group_create(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_tag_group_create)(void *user_data, VNodeID node_id, uint16 group_id, const char *name);
	VNodeID node_id;
	uint16 group_id;
	char name[16];
	
	func_tag_group_create = v_fs_get_user_func(16);
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &group_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(name[0] == 0)
		printf("receive: verse_send_tag_group_destroy(node_id = %u group_id = %u ); callback = %p\n", node_id, group_id, v_fs_get_alias_user_func(16));
	else
		printf("receive: verse_send_tag_group_create(node_id = %u group_id = %u name = %s ); callback = %p\n", node_id, group_id, name, v_fs_get_user_func(16));
#endif
	if(name[0] == 0)
	{
		void (* alias_tag_group_destroy)(void *user_data, VNodeID node_id, uint16 group_id);
		alias_tag_group_destroy = v_fs_get_alias_user_func(16);
		if(alias_tag_group_destroy != NULL)
			alias_tag_group_destroy(v_fs_get_alias_user_data(16), node_id, group_id);
		return buffer_pos;
	}
	if(func_tag_group_create != NULL)
		func_tag_group_create(v_fs_get_user_data(16), node_id, group_id, name);

	return buffer_pos;
}

void verse_send_tag_group_subscribe(VNodeID node_id, uint16 group_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 17);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_tag_group_subscribe(node_id = %u group_id = %u );\n", node_id, group_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], TRUE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_tag_group_unsubscribe(VNodeID node_id, uint16 group_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 17);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_tag_group_unsubscribe(node_id = %u group_id = %u );\n", node_id, group_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], FALSE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_tag_group_subscribe(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_tag_group_subscribe)(void *user_data, VNodeID node_id, uint16 group_id);
	VNodeID node_id;
	uint16 group_id;
	char alias_bool;

	func_tag_group_subscribe = v_fs_get_user_func(17);
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &group_id);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(alias_bool)
		printf("receive: verse_send_tag_group_unsubscribe(node_id = %u group_id = %u ); callback = %p\n", node_id, group_id, v_fs_get_alias_user_func(17));
	else
		printf("receive: verse_send_tag_group_subscribe(node_id = %u group_id = %u ); callback = %p\n", node_id, group_id, v_fs_get_user_func(17));
#endif
	if(buffer_length < buffer_pos + 1)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &alias_bool);
	if(!alias_bool)
	{
		void (* alias_tag_group_unsubscribe)(void *user_data, VNodeID node_id, uint16 group_id);
		alias_tag_group_unsubscribe = v_fs_get_alias_user_func(17);
		if(alias_tag_group_unsubscribe != NULL)
			alias_tag_group_unsubscribe(v_fs_get_alias_user_data(17), node_id, group_id);
		return buffer_pos;
	}
	if(func_tag_group_subscribe != NULL)
		func_tag_group_subscribe(v_fs_get_user_data(17), node_id, group_id);

	return buffer_pos;
}

void verse_send_tag_create(VNodeID node_id, uint16 group_id, uint16 tag_id, const char *name, VNTagType type, VNTag *tag)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 18);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_tag_create(node_id = %u group_id = %u tag_id = %u name = %s type = %u tag = %p );\n", node_id, group_id, tag_id, name, type, tag);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], tag_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	if(type > VN_TAG_BLOB)
	{
		v_cmd_buf_free(head);
		return;
	}
	switch(type)
	{
		case VN_TAG_BOOLEAN :
			buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((VNTag *)tag)->vboolean);
		break;
		case VN_TAG_UINT32 :
			buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((VNTag *)tag)->vuint32);
		break;
		case VN_TAG_REAL64 :
			buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], ((VNTag *)tag)->vreal64);
		break;
		case VN_TAG_STRING :
		{
			unsigned int i;
			for(i = 0; ((VNTag *)tag)->vstring[i] != 0 && i < VN_TAG_MAX_BLOB_SIZE; i++)
				buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((VNTag *)tag)->vstring[i]);
			buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0);
		}
		break;
		case VN_TAG_REAL64_VEC3 :
		{
			buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], ((VNTag *)tag)->vreal64_vec3[0]);
			buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], ((VNTag *)tag)->vreal64_vec3[1]);
			buffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], ((VNTag *)tag)->vreal64_vec3[2]);
		}
		break;
		case VN_TAG_LINK :
		{
			buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((VNTag *)tag)->vlink);
		}
		break;
		case VN_TAG_ANIMATION :
		{
			buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((VNTag *)tag)->vanimation.curve);
			buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((VNTag *)tag)->vanimation.start);
			buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((VNTag *)tag)->vanimation.end);
		}
		break;
		case VN_TAG_BLOB :
		{
			unsigned int i;
			if(((VNTag *)tag)->vblob.size > VN_TAG_MAX_BLOB_SIZE)
				((VNTag *)tag)->vblob.size = VN_TAG_MAX_BLOB_SIZE;
			buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], ((VNTag *)tag)->vblob.size);
			for(i = 0; i < ((VNTag *)tag)->vblob.size; i++)
				buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8 *)((VNTag *)tag)->vblob.blob)[i]);
		}
		break;
	}
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_tag_destroy(VNodeID node_id, uint16 group_id, uint16 tag_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 18);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_tag_destroy(node_id = %u group_id = %u tag_id = %u );\n", node_id, group_id, tag_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], tag_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)-1);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_tag_create(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_tag_create)(void *user_data, VNodeID node_id, uint16 group_id, uint16 tag_id, const char *name, VNTagType type, VNTag *tag);
	VNodeID node_id;
	uint16 group_id;
	uint16 tag_id;
	char name[16];
	uint8 type;
	VNTag *tag;
	
	func_tag_create = v_fs_get_user_func(18);
	if(buffer_length < 8)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &group_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &tag_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
	if(buffer_length < 1 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &type);
#if defined V_PRINT_RECEIVE_COMMANDS
	if(type >= VN_TAG_TYPE_COUNT)
		printf("receive: verse_send_tag_destroy(node_id = %u group_id = %u tag_id = %u ); callback = %p\n", node_id, group_id, tag_id, v_fs_get_alias_user_func(18));
	else
		printf("receive: verse_send_tag_create(node_id = %u group_id = %u tag_id = %u name = %s type = %u ); callback = %p\n", node_id, group_id, tag_id, name, type, v_fs_get_user_func(18));
#endif
	if(type < VN_TAG_TYPE_COUNT)
	{
		VNTag tag;
		unsigned int i;
		char string[VN_TAG_MAX_BLOB_SIZE];
		switch(type)
		{
			case VN_TAG_BOOLEAN :
				buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &tag.vboolean);
			break;
			case VN_TAG_UINT32 :
				buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &tag.vuint32);
			break;
			case VN_TAG_REAL64 :
				buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &tag.vreal64);
			break;
			case VN_TAG_STRING :
			{
				tag.vstring = string;
				vnp_raw_unpack_string(&buf[buffer_pos], string, VN_TAG_MAX_BLOB_SIZE, buffer_length - buffer_pos);
			}
			break;
			case VN_TAG_REAL64_VEC3 :
			{
				buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &tag.vreal64_vec3[0]);
				buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &tag.vreal64_vec3[1]);
				buffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &tag.vreal64_vec3[2]);
			}
			break;
			case VN_TAG_LINK :
			{
				buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &tag.vlink);
			}
			break;
			case VN_TAG_ANIMATION :
			{
				buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &tag.vanimation.curve);
				buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &tag.vanimation.start);
				buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &tag.vanimation.end);
			}
			break;
			case VN_TAG_BLOB :
			{
				buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &tag.vblob.size);
				if(tag.vblob.size > VN_TAG_MAX_BLOB_SIZE)
					tag.vblob.size = VN_TAG_MAX_BLOB_SIZE;
				tag.vblob.blob = string;
				for(i = 0; i < tag.vblob.size; i++)
					buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &string[i]);
			}
			break;
		}
		if(func_tag_create != NULL)
		func_tag_create(v_fs_get_user_data(18), node_id, group_id, tag_id, name, type, &tag);
		return buffer_pos;
	}

	if(type >= VN_TAG_TYPE_COUNT)
	{
		void (* alias_tag_destroy)(void *user_data, VNodeID node_id, uint16 group_id, uint16 tag_id);
		alias_tag_destroy = v_fs_get_alias_user_func(18);
		if(alias_tag_destroy != NULL)
			alias_tag_destroy(v_fs_get_alias_user_data(18), node_id, group_id, tag_id);
		return buffer_pos;
	}
	if(func_tag_create != NULL)
		func_tag_create(v_fs_get_user_data(18), node_id, group_id, tag_id, name, (VNTagType)type, tag);

	return buffer_pos;
}

void verse_send_node_name_set(VNodeID node_id, const char *name)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 19);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: verse_send_node_name_set(node_id = %u name = %s );\n", node_id, name);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 512);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_node_name_set(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_node_name_set)(void *user_data, VNodeID node_id, const char *name);
	VNodeID node_id;
	char name[512];
	
	func_node_name_set = v_fs_get_user_func(19);
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 512, buffer_length - buffer_pos);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_node_name_set(node_id = %u name = %s ); callback = %p\n", node_id, name, v_fs_get_user_func(19));
#endif
	if(func_node_name_set != NULL)
		func_node_name_set(v_fs_get_user_data(19), node_id, name);

	return buffer_pos;
}

#endif

