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

void verse_send_o_transform_pos_real32(VNodeID node_id, uint32 time, real32 *pos, real32 *speed, real32 *accelerate, real32 *drag_normal, real32 drag)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 32);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_transform_pos_real32(node_id = %u time = %u pos = %p speed = %p accelerate = %p drag_normal = %p drag = %f );\n", node_id, time, pos, speed, accelerate, drag_normal, drag);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], time);
	{
		unsigned char mask = 0;
		unsigned int cmd;
		cmd = buffer_pos++;
		buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], pos[0]);
		buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], pos[1]);
		buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], pos[2]);
		if(speed[0] > 0.0000001 || speed[0] < -0.0000001 || speed[1] > 0.0000001 || speed[1] < -0.0000001 || speed[2] > 0.0000001 || speed[2] < -0.0000001)
		{
			mask |= 1;
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], speed[0]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], speed[1]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], speed[2]);
		}
		if(accelerate[0] > 0.0000001 || accelerate[0] < -0.0000001 || accelerate[1] > 0.0000001 || accelerate[1] < -0.0000001 || accelerate[2] > 0.0000001 || accelerate[2] < -0.0000001)
		{
			mask |= 2;
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], accelerate[0]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], accelerate[1]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], accelerate[2]);
		}
		if(drag_normal[0] > 0.0000001 || drag_normal[0] < -0.0000001 || drag_normal[1] > 0.0000001 || drag_normal[1] < -0.0000001 || drag_normal[2] > 0.0000001 || drag_normal[2] < -0.0000001)
		{
			mask |= 4;
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], drag_normal[0]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], drag_normal[1]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], drag_normal[2]);
		}
		if(drag > 0.0000001 || drag < -0.0000001)
		{
			mask |= 8;
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], drag);
		}
		vnp_raw_pack_uint8(&buf[cmd], mask);
	}if(FALSE)
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_transform_pos_real32(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_transform_pos_real32)(void *user_data, VNodeID node_id, uint32 time, real32 *pos, real32 *speed, real32 *accelerate, real32 *drag_normal, real32 drag);
	VNodeID node_id;
	uint32 time;
	real32 *pos;
	real32 *speed;
	real32 *accelerate;
	real32 *drag_normal;
	real32 drag;
	
	func_o_transform_pos_real32 = user_func;
	if(buffer_length < 8)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &time);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_transform_pos_real32(node_id = %u time = %u drag = %f ); callback = %p\n", node_id, time, drag, user_func);
#endif
	{
		float output[4][3];
		unsigned int i, j;
		char mask, pow = 1;
		buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &mask);
		for(j = 0; j < 3; j++)
			buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &output[0][j]);
		for(i = 1; i < 4; i++)
		{
			if((mask & pow) != 0)
				for(j = 0; j < 3; j++)
					buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &output[i][j]);
			else
				for(j = 0; j < 3; j++)
					output[i][j] = 0;
			pow *= 2;
		}
		if((mask & pow) != 0)
			buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &drag);
		if(func_o_transform_pos_real32 != NULL)
			func_o_transform_pos_real32(user_data, node_id, time, &output[0][0], &output[1][0], &output[2][0], &output[3][0], drag);
		return buffer_pos;
	}

	if(func_o_transform_pos_real32 != NULL)
		func_o_transform_pos_real32(user_data, node_id, time, pos, speed, accelerate, drag_normal, drag);

	return buffer_pos;
}

