/*
**
*/

#include <stdlib.h>
#include <stdio.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "v_cmd_buf.h"
#include "v_network_out_que.h"
#include "v_network.h"
#include "v_connection.h"
#include "v_encryption.h"

void v_send_hidden_connect_contact() /*  Stage 0: Clinets inital call to connect to host */
{
	uint8 buf[V_ENCRYPTION_LOGIN_KEY_SIZE + 4 + 1 + 1], *public_key;
	unsigned int i, buffer_pos = 0;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 0);/* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0);/* Packing the command */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0);/* Stage 0 */
	public_key = v_con_get_my_public_key();
	for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE; i++)
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], public_key[i]);/* Packing the command */
	v_n_send_data(v_con_get_network_address(), buf, buffer_pos);
}

void v_send_hidden_connect_send_key() /*  Stage 1: Hosts reply to any atempt to connect */
{
	uint8 buf[V_ENCRYPTION_LOGIN_KEY_SIZE * 3 + 4 + 1 + 1 + 1], *public_key, *encrypted_key;
	unsigned int i, buffer_pos = 0, s, f;

	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 0);/* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0);/* Packing the command */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 1);/* Stage 1 */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], V_RELEASE_NUMBER);/* version */
	v_n_get_current_time(&s, &f);
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], s);/* time, seconds */
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], f);/* time, fraction */
	public_key = v_con_get_my_public_key();
	for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE; i++)
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], public_key[i]);/* id key */
	encrypted_key = v_con_get_other_public_key();
	public_key = v_con_get_my_public_key();
	for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE; i++)
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], public_key[i]);/* public key */
	encrypted_key = v_con_get_other_public_key();
	for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE / 2; i++)
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], encrypted_key[i]);/* half other public key encrypted with id key */

	v_n_send_data(v_con_get_network_address(), buf, buffer_pos);
}

void v_send_hidden_connect_login() /* Stage 2: clients sends encrypted name and password */
{
	uint8 buf[1500], *name, *pass;
	unsigned int buffer_pos = 0;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 1);/* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 0);/* Packing the command */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 2);/* Stage 2 */
	name = v_con_get_name();
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], name, V_ENCRYPTION_LOGIN_KEY_SIZE / 2); /* packing name */
	pass = v_con_get_pass();
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], pass, V_ENCRYPTION_LOGIN_KEY_SIZE / 2); /* packing password */
	v_n_send_data(v_con_get_network_address(), buf, buffer_pos);
}

void v_send_hidden_connect_accept() /* Host accepts Clients connectionatempt and sends over data encryption key */
{
	uint8 buf[1500], *data_key;
	unsigned int i, buffer_pos = 0;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], 1);/* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 1);/* Packing the command */
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], verse_session_get_avatar());
	data_key = v_con_get_data_key();
	for(i = 0; i < V_ENCRYPTION_DATA_KEY_SIZE; i++)
		buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], data_key[i]);/* data key */
	v_n_send_data(v_con_get_network_address(), buf, buffer_pos);
}

void v_send_hidden_connect_terminate(VNetworkAddress *address, unsigned int packet_id, const char *bye) /* Host accepts Clients connectionatempt and sends over data encryption key */
{
	uint8 buf[1500];
	unsigned int buffer_pos = 0;
	buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], packet_id);/* Packing the packet id */
	buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], 2);/* Packing the command */
	buffer_pos += vnp_raw_pack_string(&buf[buffer_pos], bye, 512); /* pack message */
	v_e_encrypt_command(buf, sizeof(uint32), buf+sizeof(uint32), buffer_pos, v_con_get_data_key());
	v_n_send_data(address, buf, buffer_pos);
}


VSession * verse_send_connect(const char *name, const char *pass, const char *address, uint8 *key)
{

	VNetworkAddress a; 
	VSession *session;
	if(v_n_set_network_address(&a, address))
	{
		#if defined(V_PRINT_SEND_COMMANDS)
		char ip_string[32];
		#endif
		session = v_con_connect(a.ip, a.port, V_CS_CONTACT);
		#if defined(V_PRINT_SEND_COMMANDS)
		v_n_get_address_string(&a, ip_string);
		printf("send: %p = verse_send_connect(name = %s, pass = %s, address = %s (%s), key = %p); callback = %p\n", session, name, pass, address, ip_string, key);
		#endif
		v_con_set_name_pass(name, pass);
		if(key != NULL)
			v_con_set_other_public_key(key);
		v_send_hidden_connect_contact(&a, v_con_get_my_public_key());
		return session;
	}
	else
	{
		#if defined(V_PRINT_SEND_COMMANDS)
		printf("send: NULL = verse_send_connect(name = %s, pass = %s, address = %s (Unressolved DNS), key = %p); callback = %p\n", name, pass, address, key);
		#endif
		return NULL;
	}
}


