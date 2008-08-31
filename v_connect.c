/*
 * $Id$ 
 *
 * ***** BEGIN BSD LICENSE BLOCK *****
 *
 * Copyright (c) 2005-2008, The Uni-Verse Consortium.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***** END BSD LICENSE BLOCK *****
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "v_cmd_buf.h"
#include "v_network_out_que.h"
#include "v_network.h"
#include "v_connection.h"
#include "v_util.h"

extern void verse_send_packet_ack(uint32 packet_id);

/*  Stage 0: Clinets inital call to connect to host */
static void v_send_hidden_connect_contact(void)
{
	uint8 buf[1500];
	unsigned int i, buffer_pos = 0;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 0); /* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0); /* Packing the command */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], V_CS_IDLE); /* Stage 0 */

	v_n_send_data(v_con_get_network_address(), buf, buffer_pos);
}

/*  Stage 1: Hosts reply to any atempt to connect */
static void v_send_hidden_connect_send_key(void)
{
	uint8 buf[1500];
	unsigned int i, buffer_pos = 0, s, f;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 0); /* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0); /* Packing the command */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], V_CS_CONTACT); /* Packing Stage 1 */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], V_RELEASE_NUMBER); /* Packing version number */
	v_n_get_current_time(&s, &f);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], s); /* Packing time (seconds) */
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], f); /* Packing time (fraction) */
	
	v_n_send_data(v_con_get_network_address(), buf, buffer_pos);
}

/* Stage 2: clients sends name and password */
static void v_send_hidden_connect_login(void)
{
	uint8		buf[1500];
	const char	*name, *pass;
	unsigned int	buffer_pos = 0, i;

	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 1); /* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0); /* Packing the command */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], V_CS_CONTACTED); /* Packing Stage 2 */
	name = v_con_get_name();
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, 32); /* Packing name */
	pass = v_con_get_pass();
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], pass, 32); /* Packing password */

	v_n_send_data(v_con_get_network_address(), buf, buffer_pos);
}

/* Host accepts Clients connectionatempt */
static void v_send_hidden_connect_accept(void)
{
	uint8 buf[1500];
	unsigned int buffer_pos = 0;

	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 1); /* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 1); /* Packing the command */
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], verse_session_get_avatar()); /* Packing avatar ID */
	
	v_n_send_data(v_con_get_network_address(), buf, buffer_pos);
}

/* Host send bye bye command */
static void v_send_hidden_connect_terminate(VNetworkAddress *address, unsigned int packet_id, const char *bye)
{
	uint8 buf[1500];
	unsigned int buffer_pos = 0;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], packet_id); /* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 2); /* Packing the command */
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], bye, 512); /* Packing bye bye message */

	v_n_send_data(address, buf, buffer_pos);
}

VSession verse_send_connect(const char *name, const char *pass, const char *address)
{
	VNetworkAddress a; 
	VSession *session;

	if(v_n_set_network_address(&a, address))
	{
#if defined(V_PRINT_SEND_COMMANDS)
		char ip_string[32];
#endif
		session = v_con_connect(&a, V_CS_CONTACT);
#if defined(V_PRINT_SEND_COMMANDS)
		v_n_get_address_string(&a, ip_string);
		printf("send: %p = verse_send_connect(name = %s, pass = %s, address = %s (%s)\n",
				session, name, pass, address, ip_string);
#endif
		v_con_set_name_pass(name, pass);
		v_send_hidden_connect_contact();
		v_con_inqueue_timer_update();	/* Reset timer in connection's in queue, above takes a while. */
		return session;
	}
	else
	{
#if defined(V_PRINT_SEND_COMMANDS)
		printf("send: NULL = verse_send_connect(name = %s, pass = %s, address = %s (Unresolved DNS));\n",
				name, pass, address);
#endif
		return NULL;
	}
}

