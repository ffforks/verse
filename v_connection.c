/*
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v_cmd_buf.h"
#include "v_network_in_que.h"
#include "v_network_out_que.h"
#include "v_cmd_gen.h"
#include "v_connection.h"
#include "v_encryption.h"

#if !defined(V_GENERATE_FUNC_MODE)
#include "verse.h"

#define CONNECTION_CHUNK_SIZE	16
#define PING_TIMEOUT		200000		/* 200000 microseconds. */
#define CONNECTION_TIMEOUT	50		/* 200 * 50 miliseconds. */

#define V_MAX_CONNECT_PACKET_SIZE	1500

typedef struct {
	VNetOutQueue	*out_queue;
	VNetInQueue		in_queue;
	VNetworkAddress	network_address;
	boolean			connected;
	unsigned int	avatar;
/*	unsigned int	packet_id;*/
	unsigned int	ping_timeout;
	unsigned int	connect_timeout;
	unsigned int	timedelta[2];
	boolean			destroy_flag;
	void			*ordered_storage;
	char			name[V_ENCRYPTION_LOGIN_KEY_SIZE / 2];
	char			pass[V_ENCRYPTION_LOGIN_KEY_SIZE / 2];
	VConnectStage	connect_stage;
	unsigned int	stage_atempts;
	uint8			key_my_public[V_ENCRYPTION_LOGIN_KEY_SIZE];
	uint8			key_my_private[V_ENCRYPTION_LOGIN_KEY_SIZE];
	uint8			key_my_n[V_ENCRYPTION_LOGIN_KEY_SIZE];
	uint8			key_other_public[V_ENCRYPTION_LOGIN_KEY_SIZE];
	uint8			key_other_n[V_ENCRYPTION_LOGIN_KEY_SIZE];
	uint8			key_data[V_ENCRYPTION_DATA_KEY_SIZE];
} VConnection;

static struct {
	VConnection		*con;
	unsigned int	con_count;
	unsigned int	current_connection;
	VNetworkAddress	*connect_address;
	void			*unified_func_storage;
	uint16			connect_port;
	unsigned int	pending_packets;
} VConData;

static boolean v_con_initialized = FALSE;

void v_con_init(void)
{
	if(v_con_initialized == TRUE)
		return;
	VConData.con = malloc((sizeof *VConData.con) * CONNECTION_CHUNK_SIZE);
	memset(VConData.con, 0, (sizeof *VConData.con) * CONNECTION_CHUNK_SIZE);	/* Clear the memory. */
	VConData.con_count = 0;
	v_con_initialized = TRUE;
	VConData.pending_packets = 0;
}

void verse_set_port(uint16 port)
{
	v_n_set_port(port);
}

extern void *v_fs_create_func_storage(void);
extern void *v_create_ordered_storage(void);
extern void v_destroy_ordered_storage(void *data);

void *v_con_connect(uint32 ip, uint16 port, VConnectStage stage)
{
	v_con_init();
	if((VConData.con_count - 1) % CONNECTION_CHUNK_SIZE == 0)
		VConData.con = realloc(VConData.con, (sizeof *VConData.con) * (VConData.con_count + CONNECTION_CHUNK_SIZE));
	VConData.con[VConData.con_count].out_queue = v_noq_create_network_queue();
	v_niq_clear(&VConData.con[VConData.con_count].in_queue);
	VConData.con[VConData.con_count].connected = FALSE;
	VConData.con[VConData.con_count].network_address.ip = ip;
	VConData.con[VConData.con_count].network_address.port = port;
	VConData.con[VConData.con_count].avatar = 0;
/*	VConData.con[VConData.con_count].packet_id = 2;*/
	VConData.con[VConData.con_count].destroy_flag = FALSE;

	VConData.con[VConData.con_count].ping_timeout = 0;
	VConData.con[VConData.con_count].connect_timeout = 0;
	VConData.con[VConData.con_count].ordered_storage = v_create_ordered_storage();
	VConData.con[VConData.con_count].name[0] = 0;
	VConData.con[VConData.con_count].pass[0] = 0;
	VConData.con[VConData.con_count].connect_stage = stage;
	VConData.con[VConData.con_count].stage_atempts = 0;
	VConData.con[VConData.con_count].timedelta[0] = 0;
	VConData.con[VConData.con_count].timedelta[1] = 0;
	VConData.current_connection = VConData.con_count;
	++VConData.con_count;
	return VConData.con[VConData.current_connection].out_queue;
}

void verse_session_destroy(VSession session)
{
	unsigned int i;
	for(i = 0; i < VConData.con_count && VConData.con[i].out_queue != session; i++);
	if(i < VConData.con_count)
	{
		VConData.con[i].destroy_flag = TRUE;
	}
}