void v_update_connection_pending()
{
	VSession (* func_connect)(void *user_data, char *address, const char *name, const char *pass, const char *key);
	VSession (* func_connect_accept)(void *user_data, VNodeID avatar, char *address, uint8 *host_id);
	void (* func_connect_termanate)(void *user_data, char *address, const char *bye);
	char address_string[32];

	switch(v_con_get_connect_stage())
	{
#if 0
	case V_CS_CONTACT :		/* client tries to contact host */
		v_send_hidden_connect_contact();
		break;
	case V_CS_CONTACTED :		/* Host replies with challange */
		v_send_hidden_connect_send_key();
		break;
	case V_CS_PENDING_ACCEPT :	/* Client sends login */
		v_send_hidden_connect_login();
		break;
#endif
	case V_CS_PENDING_HOST_CALLBACK : /* Host got login waits for accept connect callback */
		printf("0\n");
		v_con_set_connect_stage(V_CS_PENDING_DECISION);
		printf("1\n");
		func_connect = v_fs_get_user_func(0);
		printf("2\n");
		v_n_get_address_string(v_con_get_network_address(), address_string);
		#if defined(V_PRINT_RECEIVE_COMMANDS)
		printf("receive: verse_send_connect(address = %s, name = %s, pass = %s, key = NULL); callback = %p\n", address_string, v_con_get_name(), v_con_get_pass(), func_connect);
		#endif
		if(func_connect != 0)
			func_connect(v_fs_get_user_data(0), v_con_get_name(), v_con_get_pass(), address_string, NULL);
		break;
	case V_CS_PENDING_CLIENT_CALLBACK_ACCEPT : /* Host got login waits for accept connect callback */
		v_con_set_connect_stage(V_CS_CONNECTED);
		func_connect_accept = v_fs_get_user_func(1);
		v_n_get_address_string(v_con_get_network_address(), address_string);
		#if defined(V_PRINT_RECIVE_COMMANDS)
		printf("receive: func_connect_accept(avatar = %u, address = %s, name = %s, pass = %s, key = NULL); callback = %p\n", verse_session_get_avatar(), address_string, name, pass, func_connect);
		#endif
		if(func_connect_accept != 0)
			func_connect_accept(v_fs_get_user_data(1), verse_session_get_avatar(), address_string, NULL);
		break;
	case V_CS_PENDING_CLIENT_CALLBACK_TERMINATE : /* Host got login waits for accept connect callback */
		v_con_set_connect_stage(V_CS_CONNECTED);
		func_connect_termanate = v_fs_get_user_func(2);
		v_n_get_address_string(v_con_get_network_address(), address_string);
		#if defined(V_PRINT_RECIVE_COMMANDS)
		printf("receive: func_connect_termanate(address = %s, bye = %s); callback = %p\n", address_string, "no message", func_connect);
		#endif
		if(func_connect_termanate != 0)
			func_connect_termanate(v_fs_get_user_data(2), address_string, "no message");
		break;
	default:
		;
	}	
}

void v_unpack_connection(const char *buf, unsigned int buffer_length) /* un packing all stages of connect command */
{
	unsigned int buffer_pos = 0, i, pack_id;
	uint32 seconds, fractions;
	uint8 key[V_ENCRYPTION_LOGIN_KEY_SIZE], stage, cmd_id, version;

	if(buffer_length < 5)
		return;
	
	buffer_pos = vnp_raw_unpack_uint32(&buf[buffer_pos], &pack_id);
	buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &cmd_id);
	printf("connection packet %u, cmd %u", pack_id, cmd_id);
	if(cmd_id == 0)
	{
		buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &stage);
		printf(" stage %u\n", stage);
		if(/*pack_id == 0 &&*/ stage == 0 && V_CS_IDLE == v_con_get_connect_stage()) /* reseved by host */
		{
			for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE; i++)
				buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &key[i]);
			v_con_set_connect_stage(V_CS_CONTACTED);
			v_con_set_other_public_key(key);
			v_send_hidden_connect_send_key();
			return; 
		}
		if(/*pack_id == 0 &&*/ stage == 1 && V_CS_CONTACT == v_con_get_connect_stage()) /* reseved by client */
		{
			buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &version);
