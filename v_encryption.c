/*
 * 
*/

#if !defined(V_GENERATE_FUNC_MODE)

#include <stdio.h>

#include "verse.h"
#include "v_pack.h"
#include "v_bignum.h"
#include "v_encryption.h"

extern VBigNum	v_prime_new_random(void);

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

void v_e_encrypt_command(uint8 *packet, unsigned int packet_length, const uint8 *command, unsigned int command_length, uint8 *key)
{
	unsigned int i, pos;
	vnp_raw_unpack_uint32(packet, &pos);
	pos = key[pos % V_ENCRYPTION_DATA_KEY_SIZE] + packet_length - 4;
	for(i = 0; i < command_length; i++)
		packet[packet_length + i] = command[i] ^ key[(i + pos) % V_ENCRYPTION_DATA_KEY_SIZE];
}

void v_e_dencrypt_data_packet(const uint8 *from, uint8 *to, unsigned int data_length, uint8 *key)
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

/* From Knuth. */
VBigNum v_e_math_inv(VBigNum u, VBigNum v)
{
	VBigNum	u1, u3, v1, v3, t1, t3, q, w, inv;
	int	iter = 1;

	u1 = v_bignum_new_one();
	u3 = u;
	v1 = v_bignum_new_zero();
	v3 = v;

	while(!v_bignum_eq_zero(v3))
	{
		q = v_bignum_div(u3, v3, &t3);
		w = v_bignum_mul(q, v1);
		t1 = v_bignum_add(u1, w);
	
		u1 = v1;
		v1 = t1;
		u3 = v3;
		v3 = t3;
		iter = -iter;
	}
	if(iter < 0)
		inv = v_bignum_sub(v, u1);
	else
		inv = u1;
	return inv;
}


VBigNum v_e_math_compute_gcd(VBigNum u, VBigNum v)
{
	VBigNum	g = v_bignum_new_one();

	while(!v_bignum_bit_test(u, 0) && !v_bignum_bit_test(v, 0))
	{
		u = v_bignum_bit_shift_right(u, 1);
		v = v_bignum_bit_shift_right(v, 1);
		g = v_bignum_bit_shift_left(g, 1);
	}
	while(!v_bignum_eq_zero(u))
	{
		if(!v_bignum_bit_test(u, 0))
			u = v_bignum_bit_shift_right(u, 1);
		else if(!v_bignum_bit_test(v, 0))
			v = v_bignum_bit_shift_right(v, 1);
		else
		{
			VBigNum	t = v_bignum_sub(u, v);
			if(v_bignum_bit_test(t, V_BIGNUM_BITS - 1))	/* We need |u-v|, so if negative, invert. */
			{
				t = v_bignum_not(t);
				t = v_bignum_add_ushort(t, 1);
			}
			t = v_bignum_bit_shift_right(t, 1);
			if(!v_bignum_gte(u, v))
				v = t;
			else
				u = t;
		}
	}
	return v_bignum_mul(g, v);
}

extern VBigNum v_get_prime();

void v_e_create_key_real(VBigNum *public_key, VBigNum *private_key, VBigNum *n)
{
	VBigNum p, q, phi, e, d;

	printf("find prime p\n");
	p = v_prime_new_random();
	printf("find prime q\n");
	q = v_prime_new_random();
	printf("done, computing key\n");
	phi = v_bignum_mul(v_bignum_sub_ushort(p, 1), v_bignum_sub_ushort(q, 1));
	printf("phi: ");
	v_bignum_print_hex(phi);
	e = v_bignum_new_string("0x10001");
	*public_key  = e;
	printf("      e: ");
	v_bignum_print_hex(e);
	*private_key = v_e_math_inv(e, phi);
	printf("e's inv: ");
	v_bignum_print_hex(*private_key);
	printf("msb of inv: %u\n", v_bignum_bit_msb(*private_key));
	printf("test of inverse: ");
	v_bignum_print_hex(v_bignum_mod(v_bignum_mul(e, *private_key), phi));
	printf("gcd: ");
	v_bignum_print_hex(v_e_math_compute_gcd(e, phi));
	*n = v_bignum_mul(p, q);
	printf("key-creation finished\n");
}

VBigNum v_e_encrypt_real(VBigNum *data, VBigNum *key, VBigNum *n)
{
	VBigNum z;
	unsigned int i;
/*	typedef struct
	unsigned short	data[V_BIGNUM_BITS / (CHAR_BIT * sizeof (unsigned short))];
*/
	z = v_bignum_new_one();
	for(i = V_BIGNUM_BITS; i > 1; i--)
	{
		z = v_bignum_mod(v_bignum_mul(z, z), *n);
		if(v_bignum_bit_test(*key, i))
			z = v_bignum_mul(z, v_bignum_mod(*data, *n));
	}
/*	for(i = 0; i < V_BIGNUM_BITS / (CHAR_BIT * sizeof (unsigned short)); i++)*/
	return z;
}

void v_e_create_key(uint8 *private_key, uint8 *public_key, uint8 *n)
{
	printf("generating key\n");
	v_e_create_key_real(private_key, public_key, n);
	printf(" done!\n");
}

void v_e_encrypt(uint8 *output, uint8 *data, uint8 *key, uint8 *n)
{
	unsigned int i;
	for(i = 0; i < V_BIGNUM_BITS / 8; i++)
		output[i] = data[i] + key[i];
}

void v_encrypt_test()
{
	uint8 key[768];

	printf("e\n");
	v_e_create_key_real(&key[0], &key[256], &key[512]);
/*VBigNum v_e_encrypt_real(VBigNum *data, VBigNum *key, VBigNum *n)*/
}