void verse_session_set(void *session)
{
	unsigned int i;
	for(i = 0; i < VConData.con_count && session != VConData.con[i].out_queue; i++);
	if(i < VConData.con_count)
		VConData.current_connection = i;
}


uint32 v_co_find_connection(uint32 ip, uint16 port)
{
	unsigned int i;
	for(i = 0; i < VConData.con_count; i++)
		if(ip == VConData.con[i].network_address.ip && port == VConData.con[i].network_address.port)
			return i;
	return -1;
}

boolean v_co_switch_connection(uint32 ip, uint16 port)
{
	unsigned int i;
	for(i = 0; i < VConData.con_count; i++)
	{
		if(ip == VConData.con[i].network_address.ip && port == VConData.con[i].network_address.port)
		{
			VConData.current_connection = i;
			return TRUE;
		}
	}
	return FALSE;
}

/*
extern void	v_fs_buf_unpack(const uint8 *data, unsigned int length);
extern void	v_fs_buf_store_pack(uint8 *data, unsigned int length);
extern boolean	v_fs_buf_unpack_stored(void);
*/
extern void v_unpack_connection(const char *buf, unsigned int buffer_length);

void v_con_network_listen(void)
{
	VNetworkAddress address;
	uint8 buf[V_MAX_CONNECT_PACKET_SIZE], *store;
	int size = 0;
	unsigned int connection;
	uint32 packet_id;

	v_con_init();
	connection = VConData.current_connection;
	size = v_n_receive_data(&address, buf, sizeof buf);
	while(size != -1 && size != 0)
	{
		VConData.current_connection = v_co_find_connection(address.ip, address.port);
		if(VConData.current_connection < VConData.con_count)
		{			
			if(VConData.con[VConData.current_connection].connect_stage == V_CS_CONNECTED)
			{
				vnp_raw_unpack_uint32(buf, &packet_id);
				store = v_niq_store(&VConData.con[VConData.current_connection].in_queue, size, packet_id);
				if(store != NULL)
				{
					VConData.pending_packets++;
					v_e_dencrypt_data_packet(buf, store, size, VConData.con[VConData.current_connection].key_data);
				}
			}
			else
			{
				v_unpack_connection(buf, size);
			}
		}
		else if(v_fs_func_accept_connections())
		{
			v_con_connect(address.ip, address.port, V_CS_IDLE);
			v_unpack_connection(buf, size);
		}
		size = v_n_receive_data(&address, buf, sizeof buf);
	}
	VConData.current_connection = connection;
}

void v_update_connection_pending(void);

boolean v_con_callback_update(void)
{
	boolean output = FALSE;
	int size, connection;
	VNetInPacked *p;

	connection = VConData.current_connection;
	for(VConData.current_connection = 0; VConData.current_connection < VConData.con_count; VConData.current_connection++)
		if(VConData.con[VConData.current_connection].connect_stage != V_CS_CONNECTED)
			v_update_connection_pending();
	VConData.current_connection = connection;
/*	if(VConData.pending_packets == 0)
		return FALSE;
*/	if(VConData.con[VConData.current_connection].connect_stage == V_CS_CONNECTED)
	{
		p = v_niq_get(&VConData.con[VConData.current_connection].in_queue, &size);
		while(size != 0 && size != -1)
		{
			VConData.pending_packets--;
			v_fs_unpack(p->data, size);
			v_niq_release(&VConData.con[VConData.current_connection].in_queue, p);
			p = v_niq_get(&VConData.con[VConData.current_connection].in_queue, &size);
			output = TRUE;
		}
		v_con_network_listen();
	}

	return output;
}

extern void verse_send_packet_nak(uint32 packet_id);

void v_callback_connect_terminate(const char *bye);
#define V_CON_MAX_MICROSECOND_BETWEEN_SENDS 1000

