/*
**
*/

#include <stdlib.h>
#include <stdio.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "v_cmd_buf.h"
#include "v_network_que.h"
#include "v_network.h"
#include "v_connection.h"

VSession * verse_send_connect(const char *name, const char *pass, const char *address)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	VSession con;

	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_connect(name = %s pass = %s address = %p);\n", name, pass, address);
	#endif
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], pass, 16);
	con = v_con_connect(address);
	if(con != NULL)
	{
		v_cmd_buf_set_unique_size(head, buffer_pos);
		v_nq_send_buf(v_con_get_network_queue(), head);
	}else
		v_cmd_buf_free(head);
	return con;
}

unsigned int v_unpack_connect(const char *buf, unsigned int buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	VSession (* func_connect)(void *user_data, char *name, char *pass, char *address);
	char name[16];
	char pass[16];	
	func_connect = user_func;
	if(buffer_length < 0)
		return -1;
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
	if(buffer_length < 0 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], pass, 16, buffer_length - buffer_pos);
	if(buffer_length < 0 + buffer_pos)
		return -1;
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_connect(name = %s pass = %s ); callback = %p\n", name, pass, user_func);
	#endif
	if(func_connect != NULL && v_fs_connect_get_address() != NULL)
		func_connect(user_data, name, pass, v_fs_connect_get_address());
	return buffer_pos;
}

VSession * verse_send_connect_accept(VNodeID avatar, const char *address)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	VSession con;

	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 1);/* Packing the command */
	#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_connect_accept(avatar = %u address = %p);\n", avatar, address);
	#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], avatar);
	con = v_con_connect(address);
	if(con != NULL)
	{
		v_cmd_buf_set_unique_size(head, buffer_pos);
		v_nq_send_buf(v_con_get_network_queue(), head);
	}else
		v_cmd_buf_free(head);
	return con;
}
unsigned int v_unpack_connect_accept(const char *buf, unsigned int buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	VSession (* func_connect_accept)(void *user_data, VNodeID avatar, char *address);
	VNodeID avatar;
	char *address;
	func_connect_accept = user_func;
	if(buffer_length < 4)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &avatar);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_connect_accept(avatar = %u ); callback = %p\n", avatar, user_func);
	#endif
	if(func_connect_accept != NULL)
		func_connect_accept(user_data, avatar, address);

	return buffer_pos;
}

#endif
