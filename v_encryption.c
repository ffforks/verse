#if !defined(V_GENERATE_FUNC_MODE)
#include "verse.h"
#include "v_pack.h"
#include "v_encryption.h"

void v_e_encrypt_data_key_generate(uint8 *to) /* possibly the worst key gen ever */
{
	static unsigned int counter = 0;
	unsigned int i, temp;
	for(i = 0; i < V_ENCRYPTION_DATA_KEY_SIZE; i++) 
	{
		counter++;
		temp = (counter << 13) ^ counter;
		temp = (temp * (temp * temp * 15731 + 789221) + 1376312589) & 0x7fffffff;
		to[i] = temp % 256;
	}
}

void v_e_encrypt_command(uint8 *packet, unsigned int packet_length, uint8 *command, unsigned int command_length, uint8 *key)
{
	unsigned int i, pos;
	vnp_raw_unpack_uint32(packet, &pos);
	pos = key[pos % V_ENCRYPTION_DATA_KEY_SIZE] + packet_length - 4;
	for(i = 0; i < command_length; i++)
		packet[packet_length + i] = command[i] ^ key[(i + pos) % V_ENCRYPTION_DATA_KEY_SIZE];
}

void v_e_dencrypt_data_packet(uint8 *from, uint8 *to, unsigned int data_length, uint8 *key)
{
	unsigned int i, pos;
	vnp_raw_unpack_uint32(from, &pos);
	pos = key[pos % V_ENCRYPTION_DATA_KEY_SIZE] - 4;
	for(i = 0; i < 4; i++)
		to[i] = from[i];
	for(i = 4; i < data_length; i++)
		to[i] = from[i] ^ key[(i + pos) % V_ENCRYPTION_DATA_KEY_SIZE];
}

void v_e_encrypt_login(uint8 *data, unsigned int data_length, uint8 *key)
{

}

#endif