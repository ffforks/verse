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

#if defined _WIN32
/* Windows doesn't use PAM ... we have to find some similar solution for Windows */
#else
/* on UNIX like OS add PAM header files */
#define VERSE_WITH_PAM 1
#if defined  __APPLE__
/* MAC OS X */
#include <pam/pam_appl.h>
#include <pam/pam_misc.h>
#elif defined __linux__
/* Linux */
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#else
/* Add path for PAM header files for other UNIX operating systems */
#undef VERSE_WITH_PAM
#endif
/* end of PAM */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "v_cmd_gen.h"

#if !defined V_GENERATE_FUNC_MODE

#include "verse.h"
#include "v_network.h"
#include "v_util.h"
#include "vs_server.h"

extern VNodeID	vs_node_create(VNodeID owner_id, unsigned int type);
extern void	callback_send_node_destroy(void *user_data, VNodeID node_id);
extern void	vs_reset_owner(VNodeID owner_id);

#if defined VERSE_WITH_PAM

/* verse conversation function for PAM */
int verse_conv(int num_msg,
						const struct pam_message **msg,
						struct pam_response **resp,
						void *appdata_ptr)
{
	struct pam_response *r;
	int i;

	if (num_msg <= 0 || num_msg > PAM_MAX_NUM_MSG)
		return (PAM_CONV_ERR);

	if((r=calloc(num_msg,sizeof *r))==NULL)
		return PAM_BUF_ERR;
	
	for(i=0;i<num_msg; i++) {
		r[i].resp = NULL;
		r[i].resp_retcode = 0;
		
		switch(msg[i]->msg_style) {
			case PAM_PROMPT_ECHO_ON:
				fprintf(stdout, "%s", msg[i]->msg);
				/* not neccessary for our needs */
				break;
			case PAM_PROMPT_ECHO_OFF:
				if(strncmp("Password:",msg[i]->msg,9)==0) {
					r[i].resp = strdup((char*)appdata_ptr);
					if(r[i].resp == NULL)
						goto fail;
				}
				else {
					fprintf(stdout, "%s", msg[i]->msg);
				}
				break;
			case PAM_ERROR_MSG:
				fprintf(stderr, "%s\n", msg[i]->msg);
				break;
			case PAM_TEXT_INFO:
				fprintf(stdout, "%s\n", msg[i]->msg);
				break;
			default:
				goto fail;
		}
	}
	*resp = r;

	return PAM_SUCCESS;
fail:
	for (i = 0; i < num_msg; i++) {
		if (r[i].resp != NULL) {
			memset(r[i].resp, 0, strlen(r[i].resp));
			free(r[i].resp);
		}
	}

	memset(r, 0, num_msg * sizeof *r);
	*resp = NULL;

	return PAM_CONV_ERR;
}

/* variable storing pointer at PAM conversation function and application data */
static struct pam_conv conv = {verse_conv, NULL};

/* terminate pam transaction */
static void end_user_authentication(pam_handle_t *pamh, int retval)
{
	conv.appdata_ptr = NULL;
	
	if(pam_end(pamh, retval) != PAM_SUCCESS) {
		pamh = NULL;
		fprintf(stderr, "Failed to release authentificator.\n");
	}
}

/* authentification of user, it uses only name and password now */
static int authenticate_user(const char *name,
								const char *pass,
								const char *address)
{
	pam_handle_t *pamh = NULL;
	int retval;
	
	conv.appdata_ptr = (void*)pass;
	
	/* initialization of pam transaction */
	if((retval = pam_start("verse", name, &conv, &pamh)) == PAM_SUCCESS) {
		printf("DEBUG: pam_start [OK]\n");
		/* authenticate user */
		if((retval = pam_authenticate(pamh, 0)) == PAM_SUCCESS) {
			printf("DEBUG: pam_authenticate [OK]\n");
			/* is user's account valid? */
			if((retval = pam_acct_mgmt(pamh, 0)) == PAM_SUCCESS) {
				printf("DEBUG: pam_acct_mgmt [OK]\n");
				end_user_authentication(pamh, retval);
				return TRUE;
			}
			else {
				printf("DEBUG: pam_acct_mgmt [failed] %d\n", retval);
				end_user_authentication(pamh, retval);
				return FALSE;
			}
		}
		else {
			printf("DEBUG: pam_authenticate [failed] %d\n", retval);
			end_user_authentication(pamh, retval);
			return FALSE;
		}
	}
	else {
		printf("DEBUG: pam_start [failed] %d\n", retval);
		end_user_authentication(pamh, retval);
		return FALSE;
	}
}

#endif

