/*
**
*/

#if !defined V_NETWORK_H
#define	V_NETWORK_H

#define VERSE_STD_CONNECT_PORT 4950

typedef struct VNetworkAddress VNetworkAddress;

extern void		v_n_wait_for_incoming(unsigned int milliseconds);
extern VNetworkAddress*	v_n_create_network_address(int my_port, const char *host_name);
extern void		v_n_destroy_network_address(VNetworkAddress *address);
extern unsigned int	v_n_send_data(VNetworkAddress *address, const char *data, unsigned int length);
extern unsigned int	v_n_receive_data(VNetworkAddress *address, char *data, unsigned int length, int connection_exclusive);
extern void		v_n_get_address_string(const VNetworkAddress *address, char *string);

#endif		/* V_NETWORK_H */
