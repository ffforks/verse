#include <stdlib.h>
#include "verse_header.h"
#include "v_pack.h"
#include "v_cmd_gen.h"
#include "v_connection.h"
#if !defined(V_GENERATE_FUNC_MODE)
#include "verse.h"
#include "v_cmd_buf.h"
#include "v_network_que.h"

#define V_FS_MAX_CMDS 256

extern void init_pack_and_unpack_fucs(void);

struct{
	unsigned int	(*unpack_func[V_FS_MAX_CMDS])(char *data, unsigned int length, void *user_func, void *user_data);
	void			*pack_func[V_FS_MAX_CMDS];
	void			*user_func[V_FS_MAX_CMDS];
	void			*user_data[V_FS_MAX_CMDS];
	void			*alias_pack_func[V_FS_MAX_CMDS];
	void			*alias_user_func[V_FS_MAX_CMDS];
	void			*alias_user_data[V_FS_MAX_CMDS];
}VCmdData;

boolean v_fs_initialized = FALSE;

extern void verse_send_packet_ack(uint32 packet_id);
extern void callback_send_packet_ack(void *user, uint32 packet_id);
extern void verse_send_packet_nak(uint32 packet_id);
extern void callback_send_packet_nak(void *user, uint32 packet_id);

void v_fs_init(void)
{
	unsigned int i;
	if(v_fs_initialized)
		return;
	for(i = 0; i < V_FS_MAX_CMDS; i++)
	{
		VCmdData.unpack_func[i] = NULL;
		VCmdData.pack_func[i] = NULL;
		VCmdData.user_func[i] = NULL;
		VCmdData.user_data[i] = NULL;
		VCmdData.alias_pack_func[i] = NULL;
		VCmdData.alias_user_func[i] = NULL;
		VCmdData.alias_user_data[i] = NULL;
	}
	#if !defined(V_GENERATE_FUNC_MODE)
	init_pack_and_unpack_fucs();
	#endif
	for(i = 0; i < V_FS_MAX_CMDS && VCmdData.pack_func[i] != verse_send_packet_ack; i++);
	VCmdData.user_func[i] = callback_send_packet_ack;
	for(i = 0; i < V_FS_MAX_CMDS && VCmdData.pack_func[i] != verse_send_packet_nak; i++);
	VCmdData.user_func[i] = callback_send_packet_nak;

	v_fs_initialized = TRUE;
}


void v_fs_add_func(unsigned int cmd_id, unsigned int (*unpack_func)(char *data, unsigned int length, void *user_func, void *user_data), void *pack_func, void *alias_func)
{
	VCmdData.unpack_func[cmd_id] = unpack_func;
	VCmdData.pack_func[cmd_id] = pack_func;
	VCmdData.alias_pack_func[cmd_id] = alias_func;
}

void *v_fs_get_user_func(unsigned int cmd_id)
{
	return VCmdData.user_func[cmd_id];
}

void *v_fs_get_user_data(unsigned int cmd_id)
{
	return VCmdData.user_func[cmd_id];
}

void *v_fs_get_alias_user_func(unsigned int cmd_id)
{
	return VCmdData.alias_user_func[cmd_id];
}

void *v_fs_get_alias_user_data(unsigned int cmd_id)
{
	return VCmdData.alias_user_data[cmd_id];
}

void verse_callback_set(void *command, void *callback, void *user)
{
	unsigned int i;
	if(!v_fs_initialized)
		v_fs_init();

	for(i = 0; i < V_FS_MAX_CMDS; i++)
	{
		if(VCmdData.pack_func[i] == command)
		{
			VCmdData.user_data[i] = user;
			VCmdData.user_func[i] = callback;
			return;
		}
		if(VCmdData.alias_pack_func[i] == command)
		{
			VCmdData.alias_user_data[i] = user;
			VCmdData.alias_user_func[i] = callback;
			return;
		}
	}
}

boolean v_fs_func_acsept_connections()
{
/*	if(VCmdData.user_func[1] == NULL)
		return FALSE;
*/	return TRUE;
}