void verse_send_o_transform_rot_real32(VNodeID node_id, uint32 time, real32 *rot, real32 *speed, real32 *accelerate, real32 *drag_normal, real32 drag)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 33);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_transform_rot_real32(node_id = %u time = %u rot = %p speed = %p accelerate = %p drag_normal = %p drag = %f );\n", node_id, time, rot, speed, accelerate, drag_normal, drag);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], time);
	{
		unsigned char mask = 0;
		unsigned int cmd;
		cmd = buffer_pos++;
		buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], rot[0]);
		buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], rot[1]);
		buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], rot[2]);
		buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], rot[3]);
		if(speed[0] > 0.0000001 || speed[0] < -0.0000001 || speed[1] > 0.0000001 || speed[1] < -0.0000001 || speed[2] > 0.0000001 || speed[3] < -0.0000001 || speed[3] < -0.0000001)
		{
			mask |= 1;
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], speed[0]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], speed[1]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], speed[2]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], speed[3]);
		}
		if(accelerate[0] > 0.0000001 || accelerate[0] < -0.0000001 || accelerate[1] > 0.0000001 || accelerate[1] < -0.0000001 || accelerate[2] > 0.0000001 || accelerate[2] < -0.0000001 || accelerate[3] > 0.0000001 || accelerate[3] < -0.0000001)
		{
			mask |= 2;
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], accelerate[0]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], accelerate[1]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], accelerate[2]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], accelerate[3]);
		}
		if(drag_normal[0] > 0.0000001 || drag_normal[0] < -0.0000001 || drag_normal[1] > 0.0000001 || drag_normal[1] < -0.0000001 || drag_normal[2] > 0.0000001 || drag_normal[2] < -0.0000001 || drag_normal[3] > 0.0000001 || drag_normal[3] < -0.0000001)
		{
			mask |= 4;
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], drag_normal[0]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], drag_normal[1]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], drag_normal[2]);
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], drag_normal[3]);
		}
		if(drag > 0.0000001 || drag < -0.0000001)
		{
			mask |= 8;
			buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], drag);
		}
		vnp_raw_pack_uint8(&buf[cmd], mask);
	}if(FALSE)
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_transform_rot_real32(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_transform_rot_real32)(void *user_data, VNodeID node_id, uint32 time, real32 *rot, real32 *speed, real32 *accelerate, real32 *drag_normal, real32 drag);
	VNodeID node_id;
	uint32 time;
	real32 *rot;
	real32 *speed;
	real32 *accelerate;
	real32 *drag_normal;
	real32 drag;
	
	func_o_transform_rot_real32 = user_func;
	if(buffer_length < 8)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &time);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_transform_rot_real32(node_id = %u time = %u drag = %f ); callback = %p\n", node_id, time, drag, user_func);
#endif
	{
		float output[4][4];
		unsigned int i, j;
		char mask, pow = 1;
		buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &mask);
		for(j = 0; j < 4; j++)
			buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &output[0][j]);
		for(i = 1; i < 4; i++)
		{
			if((mask & pow) != 0)
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &output[i][j]);
			else
				for(j = 0; j < 4; j++)
					output[i][j] = 0;
			pow *= 2;
		}
		if((mask & pow) != 0)
			buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &drag);
		if(func_o_transform_rot_real32 != NULL)
			func_o_transform_rot_real32(user_data, node_id, time, &output[0][0], &output[1][0], &output[2][0], &output[3][0], drag);
		return buffer_pos;
	}

	if(func_o_transform_rot_real32 != NULL)
		func_o_transform_rot_real32(user_data, node_id, time, rot, speed, accelerate, drag_normal, drag);

	return buffer_pos;
}

void verse_send_o_transform_scale_real32(VNodeID node_id, real32 scale_x, real32 scale_y, real32 scale_z)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 34);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_transform_scale_real32(node_id = %u scale_x = %f scale_y = %f scale_z = %f );\n", node_id, scale_x, scale_y, scale_z);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], scale_x);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], scale_y);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], scale_z);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_transform_scale_real32(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_transform_scale_real32)(void *user_data, VNodeID node_id, real32 scale_x, real32 scale_y, real32 scale_z);
	VNodeID node_id;
	real32 scale_x;
	real32 scale_y;
	real32 scale_z;
	
	func_o_transform_scale_real32 = user_func;
	if(buffer_length < 16)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &scale_x);
	buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &scale_y);
	buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &scale_z);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_transform_scale_real32(node_id = %u scale_x = %f scale_y = %f scale_z = %f ); callback = %p\n", node_id, scale_x, scale_y, scale_z, user_func);
#endif
	if(func_o_transform_scale_real32 != NULL)
		func_o_transform_scale_real32(user_data, node_id, scale_x, scale_y, scale_z);

	return buffer_pos;
}

