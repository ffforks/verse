/* A minimalist Verse example. Ask server for nodes, print information.
 * 
 * In Linux, this can be built with a command like so:
 * 
 * gcc -o list-nodes -I.. list-nodes.c  -L.. -lverse
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "verse.h"      /* Bring in the Verse API. */

/* A callback for connection acception: will be called when server accepts this client. */
static void callback_accept_connect(void *user, uint32 avatar, void *address, void *connection, uint8 *host_id)
{
	uint32 i, mask = 0;

	printf("Connected to a Verse host!\n\nListing nodes:\n");

	/* Build node subscription mask. */
	for(i = 0; i < V_NT_NUM_TYPES; i++)
		mask |= 1 << i;
	verse_send_node_index_subscribe(mask);     /* Request listing of all nodes. */
}

/* A callback for node creation: is called to report information about existing nodes, too. */
static void callback_node_create(void *user, VNodeID node_id, VNodeType type, VNodeOwner ownership)
{
	printf(" Node #%u has type %u\n", node_id, type);
}

/* A callback for termination of connection: is usually called, when user
   typed invalid username or password */
static void callback_connect_terminate(const char *address, const char *bye)
{
	printf("Message form server %s: %s\n", address, bye);
	
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]	)
{
	char buf[1024];
#if defined _WIN32
	char *username = "username";
	char *password = "password";
#else
	char *username = NULL;
	char *pass = NULL;
#endif
	
	/* Register callbacks for interesting commands. */
	verse_callback_set(verse_send_connect_accept,    callback_accept_connect, NULL);
	verse_callback_set(verse_send_node_create,	     callback_node_create, NULL);
	verse_callback_set(verse_send_connect_terminate, callback_connect_terminate, NULL);

#if defined _WIN32
	/* I'm not sure, if getpass() is available at Windows. */
#else
	/* get username and password from user */
	if(argc == 2) {
		username = argv[1];
	}
	else {
		printf("Syntax: %s <username>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	if(username!=NULL) {
		/* This function is probably available only at UNIX operating systems */
		pass = getpass("password: ");
	}
#endif

	if(pass!=NULL) {
		/* Kick off program by connecting to Verse host on local machine. */
		verse_send_connect(username, pass, "localhost", NULL);
		
		while(TRUE)
			verse_callback_update(10000);	/* Listen to network, get callbacks. */

		return EXIT_SUCCESS;		/* This is never reached. */
	}
	else {
		return EXIT_FAILURE;
	}
}