static void callback_send_connect(void *user, const char *name, const char *pass, const char *address, const uint8 *host_id)
{
	VNodeID avatar;
	VSession *session;

	printf("Connecting '%s' from %s ... ", name, address);
#if defined VERSE_WITH_PAM
	if(authenticate_user(name, pass, address))
#else
	if(TRUE)
#endif
	{
		printf("OK\n");
		avatar = vs_node_create(~0, V_NT_OBJECT);
		session = verse_send_connect_accept(avatar, address, NULL);
		vs_add_new_connection(session, name, pass, avatar);
/*		vs_avatar_init(avatar, name);*/
	}
	else
	{
		printf("failed\n");
		verse_send_connect_terminate(address, "I'm sorry, but you are not welcome here.");
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

static void vs_load_host_id(const char *file_name)
{
	FILE	*f;
	uint8	id[V_HOST_ID_SIZE];
	size_t	got;

	/* Attempt to read key from given filename. Fails silently. */
	if((f = fopen(file_name, "rb")) != NULL)
	{
		if((got = fread(id, 1, sizeof id, f)) > 0)
		{
			printf("Loaded %u-bit host ID key successfully\n", 8 * (got / 3));
			verse_host_id_set(id);
		}
		fclose(f);
		if(got)
			return;
	}
	/* If file didn't open, or reading failed, generate a new key and write it out. */
	verse_host_id_create(id);
	verse_host_id_set(id);
	if((f = fopen(file_name, "wb")) != NULL)
	{
		if(fwrite(id, sizeof id, 1, f) != 1)
			fprintf(stderr, "Warning: Couldn't write host ID to \"%s\"\n", file_name);
		fclose(f);
	}
	else
		fprintf(stderr, "Warning: Couldn't open \"%s\" for host ID writing\n", file_name);
}

static void cb_sigint_handler(int sig)
{
	if(sig == SIGINT)
	{
		printf("Verse server terminating\n");
		exit(EXIT_SUCCESS);
	}
}

static void callback_send_ping(void *user, const char *address, const char *message)
{
	if(strncmp(message, "DESCRIBE", 8) == 0 && message[8] == ' ')
		vs_master_handle_describe(address, message + 9);
}

static void usage(void)
{
	printf("Verse server usage:\n");
	printf(" -h\t\t\tShow this usage information.\n");
	printf(" -ms\t\t\tRegisters the server with a master server at the address\n");
	printf(" \t\t\tgiven with the -ms:ip= option. Off by default.\n");
	printf(" -ms:ip=IP[:PORT]\tSet master server to register with. Implies -ms.\n");
	printf(" \t\t\tThe default address is <%s>.\n", vs_master_get_address());
	printf(" -ms:de=DESC\t\tSet description, sent to master server.\n");
	printf(" -ms:ta=TAGS\t\tSet tags, sent to master server.\n");
	printf(" -port=PORT\t\tSet port to use for incoming connections.\n");
	printf(" -4\t\t\tForces Verse server to use IPv4 addresess only.\n");
	printf(" -6\t\t\tForces Verse server to use IPv6 addresess only.\n");
	printf(" -version\t\tPrint version information and exit.\n");
}

int main(int argc, char **argv)
{
	uint32		i, seconds, fractions, port = VERSE_STD_CONNECT_PORT, protocol = 0 /* IPv4 or IPv6 */;

	signal(SIGINT, cb_sigint_handler);

	vs_master_set_address("master.uni-verse.org");		/* The default master address. */
	vs_master_set_enabled(FALSE);				/* Make sure master server support is disabled. */
	for(i = 1; i < (uint32) argc; i++)
	{
		if(strcmp(argv[i], "-h") == 0)
		{
			usage();
			return EXIT_SUCCESS;
		}
		else if(strcmp(argv[i], "-ms") == 0)
			vs_master_set_enabled(TRUE);
                else if(strncmp(argv[i], "-ms:ip=", 7) == 0)
		{
                        vs_master_set_address(argv[i] + 7);
			vs_master_set_enabled(TRUE);
		}
                else if(strncmp(argv[i], "-ms:de=", 7) == 0)
                        vs_master_set_desc(argv[i] + 7);
                else if(strncmp(argv[i], "-ms:ta=", 7) == 0)
			vs_master_set_tags(argv[i] + 7);
		else if(strncmp(argv[i], "-port=", 6) == 0)
			port = strtoul(argv[i] + 6, NULL, 0);
		else if(strncmp(argv[i], "-4", 2) == 0 && protocol!=6)
			protocol = 4;
		else if(strncmp(argv[i], "-6", 2) == 0 && protocol!=4)
			protocol = 6;
		else if(strcmp(argv[i], "-version") == 0)
		{
			printf("r%up%u%s\n", V_RELEASE_NUMBER, V_RELEASE_PATCH, V_RELEASE_LABEL);
			return EXIT_SUCCESS;
		}
		else
			fprintf(stderr, "Ignoring unknown argument \"%s\", try -h for help\n", argv[i]);
	}

	printf("Verse Server r%up%u%s by Eskil Steenberg <http://verse.blender.org/>\n", V_RELEASE_NUMBER, V_RELEASE_PATCH, V_RELEASE_LABEL);
	verse_set_port(port);	/* The Verse standard port. */
	verse_set_protocol(protocol);	/* Set used protocol (IPv4 or IPv6) */
	printf(" Listening on port %d\n", port);

	/* Seed the random number generator. Still rather too weak for crypto, I guess. */
	v_n_get_current_time(&seconds, &fractions);
	srand(seconds ^ fractions);

	vs_load_host_id("host_id.rsa");
	vs_init_node_storage();
	vs_o_callback_init();
	vs_g_callback_init();
	vs_m_callback_init();
	vs_b_callback_init();
	vs_t_callback_init();
	vs_c_callback_init();
	vs_a_callback_init();
	vs_h_callback_init();
	init_callback_node_storage();
	verse_callback_set(verse_send_ping,		callback_send_ping, NULL);
	verse_callback_set(verse_send_connect,		callback_send_connect,		NULL);
	verse_callback_set(verse_send_connect_terminate, callback_send_connect_terminate, NULL);

	while(TRUE)
	{
		vs_set_next_session();
		verse_callback_update(1000000);
		vs_master_update();
	}
	return EXIT_SUCCESS;
}

#endif		/* V_GENERATE_FUNC_MODE */
