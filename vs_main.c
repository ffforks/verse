/*
** A simple Verse server.
*/

#include <stdio.h>
#include <stdlib.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "vs_server.h"

extern VNodeID	vs_node_create(VNodeID owner_id, unsigned int type);
extern void	callback_send_node_destroy(void *user_data, VNodeID node_id);
void vs_reset_owner(VNodeID owner_id);

static void callback_send_connect(void *user, char *name, char *pass, void *address, uint8 *host_id)
{
	VNodeID avatar;
	VSession *session;

	if(TRUE)
	{
		avatar = vs_node_create(0, V_NT_OBJECT);
		session = verse_send_connect_accept(avatar, address, NULL);
		vs_add_new_connection(session, name, pass, avatar);
	}
	else
	{
		verse_send_connect_terminate(address, "I'm sorry but you are not welcome here.");
	}
}

static void callback_send_connect_terminate(void *user, char *address, char *bye)
{
	printf("callback_send_connect_terminate\n");
	vs_reset_owner(vs_get_avatar());
	callback_send_node_destroy(NULL, vs_get_avatar());
	verse_session_destroy(vs_get_session());
	vs_remove_connection();
}

static void callback_send_ping(void *user, char *address, char *text)
{
	verse_send_ping(address, "You have reached a Verse server, I'm not home at the moment, but please leave a message.");
}

int main(int argc, char **argv)
{
	printf("Verse Server r%up%u%s By Eskil Steenberg <http://www.blender.org/modules/verse/>\n", V_RELEASE_NUMBER, V_RELEASE_PATCH, V_RELEASE_LABEL);
	verse_set_port(4950); /* this is the standard port */
	vs_init_node_storage();
	vs_o_callback_init();
	vs_g_callback_init();
	vs_m_callback_init();
	vs_b_callback_init();
	vs_t_callback_init();
	vs_c_callback_init();
	vs_h_callback_init();
	init_callback_node_storage();
	verse_callback_set(verse_send_connect,		callback_send_connect,		NULL);
	verse_callback_set(verse_send_ping,		callback_send_ping,		NULL);
	verse_callback_set(verse_send_connect_terminate, callback_send_connect_terminate, NULL);

	while(TRUE)
	{
		vs_set_next_session();
		verse_callback_update(1000000);
	}
	return EXIT_SUCCESS;
}

#endif		/* V_GENERATE_FUNC_MODE */