void verse_callback_update(unsigned int microseconds)
{
	unsigned int connection, passed;
	connection = VConData.current_connection;
	for(VConData.current_connection = 0; VConData.current_connection < VConData.con_count; VConData.current_connection++)
	{
		v_noq_send_queue(VConData.con[VConData.current_connection].out_queue, &VConData.con[VConData.current_connection].network_address);
		if(VConData.con[VConData.current_connection].destroy_flag == TRUE)
		{
			v_noq_destroy_network_queue(VConData.con[VConData.current_connection].out_queue);
			v_destroy_ordered_storage(VConData.con[VConData.current_connection].ordered_storage);
			VConData.con[VConData.current_connection] = VConData.con[--VConData.con_count];
			if(connection < VConData.con_count)
			{
				VConData.current_connection = 0;
			}
			return;
		}
	}

	VConData.current_connection = connection;

	if(VConData.con_count > 0)
		if(v_niq_time_out(&VConData.con[VConData.current_connection].in_queue))
			v_callback_connect_terminate("connection timed out");

	v_con_network_listen();
	if(VConData.con_count > 0)
		if(v_con_callback_update())
			return;
	for(passed = 0; passed <= microseconds && VConData.pending_packets == 0; passed += V_CON_MAX_MICROSECOND_BETWEEN_SENDS)
	{
		if(V_CON_MAX_MICROSECOND_BETWEEN_SENDS < microseconds - passed)
			v_n_wait_for_incoming(V_CON_MAX_MICROSECOND_BETWEEN_SENDS);
		else
			v_n_wait_for_incoming(microseconds - passed);
		v_con_network_listen();
		connection = VConData.current_connection;
		for(VConData.current_connection = 0; VConData.current_connection < VConData.con_count; VConData.current_connection++)
			v_noq_send_queue(VConData.con[VConData.current_connection].out_queue, &VConData.con[VConData.current_connection].network_address);
		VConData.current_connection = connection;
	}
	if(VConData.con_count > 0)
		v_con_callback_update();
}

void verse_host_id_set(uint8 *id)
{
}

void v_con_set_name_pass(const char *name, const char *pass)
{
	unsigned int i;
	for(i = 0; i < 16 - 1 && name[i] != 0; i++)
		VConData.con[VConData.current_connection].name[i] = name[i];
	VConData.con[VConData.current_connection].name[i] = 0;
	for(i = 0; i < 16 - 1 && pass[i] != 0; i++)
		VConData.con[VConData.current_connection].pass[i] = pass[i];
	VConData.con[VConData.current_connection].pass[i] = 0;
}

char *v_con_get_name(void)
{
	return VConData.con[VConData.current_connection].name;
}

char *v_con_get_pass(void)
{
	return VConData.con[VConData.current_connection].pass;
}

void v_con_set_connect_stage(VConnectStage stage)
{
	VConData.con[VConData.current_connection].connect_stage = stage;
	VConData.con[VConData.current_connection].stage_atempts = 0;
}

VConnectStage v_con_get_connect_stage(void)
{
	return VConData.con[VConData.current_connection].connect_stage;
}

uint8 *v_con_get_my_public_key(void)
{
	return VConData.con[VConData.current_connection].key_my_public;
}

uint8 *v_con_get_my_private_key(void)
{
	return VConData.con[VConData.current_connection].key_my_private;
}

boolean v_con_set_other_public_key(uint8 *key)
{
	unsigned int i;
	boolean output = TRUE;
	for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE; i++)
	{
		if(VConData.con[VConData.current_connection].key_other_public[i] != key[i])
			output = FALSE;
		VConData.con[VConData.current_connection].key_other_public[i] = key[i];
	}
	return output;
}

uint8 * v_con_get_other_public_key(void)
{
	return VConData.con[VConData.current_connection].key_other_public;
}

void v_con_set_data_key(uint8 *key)
{
	unsigned int i;
	for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE; i++)
		VConData.con[VConData.current_connection].key_data[i] = key[i];
}

uint8 * v_con_get_data_key(void)
{
	return VConData.con[VConData.current_connection].key_data;
}

void * v_con_get_network_queue(void)
{
	return VConData.con[VConData.current_connection].out_queue;
}

VNetworkAddress * v_con_get_network_address(void)
{
	return &VConData.con[VConData.current_connection].network_address;
}

void * v_con_get_ordered_storage(void)
{
	return VConData.con[VConData.current_connection].ordered_storage;
}

void v_con_set_avatar(uint32 avatar)
{
	VConData.con[VConData.current_connection].avatar = avatar;
}

uint32 verse_session_get_avatar(void)
{
	return VConData.con[VConData.current_connection].avatar;
}

void verse_session_get_time(uint32 *seconds, uint32 *fractions)
{
	uint32 s, f;
	v_n_get_current_time(&s, &f);
	if(f + VConData.con[VConData.current_connection].timedelta[1] > f && f + VConData.con[VConData.current_connection].timedelta[1] > VConData.con[VConData.current_connection].timedelta[1])
		s++;
	if(seconds != NULL)
		*seconds = s + VConData.con[VConData.current_connection].timedelta[0];
	if(fractions != NULL)
		*fractions = f + VConData.con[VConData.current_connection].timedelta[1];
}

void v_con_set_time(uint32 seconds, uint32 fractions)
{
	uint32 s, f, tmp;
	v_n_get_current_time(&s, &f);

	tmp = fractions - f;
	if(tmp > fractions)
		s--;
	VConData.con[VConData.current_connection].timedelta[0] = seconds - s;
	VConData.con[VConData.current_connection].timedelta[1] = tmp;
}

#endif
