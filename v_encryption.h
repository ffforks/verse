
#define V_ENCRYPTION_LOGIN_KEY_SIZE 512
#define V_ENCRYPTION_DATA_KEY_SIZE 256

extern void v_e_encrypt_data_key_generate(uint8 *to);
extern void v_e_encrypt_command(uint8 *packet, unsigned int packet_length, const uint8 *command, unsigned int command_length, uint8 *key);
extern void v_e_dencrypt_data_packet(const uint8 *from, uint8 *to, unsigned int data_length, uint8 *key);
extern void v_e_encrypt_login(uint8 *data, unsigned int data_length, uint8 *key);
