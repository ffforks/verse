#if !defined(V_GENERATE_FUNC_MODE)
#include "verse.h"
#include "v_pack.h"
#include "v_bignum.h"
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


VBigNum v_e_math_inv(VBigNum a, VBigNum b)
{
	VBigNum t, t_null, a_null, q, r, temp;

	a_null = a;
	t_null = v_bignum_new_zero();
	t = v_bignum_new_one();
	q = v_bignum_div(a, b, NULL);
	r = v_bignum_sub(a, b);
	while(!v_bignum_eq(r, v_bignum_new_zero()))
	{
		temp = v_bignum_mod(v_bignum_sub(t_null, v_bignum_mul(q, t)), a);
		t_null = t;
		t = temp;
		a_null = b;
		b = r;
		q = v_bignum_div(a_null, b, NULL);
		q = v_bignum_sub(r, v_bignum_mul(q, b));
	}
	return t;
}

VBigNum v_e_math_compute_gcd(VBigNum a, VBigNum b)
{
	VBigNum a_null, b_null, t_null, s_null, s, t, q, r, temp;
	a_null = a;	
	b_null = b;
	t_null = v_bignum_new_one();
	s_null = v_bignum_new_one();
	s = v_bignum_new_zero();
	q = v_bignum_div(a_null, b_null, NULL);
	r = v_bignum_sub(a_null, v_bignum_mul(q, b_null));
	t = v_bignum_new_one();
	while(!v_bignum_eq(r, v_bignum_new_zero()))
	{
		temp = v_bignum_sub(t_null, v_bignum_mul(q, t));
		t_null = t;
		t = temp;
		temp = v_bignum_sub(s_null, v_bignum_mul(q, s));
		s_null = s;
		s = temp;
		a_null = b_null;
		b_null = r;
		q = v_bignum_div(a_null, b_null, NULL);
		r = v_bignum_sub(a_null, v_bignum_mul(q, b_null));
	}
	return r;
}

VBigNum v_e_math_gen_large_prime(unsigned int a)
{
	return v_bignum_new_one();
}

void v_e_create_key_real(unsigned int sead_a, unsigned int sead_b, unsigned int sead_c, VBigNum *public_key, VBigNum *private_key, VBigNum *n)
{
	VBigNum p, q, b, phi;
	p = v_e_math_gen_large_prime(sead_a);
	q = v_e_math_gen_large_prime(sead_b);
	*n = v_bignum_mul(p, q); /* known to both needed by both */
	phi = v_bignum_mul(v_bignum_sub_ushort(p, 1), v_bignum_sub_ushort(q, 1));

	b = v_bignum_new_random();
/*	while(!(b > 1 && b < phi))*/
		b = v_bignum_new_random();
	
	while(!v_bignum_eq(v_bignum_new_one(), v_e_math_compute_gcd(b, phi)))
	{
		b = v_bignum_add_ushort(b, 1);
/*		while(!(b > 1 && b < phi))*/
			b = v_bignum_new_random();
			
	}
	*public_key = b;
	*private_key = v_e_math_inv(b, phi);
}

VBigNum v_e_encrypt_real(VBigNum *data, VBigNum *key, VBigNum *n)
{
	VBigNum z;
	unsigned int i;
	z = v_bignum_new_one();
	for(i = V_BIGNUM_BITS; i > 1; i--)
	{
		z = v_bignum_mod(v_bignum_mul(z, z), *n);
		if(v_bignum_bit_test(*key, i))
			z = v_bignum_mul(z, v_bignum_mod(*data, *n));
	}
	return z;
}

void v_e_create_key(uint8 *private_key, uint8 *public_key, uint8 *n)
{
	static unsigned int random = 6;
	unsigned int i;

	random++;
	for(i = 0; i < V_BIGNUM_BITS / 8; i++)
	{
		public_key[i] = ((i + random) % 256); 
		private_key[i] = 256 - public_key[i];
		n[i] = random;
	}
}

void v_e_encrypt(uint8 *output, uint8 *data, uint8 *key, uint8 *n)
{
	unsigned int i;
	for(i = 0; i < V_BIGNUM_BITS / 8; i++)
		output[i] = data[i] + key[i];
}
