

#define V_ENCRYPTION_LOGIN_KEY_SIZE 256
#define V_ENCRYPTION_LOGIN_KEY_FULL_SIZE 256 * 3
#define V_ENCRYPTION_LOGIN_KEY_HALF_SIZE 256 * 2


#define V_ENCRYPTION_LOGIN_PUBLIC_START 256 * 0
#define V_ENCRYPTION_LOGIN_N_START 256 * 1
#define V_ENCRYPTION_LOGIN_PRIVATE_START 256 * 2


#define V_ENCRYPTION_DATA_KEY_SIZE 256

extern void v_e_encrypt_data_key_generate(uint8 *to);
extern void v_e_encrypt_command(uint8 *packet, unsigned int packet_length, const uint8 *command, unsigned int command_length, uint8 *key);
extern void v_e_dencrypt_data_packet(const uint8 *from, uint8 *to, unsigned int data_length, uint8 *key);

extern void v_e_create_key(uint8 *private_key, uint8 *public_key, uint8 *n);
extern void v_e_encrypt(uint8 *output, uint8 *data, uint8 *key, uint8 *n);