/* Connection handshake, sending connection commands */
void v_update_connection_pending(boolean resend)
{
	VSession (* func_connect)(void *user_data, const char *name, const char *pass, const char *address) = NULL;
	VSession (* func_connect_accept)(void *user_data, VNodeID avatar, char *address);
	void (* func_connect_terminate)(void *user_data, char *address, const char *bye);
	char address_string[32];

	switch(v_con_get_connect_stage())
	{
	case V_CS_CONTACT :		/* client tries to contact host */
		if(resend)
			v_send_hidden_connect_contact();
		break;
	case V_CS_CONTACTED :		/* Host replies with challange */
		if(resend)
			v_send_hidden_connect_send_key();
		break;
	case V_CS_PENDING_ACCEPT :	/* Client sends login */
		if(resend)
			v_send_hidden_connect_login();
		break;
	case V_CS_PENDING_HOST_CALLBACK : /* Host got login waits for accept connect callback */
		v_con_set_connect_stage(V_CS_PENDING_DECISION);
		func_connect = v_fs_get_user_func(0);
		v_n_get_address_string(v_con_get_network_address(), address_string);
#if defined(V_PRINT_RECEIVE_COMMANDS)
		printf("receive: verse_send_connect(address = %s, name = %s, pass = %s); callback = %p\n",
				address_string, v_con_get_name(), v_con_get_pass(), func_connect);
#endif
		if(func_connect != 0)
			func_connect(v_fs_get_user_data(0), v_con_get_name(), v_con_get_pass(), address_string);
		break;
	case V_CS_PENDING_CLIENT_CALLBACK_ACCEPT : /* Host got login waits for accept connect callback */
		v_con_set_connect_stage(V_CS_CONNECTED);
		func_connect_accept = v_fs_get_user_func(1);
		v_n_get_address_string(v_con_get_network_address(), address_string);
#if defined(V_PRINT_RECEIVE_COMMANDS)
		printf("receive: func_connect_accept(avatar = %u, address = %s, name = %s, pass = %s); callback = %p\n",
		       verse_session_get_avatar(), address_string, v_con_get_name(), v_con_get_pass(), func_connect);
#endif
		if(func_connect_accept != 0)
			func_connect_accept(v_fs_get_user_data(1), verse_session_get_avatar(), address_string);
		break;
	case V_CS_PENDING_CLIENT_CALLBACK_TERMINATE : /* Host got login waits for accept connect callback */
		v_con_set_connect_stage(V_CS_CONNECTED);
		func_connect_terminate = v_fs_get_user_func(2);
		v_n_get_address_string(v_con_get_network_address(), address_string);
#if defined(V_PRINT_RECEIVE_COMMANDS)
		printf("receive: func_connect_terminate(address = %s, bye = %s); callback = %p\n",
				address_string, "no message", func_connect);
#endif
		if(func_connect_terminate != 0)
			func_connect_terminate(v_fs_get_user_data(2), address_string, "no message");
		break;
	default:
		;
	}	
}