void verse_send_o_transform_pos_real64(VNodeID node_id, uint32 time, real64 *pos, real64 *speed, real64 *accelerate, real64 *drag_normal, real64 drag)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 35);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_transform_pos_real64(node_id = %u time = %u pos = %p speed = %p accelerate = %p drag_normal = %p drag = %f );\n", node_id, time, pos, speed, accelerate, drag_normal, drag);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], time);
	{
		unsigned char mask = 0;
		unsigned int cmd;
		cmd = buffer_pos++;
		buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], pos[0]);
		buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], pos[1]);
		buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], pos[2]);
		if(speed[0] > 0.0000001 || speed[0] < -0.0000001 || speed[1] > 0.0000001 || speed[1] < -0.0000001 || speed[2] > 0.0000001 || speed[2] < -0.0000001)
		{
			mask |= 1;
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], speed[0]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], speed[1]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], speed[2]);
		}
		if(accelerate[0] > 0.0000001 || accelerate[0] < -0.0000001 || accelerate[1] > 0.0000001 || accelerate[1] < -0.0000001 || accelerate[2] > 0.0000001 || accelerate[2] < -0.0000001)
		{
			mask |= 2;
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], accelerate[0]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], accelerate[1]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], accelerate[2]);
		}
		if(drag_normal[0] > 0.0000001 || drag_normal[0] < -0.0000001 || drag_normal[1] > 0.0000001 || drag_normal[1] < -0.0000001 || drag_normal[2] > 0.0000001 || drag_normal[2] < -0.0000001)
		{
			mask |= 4;
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag_normal[0]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag_normal[1]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag_normal[2]);
		}
		if(drag > 0.0000001 || drag < -0.0000001)
		{
			mask |= 8;
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag);
		}
		vnp_raw_pack_uint8(&buf[cmd], mask);
	}if(FALSE)
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_transform_pos_real64(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_transform_pos_real64)(void *user_data, VNodeID node_id, uint32 time, real64 *pos, real64 *speed, real64 *accelerate, real64 *drag_normal, real64 drag);
	VNodeID node_id;
	uint32 time;
	real64 *pos;
	real64 *speed;
	real64 *accelerate;
	real64 *drag_normal;
	real64 drag;
	
	func_o_transform_pos_real64 = user_func;
	if(buffer_length < 8)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &time);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_transform_pos_real64(node_id = %u time = %u drag = %f ); callback = %p\n", node_id, time, drag, user_func);
#endif
	{
		double output[4][3];
		unsigned int i, j;
		char mask, pow = 1;
		buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &mask);
		for(j = 0; j < 3; j++)
			buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &output[0][j]);
		for(i = 1; i < 4; i++)
		{
			if((mask & pow) != 0)
				for(j = 0; j < 3; j++)
					buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &output[i][j]);
			else
				for(j = 0; j < 3; j++)
					output[i][j] = 0;
			pow *= 2;
		}
		if((mask & pow) != 0)
			buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &drag);
		if(func_o_transform_pos_real64 != NULL)
			func_o_transform_pos_real64(user_data, node_id, time, &output[0][0], &output[1][0], &output[2][0], &output[3][0], drag);
		return buffer_pos;
	}

	if(func_o_transform_pos_real64 != NULL)
		func_o_transform_pos_real64(user_data, node_id, time, pos, speed, accelerate, drag_normal, drag);

	return buffer_pos;
}