/*			if(version != V_RELEASE_NUMBER)
			{
				char error_message[128];
				func_connect_deny = v_fs_get_user_func(2);
				#if defined(V_PRINT_RECIVE_COMMANDS)
				printf("receive: verse_send_connect_deny(Host is running version %u you are running version %u); callback = %p\n", (uint32)version, (uint32)V_RELEASE_NUMBER func_connect_deny);
				#endif
				if(func_connect_deny != NULL)
				{
					sprintf(error_message, "Host is running version %u you are running version %u", (uint32)version, (uint32)V_RELEASE_NUMBER);
					func_connect_deny(v_fs_get_user_data(2), error_message);
				}
				return;
			}*/
			buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &seconds);
			buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &fractions);
			v_con_set_time(seconds, fractions);
			for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE; i++)
				buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &key[i]);
			v_con_set_other_public_key(key);
			for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE; i++)
				buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &key[i]);
			for(i = 0; i < V_ENCRYPTION_LOGIN_KEY_SIZE / 2; i++)
				buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &key[i]); 
			if(FALSE) /* Host is not who it appers top be */
			{
		/*		func_connect_deny = v_fs_get_user_func(2);
				#if defined(V_PRINT_RECIVE_COMMANDS)
				printf("receive: verse_send_connect_deny(Host failed identity check); callback = %p\n", func_connect_deny);
				#endif
				if(func_connect_deny != NULL)
					func_connect_deny(v_fs_get_user_data(2), "Host failed identity check");
				return;*/
			}
			v_con_set_connect_stage(V_CS_PENDING_ACCEPT);
			v_send_hidden_connect_login();
			return; 
		}
		if(/*pack_id == 0 &&*/ stage == 2 && V_CS_CONTACTED == v_con_get_connect_stage()) /* reseved by host */
		{
			char name[16], pass[16];
			VNetworkAddress *address;
			address = v_con_get_network_address();
			buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, V_ENCRYPTION_LOGIN_KEY_SIZE / 2, buffer_length - buffer_pos);
			buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], pass, V_ENCRYPTION_LOGIN_KEY_SIZE / 2, buffer_length - buffer_pos);
			v_con_set_name_pass(name, pass);
			v_con_set_connect_stage(V_CS_PENDING_HOST_CALLBACK);
			printf("got here right\n");
			return; 
		}
	}
	if(/*pack_id == 0 &&*/ cmd_id == 1 && V_CS_PENDING_ACCEPT == v_con_get_connect_stage()) /* reseved by client */
	{
		uint32 avatar;
		buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &avatar);
		v_con_set_avatar(avatar);
		for(i = 0; i < V_ENCRYPTION_DATA_KEY_SIZE; i++)
			buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &key[i]);
		v_con_set_connect_stage(V_CS_PENDING_CLIENT_CALLBACK_ACCEPT);
		v_con_set_data_key(key);
		v_send_hidden_connect_send_key();
		return; 
	}
	if(/*pack_id == 0 &&*/ cmd_id == 2 && V_CS_PENDING_ACCEPT == v_con_get_connect_stage()) /* reseved by client */
	{
	/*	buffer_pos += vnp_raw_unpack_string(&buf[buffer_pos], name, 512, buffer_length - buffer_pos);
	*/	v_con_set_connect_stage(V_CS_PENDING_CLIENT_CALLBACK_TERMINATE);
		return; 
	}
}

VSession * verse_send_connect_accept(VNodeID avatar, const char *address, uint8 *host_id)
{
	VNetworkAddress a;
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("send: verse_send_connect_accept(avatar = %u, address = %s, host_id = NULL);\n", avatar, address);
	#endif

	if(!v_n_set_network_address(&a, address))
		return NULL;
	if(v_co_switch_connection(a.ip, a.port))
	{
		if(v_con_get_connect_stage() != V_CS_PENDING_DECISION)
			return NULL;
		v_con_set_avatar(avatar);
		v_con_set_connect_stage(V_CS_CONNECTED);
		v_e_encrypt_data_key_generate(v_con_get_data_key());
		v_send_hidden_connect_accept();
		return v_con_get_network_queue();
	}
	return NULL;
}

void v_callback_connect_terminate(const char *bye)
{
	void (* func_connect_terminate)(void *user_data, char *address, const char *bye);
	char address_string[32];

	printf("terminate\n");
	func_connect_terminate = v_fs_get_user_func(2);
	v_n_get_address_string(v_con_get_network_address(), address_string);
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("receive: verse_send_connect_terminate(address = %s, bye = %s); callback = %p\n", address_string, bye, func_connect_terminate);
	#endif
	if(func_connect_terminate != 0)
		func_connect_terminate(v_fs_get_user_data(2), address_string, bye);
}


void verse_send_connect_terminate(const char *address, const char *bye)
{
	VNetworkAddress a;
	#if defined(V_PRINT_RECIVE_COMMANDS)
	printf("send: verse_send_connect_terminate(address = %s, bye = %s);\n", address, bye);
	#endif

	if(address == NULL)
		v_send_hidden_connect_terminate(v_con_get_network_address(), v_noq_get_next_out_packet_id(v_con_get_network_queue()), bye);
	else if(!v_n_set_network_address(&a, address))
		return;
	else if(v_co_switch_connection(a.ip, a.port))
		v_send_hidden_connect_terminate(v_con_get_network_address(), v_noq_get_next_out_packet_id(v_con_get_network_queue()), bye);

	if(v_con_get_connect_stage() != V_CS_PENDING_DECISION)
		verse_session_destroy(v_con_get_network_queue());
}

boolean v_ping_store(VNetworkAddress *address, char *buf, unsigned int length)
{
	return FALSE;
}

#endif
