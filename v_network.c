/*
**
*/

#if defined _WIN32
#include <winsock.h>
typedef unsigned int uint;
typedef SOCKET VSocket;
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
typedef char boolean;

#include "v_cmd_gen.h"
#include "v_network.h"

#define	TRUE	1
#define	FALSE	0

typedef struct{
	struct sockaddr_in address;
	struct hostent *he;
} VNetworkConnection;

#define VERSE_STD_CONNECT_TO_PORT 4950

VSocket	my_socket = -1;
uint16	my_port = 0;

/*
struct VNetworkAddress {
	VSocket	socket;
	uint32	their_ip;
	uint16	their_port;
	uint16	my_port;
};*/

void v_n_set_port(unsigned short port)
{
	my_port = port;
}

VSocket v_n_socket_create(void)
{
	static boolean initialized = FALSE;
	struct sockaddr_in address;
	if(my_socket != -1)
		return my_socket;
#if defined _WIN32
	if(!initialized)
	{
		WSADATA wsaData;

		if(WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
		{
			fprintf(stderr, "WSAStartup failed.\n");
			exit(1);
		}

	}
#endif
	initialized = TRUE;
	if((my_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		return -1;
#if defined _WIN32
	{
		unsigned long	one = 1UL;
		if(ioctlsocket(my_socket, FIONBIO, &one) != 0)
			return -1;
	}
#else
	if(fcntl(my_socket, F_SETFL, O_NONBLOCK) != 0)
		return -1;
#endif
	address.sin_family = AF_INET;     /* host byte order */
	address.sin_port = htons(my_port); /* short, network byte order */
	address.sin_addr.s_addr = INADDR_ANY;
	if(bind(my_socket, (struct sockaddr *) &address, sizeof(struct sockaddr)) != 0)
		exit(0); /* FIX ME */
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
/*
void v_n_destroy_network_address(VNetworkAddress *address)
{
	v_n_socket_destroy(address->socket);
	free(address);
}
*/
/* A strdup() implementation, since we can't depend on it being available in e.g. Windows. 
static char * v_n_strdup(const char *str)
{
	size_t	len;
	char	*buf;

	if(str == NULL)
		return NULL;
	len = strlen(str);
	if((buf = malloc(len + 1)) != NULL)
	{
		strcpy(buf, str);
		return buf;
	}
	return NULL;
}*/

boolean v_n_set_network_address(VNetworkAddress *address, const char *host_name)
{
	struct hostent *he;
	char *colon = NULL;
	v_n_socket_create();
	address->port = VERSE_STD_CONNECT_TO_PORT;
	if((colon = strchr(host_name, ':')) != NULL)
	{
		unsigned int tp;
		*colon = '\0';
		if(sscanf(colon + 1, "%u", &tp) == 1)
			address->port = tp;
	}
	if((he = gethostbyname(host_name)) != NULL)
	{
		memcpy(&address->ip, he->h_addr_list[0], he->h_length);
		address->ip = htonl(address->ip);
	}else
		return FALSE;
	if(colon != NULL)
		*colon = ':';
	return TRUE;
}

int v_n_send_data(VNetworkAddress *address, const char *data, size_t length)
{
	struct sockaddr_in	address_in;
	if(v_n_socket_create() == -1)
		return 0;
	address_in.sin_family = AF_INET;     /* host byte order */
	address_in.sin_port = htons(address->port); /* short, network byte order */
	address_in.sin_addr.s_addr = htonl(address->ip);
	memset(&address_in.sin_zero, 0, sizeof address_in.sin_zero);
/*	{
		char string[32];
		v_n_get_address_string(address, string);
		printf("send to %s\n", string);
	}*/
	return sendto(v_n_socket_create(), data, length, 0, (struct sockaddr *) &address_in, sizeof(struct sockaddr));
}

#if !defined V_GENERATE_FUNC_MODE

extern void *v_con_get_network_address_id(unsigned int id);
extern unsigned int v_con_get_network_address_count();

void v_n_wait_for_incoming(unsigned int microseconds) 
{
	struct timeval tv;
	fd_set fd_select;
	if(microseconds == 0)
		return;
	v_n_socket_create();
	tv.tv_sec = microseconds / 1000000;
	tv.tv_usec = microseconds % 1000000;
	FD_ZERO(&fd_select);
/*	address = v_con_get_network_address_id(i);*/
	FD_SET(my_socket, &fd_select);
	select(1, &fd_select, NULL, NULL, &tv);
}

#endif

int v_n_receive_data(VNetworkAddress *address, char *data, size_t length)
{
	size_t	from_length = sizeof(struct sockaddr);
	int		output_length;
	struct	sockaddr_in address_in;
	if(v_n_socket_create() == -1)
		return 0;
	memset(&address_in, 0, sizeof address_in);
	address_in.sin_family = AF_INET;
	address_in.sin_port = htons(my_port); 
	address_in.sin_addr.s_addr = INADDR_ANY;
	output_length = recvfrom(v_n_socket_create(), data, length, 0, (struct sockaddr *) &address_in, &from_length);
	address->ip   = ntohl(address_in.sin_addr.s_addr);
	address->port = ntohs(address_in.sin_port);
	return output_length;
}


#if defined _WIN32

void v_n_get_current_time(uint32 *seconds, uint32 *fractions)
{
    static LARGE_INTEGER freqency;
    static boolean init = FALSE;
    LARGE_INTEGER conter;

    if(!init)
    {
        init = TRUE;
        QueryPerformanceFrequency(&freqency);
    }

    QueryPerformanceCounter(&conter);
	if(seconds != NULL)
		*seconds = conter.QuadPart / freqency.QuadPart;
	if(fractions != NULL)
		*fractions = (uint32)((conter.QuadPart % freqency.QuadPart) * (0xffffffff / freqency.QuadPart));
}

#else

void v_n_get_current_time(uint32 *seconds, uint32 *fractions)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
	if(seconds != NULL)
	    *seconds = tv.tv_sec;
	if(fractions != NULL)
		*fractions = (uint32)((double)tv.tv_usec / (double)1000000 * (double)((uint32)-1));
}

#endif


void v_n_get_address_string(const VNetworkAddress *address, char *string)
{
	sprintf(string, "%u.%u.%u.%u:%u", address->ip / (256 * 256 * 256), (address->ip / (256 * 256)) % 256, (address->ip / 256) % 256, address->ip % 256, address->port);
}