void verse_send_o_transform_rot_real64(VNodeID node_id, uint32 time, real64 *rot, real64 *speed, real64 *accelerate, real64 *drag_normal, real64 drag)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 36);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_transform_rot_real64(node_id = %u time = %u rot = %p speed = %p accelerate = %p drag_normal = %p drag = %f );\n", node_id, time, rot, speed, accelerate, drag_normal, drag);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], time);
	{
		char bitfeald = 0;
		unsigned int cmd;
		cmd = buffer_pos++;
		buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], rot[0]);
		buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], rot[1]);
		buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], rot[2]);
		buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], rot[3]);
		if(speed[0] > 0.0000001 || speed[0] < -0.0000001 || speed[1] > 0.0000001 || speed[1] < -0.0000001 || speed[2] > 0.0000001 || speed[2] < -0.0000001 || speed[3] > 0.0000001 || speed[3] < -0.0000001)
		{
			bitfeald += 1;
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], speed[0]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], speed[1]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], speed[2]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], speed[3]);
		}
		if(accelerate[0] > 0.0000001 || accelerate[0] < -0.0000001 || accelerate[1] > 0.0000001 || accelerate[1] < -0.0000001 || accelerate[2] > 0.0000001 || accelerate[2] < -0.0000001 || accelerate[3] > 0.0000001 || accelerate[3] < -0.0000001)
		{
			bitfeald += 2;
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], accelerate[0]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], accelerate[1]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], accelerate[2]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], accelerate[3]);
		}
		if(drag_normal[0] > 0.0000001 || drag_normal[0] < -0.0000001 || drag_normal[1] > 0.0000001 || drag_normal[1] < -0.0000001 || drag_normal[2] > 0.0000001 || drag_normal[2] < -0.0000001 || drag_normal[3] > 0.0000001 || drag_normal[3] < -0.0000001)
		{
			bitfeald += 4;
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag_normal[0]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag_normal[1]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag_normal[2]);
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag_normal[3]);
		}
		if(drag > 0.0000001 || drag < -0.0000001)
		{
			bitfeald += 8;
			buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag);
		}
		vnp_raw_pack_uint8(&buf[cmd], bitfeald);
	}if(FALSE)
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], drag);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_transform_rot_real64(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_transform_rot_real64)(void *user_data, VNodeID node_id, uint32 time, real64 *rot, real64 *speed, real64 *accelerate, real64 *drag_normal, real64 drag);
	VNodeID node_id;
	uint32 time;
	real64 *rot;
	real64 *speed;
	real64 *accelerate;
	real64 *drag_normal;
	real64 drag;
	
	func_o_transform_rot_real64 = user_func;
	if(buffer_length < 8)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &time);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_transform_rot_real64(node_id = %u time = %u drag = %f ); callback = %p\n", node_id, time, drag, user_func);
#endif
	{
		double output[4][4];
		unsigned int i, j;
		char mask, pow = 1;
		buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &mask);
		for(j = 0; j < 4; j++)
			buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &output[0][j]);
		for(i = 1; i < 4; i++)
		{
			if((mask & pow) != 0)
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &output[i][j]);
			else
				for(j = 0; j < 4; j++)
					output[i][j] = 0;
			pow *= 2;
		}
		if((mask & pow) != 0)
			buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &drag);
		if(func_o_transform_rot_real64 != NULL)
			func_o_transform_rot_real64(user_data, node_id, time, &output[0][0], &output[1][0], &output[2][0], &output[3][0], drag);
		return buffer_pos;
	}

	if(func_o_transform_rot_real64 != NULL)
		func_o_transform_rot_real64(user_data, node_id, time, rot, speed, accelerate, drag_normal, drag);

	return buffer_pos;
}

void verse_send_o_transform_scale_real64(VNodeID node_id, real64 scale_x, real64 scale_y, real64 scale_z)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 37);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_transform_scale_real64(node_id = %u scale_x = %f scale_y = %f scale_z = %f );\n", node_id, scale_x, scale_y, scale_z);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], scale_x);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], scale_y);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], scale_z);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_transform_scale_real64(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_transform_scale_real64)(void *user_data, VNodeID node_id, real64 scale_x, real64 scale_y, real64 scale_z);
	VNodeID node_id;
	real64 scale_x;
	real64 scale_y;
	real64 scale_z;
	
	func_o_transform_scale_real64 = user_func;
	if(buffer_length < 28)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &scale_x);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &scale_y);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &scale_z);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_transform_scale_real64(node_id = %u scale_x = %f scale_y = %f scale_z = %f ); callback = %p\n", node_id, scale_x, scale_y, scale_z, user_func);
#endif
	if(func_o_transform_scale_real64 != NULL)
		func_o_transform_scale_real64(user_data, node_id, scale_x, scale_y, scale_z);

	return buffer_pos;
}

