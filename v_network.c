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
#include <winsock2.h>
#include <ws2tcpip.h>
typedef unsigned int uint;
typedef SOCKET VSocket;
#define inet_ntop InetNtop
#else
typedef int VSocket;
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#endif
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char uint8;
typedef char int8;
typedef unsigned char boolean;

#include "v_cmd_gen.h"
#include "v_network.h"

#define	TRUE	1
#define	FALSE	0

#define VERSE_STD_CONNECT_TO_PORT 4950

static VSocket	my_socket = -1;
static uint16	my_port = 0;
static uint32	my_protocol = 4;	/* Default protocol is 4, TODO: add autodetection of supported protocols */

boolean compare_ip6_addr(struct in6_addr *addr1, struct in6_addr *addr2)
{
	if((addr1->s6_addr[0]==addr2->s6_addr[0]) &&
			(addr1->s6_addr[1]==addr2->s6_addr[1]) &&
			(addr1->s6_addr[2]==addr2->s6_addr[2]) &&
			(addr1->s6_addr[3]==addr2->s6_addr[3]))
		return TRUE;
	else
		return FALSE;
}

void v_n_set_protocol(unsigned int protocol)
{
	if(protocol==0) {
		/* if not set, then default protocol will be IPv4 */
		my_protocol = 4;
		/* TODO: add autodetection if IPv6 could be used */
	}
	else {
		my_protocol = protocol;
	}
}

void v_n_set_port(unsigned short port)
{
	my_port = port;
}