/* Connection Handshake, unpacking all of connect command */
void v_unpack_connection(const char *buf, unsigned int buffer_length)
{
	unsigned int buffer_pos, pack_id;
	uint32 seconds, fractions;
	uint8 stage, cmd_id, version;

	/* WHY??!! */
	if(buffer_length < 5)
		return;
	
	buffer_pos = vnp_raw_unpack_uint32(buf, &pack_id); /* Unpacking packet ID */
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &cmd_id); /* Unpacking command ID */

	if(cmd_id == 0)
	{
		buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &stage); /* Unpacking stage */

		printf(" Handling connection, stage %u\n", stage);

		if(stage == V_CS_IDLE && V_CS_IDLE == v_con_get_connect_stage()) /* Reseved by host */
		{
			verse_send_packet_ack(pack_id); /* Sending ACK command */
			v_con_set_connect_stage(V_CS_CONTACTED); /* Set stage */
			v_send_hidden_connect_send_key(); /* TODO: Rename ... we don't send any key anymore */

			return; 
		}
		if(stage == V_CS_CONTACT && V_CS_CONTACT == v_con_get_connect_stage())
		{
			verse_send_packet_ack(pack_id); /* Sending ACK command */
			buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &version); /* Unpacking version of protocol */

			if(version != V_RELEASE_NUMBER)
			{
				fprintf(stderr, "Versions of client and server aren't the same\n");
				return;
			}

			buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &seconds); /* Unpacking time (seconds) */
			buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &fractions); /* Unpacking time (fractions) */
			v_con_set_time(seconds, fractions); /* Set time */
			v_con_set_connect_stage(V_CS_PENDING_ACCEPT); /* Set stage */
			v_send_hidden_connect_login(); /* Sending CONNECT command */
			return; 
		}
		if(stage == V_CS_CONTACTED && V_CS_CONTACTED == v_con_get_connect_stage()) /* Reseved by host */
		{
			char name[32], pass[32]; 

			verse_send_packet_ack(pack_id); /* Sending ACK command */
			buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 32, 32); /* Unpacking name */
			buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], pass, 32, 32); /* Unpacking password */
			v_con_set_name_pass(name, pass); /* Set name and pass */
			v_con_set_connect_stage(V_CS_PENDING_HOST_CALLBACK); /* Set stage */
			return; 
		}
	}
	if(cmd_id == 1 && V_CS_PENDING_ACCEPT == v_con_get_connect_stage()) /* Reseved by client */
	{
		uint32 avatar;

		verse_send_packet_ack(pack_id); /* Sending ACK command */
		buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &avatar); /* Unpacking avatar ID */
		v_con_set_avatar(avatar); /* Set avatar ID*/
		v_con_set_connect_stage(V_CS_PENDING_CLIENT_CALLBACK_ACCEPT); /* Set stage */
		return; 
	}
	if(cmd_id == 2 && V_CS_PENDING_ACCEPT == v_con_get_connect_stage()) /* reseved by client */
	{
		verse_send_packet_ack(pack_id);	
		/* TODO: do somethink with this string */
		/* buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 512, buffer_length - buffer_pos); */
		v_con_set_connect_stage(V_CS_PENDING_CLIENT_CALLBACK_TERMINATE);
		return; 
	}
}

VSession verse_send_connect_accept(VNodeID avatar, const char *address)
{
	VNetworkAddress a;
#if defined(V_PRINT_SEND_COMMANDS)
	printf("send: verse_send_connect_accept(avatar = %u, address = %s);\n", avatar, address);
#endif

	if(!v_n_set_network_address(&a, address))
		return NULL;
	if(v_co_switch_connection(&a))
	{
		if(v_con_get_connect_stage() != V_CS_PENDING_DECISION)
			return NULL;
		v_con_set_avatar(avatar);
		v_con_set_connect_stage(V_CS_CONNECTED);
		v_send_hidden_connect_accept();
		return v_con_get_network_queue();
	}
	return NULL;
}

void v_callback_connect_terminate(const char *bye)
{
	void (* func_connect_terminate)(void *user_data, char *address, const char *bye);
	char address_string[32];

	printf("terminate (%s)\n", bye);
	func_connect_terminate = v_fs_get_user_func(2);
	v_n_get_address_string(v_con_get_network_address(), address_string);
#if defined(V_PRINT_RECEIVE_COMMANDS)
	printf("receive: verse_send_connect_terminate(address = %s, bye = %s); callback = %p\n",
			address_string, bye, func_connect_terminate);
#endif
	if(func_connect_terminate != 0)
		func_connect_terminate(v_fs_get_user_data(2), address_string, bye);
}

