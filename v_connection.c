/*
**
*/

#include <stdio.h>
#include <stdlib.h>

#include "v_cmd_buf.h"
#include "v_network_que.h"
#include "v_network.h"
#include "v_cmd_gen.h"
#include "v_connection.h"

#if !defined(V_GENERATE_FUNC_MODE)
#include "verse.h"

#define CONNECTION_CHUNK_SIZE	16
#define PING_TIMEOUT		200000		/* 200000 microseconds. */
#define CONNECTION_TIMEOUT	50		/* 200 * 50 miliseconds. */

typedef struct{
	void			*network_queue;
	void			*network_address;
	boolean			connected;
	unsigned int	avatar;
	unsigned int	packet_id;
	unsigned int	ping_timeout;
	unsigned int	connect_timeout;
}VConection;

struct{
	VConection		*con;
	unsigned int	con_count;
	unsigned int	current_connection;
	void			*connect_address;
	void			*unified_func_storage;
	uint16			connect_port;
}VConData;

boolean v_con_initialized = FALSE;

void v_con_init(void)
{
	if(v_con_initialized == TRUE)
		return;
	VConData.con = malloc((sizeof *VConData.con) * CONNECTION_CHUNK_SIZE);
	VConData.con_count = 0;
	VConData.connect_address = NULL;
	VConData.unified_func_storage = NULL;
	VConData.connect_port = VERSE_STD_CONNECT_PORT;
	v_con_initialized = TRUE;
}

void verse_set_connect_port(uint16 port)
{
	v_con_init();
	VConData.connect_port = port;
}

extern void *v_fs_create_func_storage(void);

void *v_con_connect(char *address)
{
	void *net;
	v_con_init();
	net = v_n_create_network_address(0, address);
	if(net == NULL)
		return NULL;
	if((VConData.con_count - 1) % CONNECTION_CHUNK_SIZE == 0)
		VConData.con = realloc(VConData.con, (sizeof *VConData.con) * (VConData.con_count + CONNECTION_CHUNK_SIZE));
	VConData.con[VConData.con_count].network_queue = v_nq_create_network_queue();
	VConData.con[VConData.con_count].network_address = net;
	VConData.con[VConData.con_count].connected = FALSE;
	VConData.con[VConData.con_count].avatar = 0;
	VConData.con[VConData.con_count].packet_id = 0;
	VConData.con[VConData.con_count].ping_timeout = 0;
	VConData.con[VConData.con_count].connect_timeout = 0;
	VConData.current_connection = VConData.con_count;
	++VConData.con_count;
	return net;
}

void verse_session_destroy(VSession session)
{
	unsigned int i;
	for(i = 0; i < VConData.con_count && VConData.con[i].network_queue != session; i++);
	if(i < VConData.con_count)
	{
		v_nq_destroy_network_queue(VConData.con[VConData.con_count].network_queue);
		v_n_destroy_network_address(VConData.con[VConData.con_count].network_address);
		VConData.con[i] = VConData.con[--VConData.con_count];
	}
}

void verse_session_set(void *session)
{
	unsigned int i;
	for(i = 0; i < VConData.con_count && session != VConData.con[i].network_address; i++);
	if(i < VConData.con_count)
		VConData.current_connection = i;
}

void *v_con_get_network_queue()
{
	return VConData.con[VConData.current_connection].network_queue;
}

void *v_con_get_network_address()
{
	return VConData.con[VConData.current_connection].network_address;
}

unsigned int *v_con_get_network_expected_packet()
{
	return &VConData.con[VConData.current_connection].packet_id;
}

uint32 verse_session_get_avatar(void)
{
	return VConData.con[VConData.current_connection].avatar;
}

#define V_MAX_CONNCECT_PACKET_SIZE 512

void *v_con_get_network_address_id(unsigned int id)
{
	if(id < VConData.con_count)
		return VConData.con[VConData.current_connection].network_address;
	else
		return VConData.connect_address;
}

unsigned int v_con_get_network_address_count(void)
{
	if(v_fs_func_accept_connections())
		return VConData.con_count + 1;
	else
		return VConData.con_count;

}

void v_con_network_listen()
{
	void *address;
	char buf[V_MAX_CONNCECT_PACKET_SIZE];
	unsigned int size;
	/* no mid send new work looking for now */
	if(VConData.con_count > 0)
	{
		address = v_con_get_network_address();
		if(address != NULL)
		{
			size = v_n_receive_data(address, buf, V_MAX_CONNCECT_PACKET_SIZE, TRUE);
			while(size != -1 && size != 0)
			{
				v_fs_buf_store_pack(buf, size);
				size = v_n_receive_data(address, buf, V_MAX_CONNCECT_PACKET_SIZE, TRUE);
			}
		}
	}
}