void verse_send_o_transform_subscribe(VNodeID node_id, VNORealFormat type)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 38);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_transform_subscribe(node_id = %u type = %u );\n", node_id, type);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], TRUE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_o_transform_unsubscribe(VNodeID node_id, VNORealFormat type)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 38);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_transform_unsubscribe(node_id = %u type = %u );\n", node_id, type);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], (uint8)type);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], FALSE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_transform_subscribe(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_transform_subscribe)(void *user_data, VNodeID node_id, VNORealFormat type);
	VNodeID node_id;
	uint8 type;
	char alias_bool;

	func_o_transform_subscribe = user_func;
	if(buffer_length < 5)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &type);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_transform_subscribe(node_id = %u type = %u ); callback = %p\n", node_id, type, user_func);
#endif
	if(buffer_length < buffer_pos + 1)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &alias_bool);
	if(!alias_bool)
	{
		void (* alias_o_transform_unsubscribe)(void *user_data, VNodeID node_id, VNORealFormat type);
		alias_o_transform_unsubscribe = v_fs_get_alias_user_func(38);
		if(alias_o_transform_unsubscribe != NULL)
			alias_o_transform_unsubscribe(v_fs_get_alias_user_data(38), node_id, (VNORealFormat)type);
		return buffer_pos;
	}
	if(func_o_transform_subscribe != NULL)
		func_o_transform_subscribe(user_data, node_id, (VNORealFormat)type);

	return buffer_pos;
}

void verse_send_o_set_light(VNodeID node_id, real64 light_r, real64 light_g, real64 light_b)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 39);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_set_light(node_id = %u light_r = %f light_g = %f light_b = %f );\n", node_id, light_r, light_g, light_b);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], light_r);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], light_g);
	buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], light_b);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_set_light(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_set_light)(void *user_data, VNodeID node_id, real64 light_r, real64 light_g, real64 light_b);
	VNodeID node_id;
	real64 light_r;
	real64 light_g;
	real64 light_b;
	
	func_o_set_light = user_func;
	if(buffer_length < 28)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &light_r);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &light_g);
	buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &light_b);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_set_light(node_id = %u light_r = %f light_g = %f light_b = %f ); callback = %p\n", node_id, light_r, light_g, light_b, user_func);
#endif
	if(func_o_set_light != NULL)
		func_o_set_light(user_data, node_id, light_r, light_g, light_b);

	return buffer_pos;
}

void verse_send_o_link_set(VNodeID node_id, uint16 link_id, VNodeID link, const char *name, uint32 target_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 40);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_link_set(node_id = %u link_id = %u link = %u name = %s target_id = %u );\n", node_id, link_id, link, name, target_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], link_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], link);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], target_id);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], TRUE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_o_link_destroy(VNodeID node_id, uint16 link_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 40);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_link_destroy(node_id = %u link_id = %u );\n", node_id, link_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], link_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 0);
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], -1);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], FALSE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_link_set(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_link_set)(void *user_data, VNodeID node_id, uint16 link_id, VNodeID link, const char *name, uint32 target_id);
	VNodeID node_id;
	uint16 link_id;
	VNodeID link;
	char name[16];
	uint32 target_id;
	char alias_bool;

	func_o_link_set = user_func;
	if(buffer_length < 10)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &link_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &link);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
	if(buffer_length < 4 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &target_id);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_link_set(node_id = %u link_id = %u link = %u name = %s target_id = %u ); callback = %p\n", node_id, link_id, link, name, target_id, user_func);
#endif
	if(buffer_length < buffer_pos + 1)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &alias_bool);
	if(!alias_bool)
	{
		void (* alias_o_link_destroy)(void *user_data, VNodeID node_id, uint16 link_id);
		alias_o_link_destroy = v_fs_get_alias_user_func(40);
		if(alias_o_link_destroy != NULL)
			alias_o_link_destroy(v_fs_get_alias_user_data(40), node_id, link_id);
		return buffer_pos;
	}
	if(func_o_link_set != NULL)
		func_o_link_set(user_data, node_id, link_id, link, name, target_id);

	return buffer_pos;
}