void verse_send_connect_terminate(const char *address, const char *bye)
{
	VNetworkAddress a;
#if defined(V_PRINT_RECEIVE_COMMANDS)
	printf("send: verse_send_connect_terminate(address = %s, bye = %s);\n",
			address, bye);
#endif

	if(address == NULL)
		v_send_hidden_connect_terminate(v_con_get_network_address(), v_noq_get_next_out_packet_id(v_con_get_network_queue()), bye);
	else if(!v_n_set_network_address(&a, address))
		return;
	else if(v_co_switch_connection(&a))
		v_send_hidden_connect_terminate(v_con_get_network_address(), v_noq_get_next_out_packet_id(v_con_get_network_queue()), bye);

	if(v_con_get_connect_stage() != V_CS_PENDING_DECISION)
		verse_session_destroy(v_con_get_network_queue());
}

/* Evil PING stuff */

void verse_send_ping(const char *address, const char *message)
{
	VNetworkAddress a;
	if(v_n_set_network_address(&a, address))
	{
		unsigned int buffer_pos = 0;
		uint8 buf[1500];
		buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 0);/* Packing the Packet id */
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 5);/* Packing the command */

#if defined V_PRINT_SEND_COMMANDS
		printf("send: verse_send_ping(address = %s text = %s);\n",
				address, message);
#endif
		buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], message, 1400);
		v_n_send_data(&a, buf, buffer_pos);
	}
#if defined V_PRINT_SEND_COMMANDS
	else
		printf("send: verse_send_ping(address = %s (FAULTY) message = %s);\n", address, message);
#endif
}

unsigned int v_unpack_ping(const char *buf, size_t buffer_length)
{
	unsigned int buffer_pos = 0;
	void (* func_ping)(void *user_data, const char *address, const char *text);
	char address[64];
	char message[1400];

	func_ping = v_fs_get_user_func(5);
	v_n_get_address_string(v_con_get_network_address(), address);
	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], message, 1400, buffer_length - buffer_pos);
#if defined V_PRINT_RECEIVE_COMMANDS
	printf("receive: verse_send_ping(address = %s message = %s ); callback = %p\n", address, message, v_fs_get_user_func(5));
#endif
	if(func_ping != NULL)
		func_ping(v_fs_get_user_data(5), address, message);
	return buffer_pos;
}

typedef struct {
	char	message[1400];
	void	*next;
	VNetworkAddress address;
} VPingCommand;

static VPingCommand *v_ping_commands = NULL;

boolean v_connect_unpack_ping(const char *buf, size_t buffer_length, VNetworkAddress *address)
{
	if(buffer_length > 5)
	{
		unsigned int buffer_pos = 0;
		uint8 cmd_id;
		uint32 pack_id;

		buffer_pos = vnp_raw_unpack_uint32(&buf[buffer_pos], &pack_id);
		buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &cmd_id);
		if(cmd_id == 5)
		{
			if(NULL != v_fs_get_user_func(5))
			{
				VPingCommand *pc;

				pc = malloc(sizeof *pc);
				memcpy(&pc->address, address, sizeof(VNetworkAddress));
				vnp_raw_unpack_string(&buf[buffer_pos], pc->message,
						      sizeof pc->message, buffer_length - buffer_pos);
				pc->next = v_ping_commands;
				v_ping_commands = pc;
			}
			return TRUE;
		}
	}
	return FALSE;
}

void v_ping_update(void)
{
	VPingCommand *cp;
	void (* func_ping)(void *user_data, const char *address, const char *text);
	char address[64];
	func_ping = v_fs_get_user_func(5);

	while(v_ping_commands != NULL)
	{
		cp = v_ping_commands->next;
		v_n_get_address_string(&v_ping_commands->address, address);
#if defined V_PRINT_RECEIVE_COMMANDS
		printf("receive: verse_send_ping(address = %s message = %s ); callback = %p\n", address, v_ping_commands->message, v_fs_get_user_func(5));
#endif
		if(func_ping != NULL)
			func_ping(v_fs_get_user_data(5), address, v_ping_commands->message);
		free(v_ping_commands);
		v_ping_commands = cp;
	}
}

#endif