static boolean net_initialized = FALSE;
#if defined _WIN32
void init_wsa(void)
{
	if(!net_initialized)
	{
		WSADATA wsaData;

		if(WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		{
			fprintf(stderr, "WSAStartup failed.\n");
			exit(1);
		}

	}
}
#endif

VSocket v_n_socket_create(void)
{
	
	struct sockaddr_in address_in4;
	struct sockaddr_in6 address_in6;
	int buffer_size = 1 << 20;

	if(my_socket != -1)
		return my_socket;

#if defined _WIN32
	init_wsa();
#endif
	net_initialized = TRUE;
	if(my_protocol==6) {
		if((my_socket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
			return -1;
	}
	else if(my_protocol==4) {
		if((my_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
			return -1;
	}
#if defined _WIN32
	{
		unsigned long	one = 1UL;
		if(ioctlsocket(my_socket, FIONBIO, &one) != 0)
			return -1;
	}
#else
	if(fcntl(my_socket, F_SETFL, O_NONBLOCK) != 0)
	{
		fprintf(stderr, "v_network: Couldn't make socket non-blocking\n");
		return -1;
	}
#endif
	if(my_protocol==6) {
		address_in6.sin6_family = AF_INET6;     /* host byte order */
		address_in6.sin6_port = htons(my_port); /* short, network byte order */
		address_in6.sin6_addr = in6addr_any;
		if(bind(my_socket, (struct sockaddr *) &address_in6, sizeof(address_in6)) != 0)
		{
			fprintf(stderr, "v_network: Failed to bind(), code %d (%s)\n", errno, strerror(errno));
			exit(EXIT_FAILURE); /* FIX ME */
		}
	}
	else if(my_protocol==4) {
		address_in4.sin_family = AF_INET;     /* host byte order */
		address_in4.sin_port = htons(my_port); /* short, network byte order */
		address_in4.sin_addr.s_addr = INADDR_ANY;
		if(bind(my_socket, (struct sockaddr *) &address_in4, sizeof(address_in4)) != 0)
		{
			fprintf(stderr, "v_network: Failed to bind(), code %d (%s)\n", errno, strerror(errno));
			exit(EXIT_FAILURE); /* FIX ME */
		}
	}
	if(setsockopt(my_socket, SOL_SOCKET, SO_SNDBUF, (const char *) &buffer_size, sizeof buffer_size) != 0)
		fprintf(stderr, "v_network: Couldn't set send buffer size of socket to %d\n", buffer_size);
	if(setsockopt(my_socket, SOL_SOCKET, SO_RCVBUF, (const char *) &buffer_size, sizeof buffer_size) != 0)
		fprintf(stderr, "v_network: Couldn't set receive buffer size of socket to %d\n", buffer_size);
	return my_socket;
}

void v_n_socket_destroy(void)
{
#if defined _WIN32
	closesocket(my_socket);
#else
	close(my_socket);
#endif
	my_socket = -1;
}

boolean v_n_set_network_address(VNetworkAddress *address, const char *host_name)
{
	struct hostent	*he;
	char		*colon = NULL, *left_bracket = NULL, *right_bracket = NULL, *buf = NULL, *port_name=NULL, *last_colon=NULL;
	boolean		ok = FALSE;
	unsigned short port;

	port = VERSE_STD_CONNECT_TO_PORT;

	/* If a port number after IPv6 address is included, as indicated by a bracket and colon, we need to work a bit more.
	 * Example of URI: [::1]:15283, ip6-localhost:44851 */
	if((left_bracket = strchr(host_name, '[')) != NULL && (right_bracket = strchr(host_name, ']')) != NULL) {
			unsigned int tp;
			int i, len=right_bracket - left_bracket;

			/* parse hostname */
			buf = (char*)malloc(sizeof(char)*(len));
			for(i=0, left_bracket++;left_bracket!=right_bracket;i++, left_bracket++) {
				buf[i] = *left_bracket;
			}
			buf[i] = '\0';
			
			/* parse port */
			last_colon = right_bracket+1;
			if(*last_colon==':') {
				int j;
				port_name = (char*)malloc(sizeof(char)*(strlen(host_name)-i-2));
				for(j=0, i+2, last_colon++; i<strlen(host_name); i++, j++, last_colon++) {
					port_name[j] = *last_colon;
				}
				port_name[j+1] = '\0';
			}
			host_name = buf;
			if(sscanf(port_name, "%u", &tp)==1){
				port = (unsigned short) tp;
				if(port!=tp)
					host_name = NULL;
			}
			else {
				host_name = NULL;
			}
			if(port_name) free(port_name);
	}
	/* If a port number after  IPv4 address is included, as indicated by a colon, we need to work a bit more.
	 * Example of URI: 127.0.0.1:11283, localhost:21845 */
	else if((colon = strchr(host_name, ':')) != NULL)
	{
		size_t	hl = strlen(host_name);

		if((buf = malloc(hl + 1)) != NULL)
		{
			unsigned int	tp;

			strcpy(buf, host_name);
			colon = buf + (colon - host_name);
			*colon = '\0';
			host_name = buf;
			if(sscanf(colon + 1, "%u", &tp) == 1)
			{
				port = (unsigned short) tp;
				if(port != tp)	/* Protect against overflow. */
					host_name = NULL;
			}
			else
				host_name = NULL;	/* Protect against parse error. */
		}
		else
			return FALSE;
	}

	if(host_name != NULL)
	{
		
#ifdef _WIN32
		init_wsa();
		
		if((he = gethostbyname(host_name))!=NULL) {
			if(he->h_addrtype == AF_INET6) {
				memset((char*)&address->addr6, 0, sizeof(struct sockaddr_in));
				memcpy((char*)&address->addr6.sin6_addr, he->h_addr_list[0], he->h_length);
				address->addrtype = address->addr6.sin6_family = he->h_addrtype;
				address->addr6.sin6_port = htons(port);
				ok = TRUE;
			} else if(he->h_addrtype == AF_INET) {
				memset((char*)&address->addr4, 0, sizeof(struct sockaddr_in));
				memcpy((char*)&address->addr4.sin_addr, he->h_addr_list[0], he->h_length);
				address->addrtype = address->addr4.sin_family = he->h_addrtype;
				address->addr4.sin_port = htons(port);
				ok = TRUE;
			} else {
				printf("unsupported protocol\n");
				perror("gethostbyname()");
			}
		} else {
			int errcode = WSAGetLastError();
			switch(errcode) {
				case WSANOTINITIALISED:
					printf("Not initialised: %d\n", errcode);
					break;
				case WSAENETDOWN:
					printf("Network subsystem failed: %d\n", errcode);
					break;
				case WSAHOST_NOT_FOUND:
					printf("Authorative host not found: %d\n", errcode);
					break;
				case WSATRY_AGAIN:
					printf("Nonauthorative host not found, or server failure: %d\n", errcode);
					break;
				case WSANO_RECOVERY:
					printf("Nonrecoverable error occurred: %d\n", errcode);
					break;
				case WSANO_DATA:
					printf("Valid name, no data record of requested type: %d\n", errcode);
					break;
				case WSAEINPROGRESS:
					printf("A blocking WSA 1.1 call is in progress, or the service provider is still processing a callback function: %d\n", errcode);
					break;
				case WSAEFAULT:
					printf("The name parameter is not a valid part of the user address space: %d\n", errcode);
					break;
				case WSAEINTR:
					printf("A blocking WSA 1.1 call was cancelled through WSACancelBlockingCall: %d\n", errcode);
					break;
				default:
					printf("Unknown network error occurred\n");
					break;
			}
			
			perror("gethostbyname()");
		}
		
#else
		if((he = gethostbyname2(host_name, AF_INET6)) != NULL) {
			memset((char*)&address->addr6, 0, sizeof(struct sockaddr_in));
			memcpy((char*)&address->addr6.sin6_addr, he->h_addr_list[0], he->h_length);
			address->addrtype = address->addr6.sin6_family = he->h_addrtype;
			address->addr6.sin6_port = htons(port);
			ok = TRUE;
		}
		else if((he = gethostbyname2(host_name, AF_INET)) != NULL) {
			memset((char*)&address->addr4, 0, sizeof(struct sockaddr_in));
			memcpy((char*)&address->addr4.sin_addr, he->h_addr_list[0], he->h_length);
			address->addrtype = address->addr4.sin_family = he->h_addrtype;
			address->addr4.sin_port = htons(port);
			ok = TRUE;
		}
		else {
			perror("gethostbyname2()");
		}
#endif
	} else {
		printf("hostname == NULL\n");
	}

	/* Free buffer used for parsing URI */
	if(buf != NULL) free(buf);

	/* When my_socket isn't created, then we can influence type of
	 * socket for verse client due to address type */
	if(my_socket==-1) {
		if(address->addrtype==AF_INET6)
			my_protocol = 6;
		else if(address->addrtype==AF_INET)
			my_protocol = 4;

		/* Create new socket */
		v_n_socket_create();
	}

	return ok;
}

int v_n_send_data(VNetworkAddress *address, const char *data, size_t length)
{
	VSocket		sock;
	int			ret;
	
	if((sock = v_n_socket_create()) == -1 || length == 0)
		return 0;


	if(address->addrtype==AF_INET6){
/*		char str[32];
		inet_ntop(AF_INET6, &address->addr6.sin6_addr, str, 32);
		printf("\tDEBUG: v_n_send_data(), address: %s, port: %d, len: %d\n", str, ntohs(address->addr6.sin6_port), length);*/

		ret = sendto(sock, data, length, 0, (struct sockaddr *) &address->addr6, sizeof(struct sockaddr_in6));
	}
	else if(address->addrtype==AF_INET)
		ret = sendto(sock, data, length, 0, (struct sockaddr *) &address->addr4, sizeof(struct sockaddr_in));

	if(ret < 0)
		fprintf(stderr, "Socket sendto() of %u bytes failed, code %d (%s)\n", (unsigned int) length, errno, strerror(errno));
	return ret;
}

#if !defined V_GENERATE_FUNC_MODE

extern void *v_con_get_network_address_id(unsigned int id);
extern unsigned int v_con_get_network_address_count();

unsigned int v_n_wait_for_incoming(unsigned int microseconds) 
{
	struct timeval	tv;
	fd_set		fd_select;
	unsigned int	s1, f1, s2, f2;

	if(microseconds == 0)
		return 0;
	v_n_socket_create();
	tv.tv_sec  = microseconds / 1000000;
	tv.tv_usec = microseconds % 1000000;
	FD_ZERO(&fd_select);
	FD_SET(my_socket, &fd_select);
	v_n_get_current_time(&s1, &f1);
	select(1, &fd_select, NULL, NULL, &tv);
	v_n_get_current_time(&s2, &f2);
	return (unsigned int) (1000000 * (s2 - s1) + (1000000.0 / 0xffffffffu) * (long) (f2 - f1));	/* Must cast to (long) for f1 > f2 case! */
}

#endif

int v_n_receive_data(VNetworkAddress *address, char *data, size_t length)
{
	VSocket	sock;
	struct	sockaddr_in address_in4;
	struct	sockaddr_in6 address_in6;
	size_t	from_length, len;

	if((sock = v_n_socket_create()) == -1)
		return 0;

	if(my_protocol==6) {
		from_length = sizeof(address_in6);
		memset(&address_in6, 0, from_length);

		address_in6.sin6_family = AF_INET6;
		address_in6.sin6_port = htons(my_port); 
		address_in6.sin6_addr = in6addr_any;
		len = recvfrom(sock, data, length, 0, (struct sockaddr *) &address_in6, &from_length);
		if(len > 0 && len!=-1)
		{
/*			char str[32];
			inet_ntop(AF_INET6, &address_in6.sin6_addr, str, 32);
			printf("\tDEBUG: IPv6 v_n_receive_data(), address: %s, port: %d, len: %d\n", str, ntohs(address_in6.sin6_port), len);*/

			address->addrtype = AF_INET6;
			memcpy((char*)&address->addr6, (char*)&address_in6, sizeof(struct sockaddr_in6));
		}
	}
	else if(my_protocol==4) {
		from_length = sizeof(address_in4);
		memset(&address_in4, 0, from_length);

		address_in4.sin_family = AF_INET;
		address_in4.sin_port = htons(my_port); 
		address_in4.sin_addr.s_addr = INADDR_ANY;
		len = recvfrom(sock, data, length, 0, (struct sockaddr *) &address_in4, &from_length);
		if(len > 0 && len!=-1)
		{
			address->addrtype = AF_INET;
			memcpy((char*)&address->addr4, (char*)&address_in4, sizeof(struct sockaddr_in));
		}
	}
	return len;
}

#if defined _WIN32

void v_n_get_current_time(uint32 *seconds, uint32 *fractions)
{
	static LARGE_INTEGER frequency;
	static boolean init = FALSE;
	LARGE_INTEGER counter;

	if(!init)
	{
		init = TRUE;
		QueryPerformanceFrequency(&frequency);
	}

	QueryPerformanceCounter(&counter);
	if(seconds != NULL)
		*seconds = (uint32) (counter.QuadPart / frequency.QuadPart);
	if(fractions != NULL)
		*fractions = (uint32) ((0xffffffffUL * (counter.QuadPart % frequency.QuadPart)) / frequency.QuadPart);
}

#else

void v_n_get_current_time(uint32 *seconds, uint32 *fractions)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	if(seconds != NULL)
	    *seconds = tv.tv_sec;
	if(fractions != NULL)
		*fractions = tv.tv_usec * 1E-6 * (double) (uint32)~0;
}

#endif

void v_n_get_address_string(const VNetworkAddress *address, char *string)
{
	char str[46];

	#if defined _WIN32
	sprintf(string, "%s", inet_ntoa(address->addr4.sin_addr));
	#else
	if(address->addrtype==AF_INET6) {
		inet_ntop(AF_INET6, &address->addr6.sin6_addr, str, 32);
		sprintf(string, "[%s]:%u", str, htons(address->addr6.sin6_port));
	}
	else if(address->addrtype==AF_INET) {
		inet_ntop(AF_INET, &address->addr4.sin_addr, str, 32);
		sprintf(string, "%s:%u", str, htons(address->addr4.sin_port));
	}
	#endif
}