void verse_send_o_method_group_create(VNodeID node_id, uint8 group_id, const char *name)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 41);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_method_group_create(node_id = %u group_id = %u name = %s );\n", node_id, group_id, name);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], group_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_o_method_group_destroy(VNodeID node_id, uint8 group_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_50);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 41);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_method_group_destroy(node_id = %u group_id = %u );\n", node_id, group_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], group_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_method_group_create(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_method_group_create)(void *user_data, VNodeID node_id, uint8 group_id, const char *name);
	VNodeID node_id;
	uint8 group_id;
	char name[16];
	
	func_o_method_group_create = user_func;
	if(buffer_length < 5)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &group_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_method_group_create(node_id = %u group_id = %u name = %s ); callback = %p\n", node_id, group_id, name, user_func);
#endif
	if(name[0] == 0)
	{
		void (* alias_o_method_group_destroy)(void *user_data, VNodeID node_id, uint8 group_id);
		alias_o_method_group_destroy = v_fs_get_alias_user_func(41);
		if(alias_o_method_group_destroy != NULL)
			alias_o_method_group_destroy(v_fs_get_alias_user_data(41), node_id, group_id);
		return buffer_pos;
	}
	if(func_o_method_group_create != NULL)
		func_o_method_group_create(user_data, node_id, group_id, name);

	return buffer_pos;
}

void verse_send_o_method_group_subscribe(VNodeID node_id, uint16 group_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 42);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_method_group_subscribe(node_id = %u group_id = %u );\n", node_id, group_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], TRUE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_o_method_group_unsubscribe(VNodeID node_id, uint16 group_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_10);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 42);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_method_group_unsubscribe(node_id = %u group_id = %u );\n", node_id, group_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], FALSE);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_method_group_subscribe(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_method_group_subscribe)(void *user_data, VNodeID node_id, uint16 group_id);
	VNodeID node_id;
	uint16 group_id;
	char alias_bool;

	func_o_method_group_subscribe = user_func;
	if(buffer_length < 6)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &group_id);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_method_group_subscribe(node_id = %u group_id = %u ); callback = %p\n", node_id, group_id, user_func);
#endif
	if(buffer_length < buffer_pos + 1)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &alias_bool);
	if(!alias_bool)
	{
		void (* alias_o_method_group_unsubscribe)(void *user_data, VNodeID node_id, uint16 group_id);
		alias_o_method_group_unsubscribe = v_fs_get_alias_user_func(42);
		if(alias_o_method_group_unsubscribe != NULL)
			alias_o_method_group_unsubscribe(v_fs_get_alias_user_data(42), node_id, group_id);
		return buffer_pos;
	}
	if(func_o_method_group_subscribe != NULL)
		func_o_method_group_subscribe(user_data, node_id, group_id);

	return buffer_pos;
}

void verse_send_o_method_create(VNodeID node_id, uint16 group_id, uint16 method_id, const char *name, uint8 param_count, VNOParamType *param_types, char * *param_names)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 43);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_method_create(node_id = %u group_id = %u method_id = %u name = %s param_count = %u param_types = %p param_names = %p );\n", node_id, group_id, method_id, name, param_count, param_types, param_names);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], method_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], param_count);
	{
		unsigned int i, j, sum = 1;
		for(i = 0; i < param_count; i++)
		{
			sum += 3;
			for(j = 0; param_names[i][j] != 0; j++);
		}
		if(sum + buffer_pos > 1500)
			return;
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], param_count);
		for(i = 0; i < param_count; i++)
		{
			buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], param_types[i]);
			buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], param_names[i], 1500 - buffer_pos);
		}
	}
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