extern void verse_send_packet_nak(uint32 packet_id);

void verse_callback_update(unsigned int milliseconds)
{
	void *address;
	char buf[V_MAX_CONNCECT_PACKET_SIZE];
	unsigned int size;
	v_con_init();
	if(v_fs_func_accept_connections())
	{
		if(VConData.connect_address == NULL)
			VConData.connect_address = v_n_create_network_address(VERSE_STD_CONNECT_PORT/*v_connect_port*/, "localhost");
		v_n_wait_for_incoming(milliseconds);
		size = v_n_receive_data(VConData.connect_address, buf, V_MAX_CONNCECT_PACKET_SIZE, FALSE);
		if(size != -1)
		{
			char string[200];
			v_n_get_address_string(VConData.connect_address, string);
			v_fs_connect_unpack(buf, size, string);
		}
	}

	if(VConData.con_count > 0)
	{
		address = v_con_get_network_address();
		if(address != NULL)
		{
			if(!v_fs_buf_unpack_stored() || !v_fs_func_accept_connections())
				v_n_wait_for_incoming(milliseconds);
			size = v_n_receive_data(address, buf, V_MAX_CONNCECT_PACKET_SIZE, TRUE);
			if(size != -1 || size != 0)
				VConData.con[VConData.current_connection].ping_timeout += milliseconds;
			while(size != -1 && size != 0)
			{
				VConData.con[VConData.current_connection].ping_timeout = 0;
				VConData.con[VConData.current_connection].connect_timeout = 0;
				v_fs_buf_unpack(buf, size);
				size = v_n_receive_data(address, buf, V_MAX_CONNCECT_PACKET_SIZE, TRUE);
			}
			if(VConData.con[VConData.current_connection].ping_timeout > PING_TIMEOUT)
			{
				verse_send_packet_nak(VConData.con[VConData.current_connection].packet_id++);
				VConData.con[VConData.current_connection].ping_timeout = 0;
				VConData.con[VConData.current_connection].connect_timeout++;
			}
			v_nq_send_queue(v_con_get_network_queue(), address);
		}
	}
	if(VConData.con_count > 0 && VConData.con[VConData.current_connection].connected && VConData.con[VConData.current_connection].connect_timeout >= CONNECTION_TIMEOUT)
	{
		void (*callback_connect_terminate_func)(void *user, char *bye);
		callback_connect_terminate_func = v_fs_get_user_func(3);
		#if defined(V_PRINT_RECIVE_COMMANDS)
		printf("receive: verse_send_connect_terminate(connection timed out); callback = %p\n", callback_connect_terminate_func);
		#endif
		printf("receive: verse_send_connect_terminate(connection timed out); callback = %p\n", callback_connect_terminate_func);
		if(callback_connect_terminate_func != NULL)
			callback_connect_terminate_func(v_fs_get_user_data(3), "connection timed out");
		v_nq_destroy_network_queue(VConData.con[VConData.current_connection].network_queue);
		v_n_destroy_network_address(VConData.con[VConData.current_connection].network_address);
		VConData.con_count--;		
		VConData.con[VConData.current_connection].network_queue = VConData.con[VConData.con_count].network_queue;
		VConData.con[VConData.current_connection].network_address = VConData.con[VConData.con_count].network_address;
		VConData.con[VConData.current_connection].connected = VConData.con[VConData.con_count].connected;
		VConData.con[VConData.current_connection].avatar = VConData.con[VConData.con_count].avatar;
		VConData.con[VConData.current_connection].packet_id = VConData.con[VConData.con_count].packet_id;
		VConData.con[VConData.current_connection].ping_timeout = VConData.con[VConData.con_count].ping_timeout;
		VConData.con[VConData.current_connection].connect_timeout = VConData.con[VConData.con_count].connect_timeout;
		VConData.con[VConData.con_count].network_queue = NULL;
		VConData.con[VConData.con_count].network_address = NULL;
		VConData.con[VConData.con_count].connected = FALSE;
		VConData.con[VConData.con_count].avatar = 0;
		VConData.con[VConData.con_count].packet_id = 0;
		VConData.con[VConData.con_count].ping_timeout = 0;
		VConData.con[VConData.con_count].connect_timeout = 0;
		VConData.current_connection = 0;
	}
}

#endif