void v_fs_buf_unpack(uint8 *data, unsigned int length)
{
	uint32 i = 0, output, pack_id, *ecspected_paket;
	uint8 cmd_id;
	ecspected_paket = v_con_get_network_ecspected_paket();
	i = vnp_raw_unpack_uint32(&data[i], &pack_id); /* each pack starts whit a 32 bit id */
	if(pack_id < *ecspected_paket)
		return;
	for(; *ecspected_paket < pack_id; (*ecspected_paket)++)
		verse_send_packet_nak(*ecspected_paket);
	(*ecspected_paket)++;
	while(i < length)
	{
		i += vnp_raw_unpack_uint8(&data[i], &cmd_id);

		if(VCmdData.unpack_func[cmd_id] != NULL)
		{
			output = VCmdData.unpack_func[cmd_id](&data[i], length - i, VCmdData.user_func[cmd_id], VCmdData.user_data[cmd_id]);
			if(output == -1)
			{
				verse_send_packet_nak(pack_id);
				return;
			}
			i += output;
		}
	}
	verse_send_packet_ack(pack_id);
}

void v_fs_buf_store_pack(uint8 *data, unsigned int length)
{
	uint32 i = 0, output, pack_id, *ecspected_paket;
	uint8 cmd_id;
	ecspected_paket = v_con_get_network_ecspected_paket();
	i = vnp_raw_unpack_uint32(&data[i], &pack_id); /* each pack starts with a 32 bit id */
	if(pack_id < *ecspected_paket)
		return;
	for(; *ecspected_paket < pack_id; (*ecspected_paket)++)
		verse_send_packet_nak(*ecspected_paket);
	(*ecspected_paket)++;
	while(i < length)
	{
		i += vnp_raw_unpack_uint8(&data[i], &cmd_id);
		if(VCmdData.unpack_func[cmd_id] != NULL)
		{
			if(cmd_id == 7 || cmd_id == 8)
				output = VCmdData.unpack_func[cmd_id](&data[i], length - i, VCmdData.user_func[cmd_id], VCmdData.user_data[cmd_id]);
			else
				output = VCmdData.unpack_func[cmd_id](&data[i], length - i, NULL, NULL);
			if(output == -1)
			{
				verse_send_packet_nak(pack_id);
				return;
			}
			i += output;
		}
	}
	v_nq_store_packed(v_con_get_network_queue(), &data[4], length - 4);
	verse_send_packet_ack(pack_id);
}

boolean v_fs_buf_unpack_stored()
{
	unsigned int i = 0, length;
	uint8 cmd_id;
	char *data;
	while((data = v_nq_get_packed(v_con_get_network_queue(), &length)) != NULL)
	{
		i = 0;
		while(i < length)
		{
			i += vnp_raw_unpack_uint8(&data[i], &cmd_id);
			if(VCmdData.unpack_func[cmd_id] != NULL)
			{
				if(cmd_id == 7 || cmd_id == 8)
					i += VCmdData.unpack_func[cmd_id](&data[i], length - i, NULL, NULL);
				else
					i += VCmdData.unpack_func[cmd_id](&data[i], length - i, VCmdData.user_func[cmd_id], VCmdData.user_data[cmd_id]);
			}
		}
		free(data);
	}
	return i != 0;
}

char *connect_address = NULL;

char *v_fs_connect_get_address(void)
{
	return connect_address;
}

extern unsigned int v_unpack_connect(char *data, unsigned int length, void *user_func, void *user_data);

void v_fs_connect_unpack(uint8 *data, unsigned int length, char *address)
{
	unsigned int i = 0, pack_id;
	uint8 cmd_id;
	i += vnp_raw_unpack_uint32(&data[i], &pack_id);
	i += vnp_raw_unpack_uint8(&data[i], &cmd_id);
	connect_address = address;
	if(cmd_id == 0 || cmd_id == 4)
	i += v_unpack_connect(&data[i], length, VCmdData.user_func[cmd_id], VCmdData.user_data[cmd_id]);	
	connect_address = NULL;
}



#endif