void verse_send_o_method_destroy(VNodeID node_id, uint16 group_id, uint16 method_id)
{
	uint8 *buf;
	unsigned int buffer_pos = 0, address_size = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 43);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_method_destroy(node_id = %u group_id = %u method_id = %u );\n", node_id, group_id, method_id);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], method_id);
	address_size = buffer_pos;
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], NULL, 16);
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], -1);
	v_cmd_buf_set_address_size(head, address_size, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_method_create(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_method_create)(void *user_data, VNodeID node_id, uint16 group_id, uint16 method_id, const char *name, uint8 param_count, VNOParamType *param_types, char * *param_names);
	VNodeID node_id;
	uint16 group_id;
	uint16 method_id;
	char name[16];
	uint8 param_count;
	VNOParamType *param_types;
	char * *param_names;
	
	func_o_method_create = user_func;
	if(buffer_length < 8)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &group_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &method_id);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 16, buffer_length - buffer_pos);
	if(buffer_length < 1 + buffer_pos)
		return -1;
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &param_count);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_method_create(node_id = %u group_id = %u method_id = %u name = %s param_count = %u ); callback = %p\n", node_id, group_id, method_id, name, param_count, user_func);
#endif
	if(param_count != 255)
	{
		unsigned int i, size, text = 0;
		VNOParamType types[256];
		uint8 t;
		char name_buf[1500], *names[256];
		for(i = 0; i < param_count; i++)
		{
			buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &t);
			types[i] = t;
			names[i] = &name_buf[text];
			size = vnp_raw_unpack_string(&buf[buffer_pos], names[i], 1500 - buffer_pos, buffer_length - buffer_pos);
			buffer_pos += size;
			text += size;
		}
		if(func_o_method_create != NULL)
			func_o_method_create(user_data, node_id, group_id, method_id, name, param_count, types, names);
		return buffer_pos;
	}

	if(name[0] == 0)
	{
		void (* alias_o_method_destroy)(void *user_data, VNodeID node_id, uint16 group_id, uint16 method_id);
		alias_o_method_destroy = v_fs_get_alias_user_func(43);
		if(alias_o_method_destroy != NULL)
			alias_o_method_destroy(v_fs_get_alias_user_data(43), node_id, group_id, method_id);
		return buffer_pos;
	}
	if(func_o_method_create != NULL)
		func_o_method_create(user_data, node_id, group_id, method_id, name, param_count, param_types, param_names);

	return buffer_pos;
}

void verse_send_o_method_send(VNodeID node_id, uint16 group_id, uint16 method_id, VNodeID sender, void *params)
{
	uint8 *buf;
	unsigned int buffer_pos = 0;
	VCMDBufHead *head;
	head = v_cmd_buf_allocate(VCMDBS_1500);/* Allocating the buffer */
	buf = ((VCMDBuffer10 *)head)->buf;

	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 44);/* Packing the command */
#if defined V_PRINT_SEND_COMMANDS
	printf("send: o_method_send(node_id = %u group_id = %u method_id = %u sender = %u params = %p );\n", node_id, group_id, method_id, sender, params);
#endif
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], node_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], group_id);
	buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], method_id);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], sender);
	{
		unsigned int i;
		uint16 size;
		vnp_raw_unpack_uint16(&params, &size);
		for(i = 0; i < size; i++)
			buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8 *)params)[i]);
		free(params);
	}
	v_cmd_buf_set_unique_size(head, buffer_pos);
	v_nq_send_buf(v_con_get_network_queue(), head);
}

unsigned int v_unpack_o_method_send(const char *buf, size_t buffer_length, void *user_func, void *user_data)
{
	unsigned int buffer_pos = 0;
	void (* func_o_method_send)(void *user_data, VNodeID node_id, uint16 group_id, uint16 method_id, VNodeID sender, void *params);
	VNodeID node_id;
	uint16 group_id;
	uint16 method_id;
	VNodeID sender;
	void *params;
	
	func_o_method_send = user_func;
	if(buffer_length < 12)
		return -1;
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &node_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &group_id);
	buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &method_id);
	buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &sender);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: o_method_send(node_id = %u group_id = %u method_id = %u sender = %u ); callback = %p\n", node_id, group_id, method_id, sender, user_func);
#endif
	{
		unsigned int i;
		uint8 par[1500];
		uint16 size;
		vnp_raw_unpack_uint16(&buf[buffer_pos], &size);
		for(i = 0; i < size; i++)
			buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &par[i]);
		if(func_o_method_send != NULL)
			func_o_method_send(user_data, node_id, group_id, method_id, sender, par);
		return buffer_pos;
	}

	if(func_o_method_send != NULL)
		func_o_method_send(user_data, node_id, group_id, method_id, sender, params);

	return buffer_pos;
}

#endif

