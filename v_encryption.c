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


VBigNum v_e_math_inv_pontus(VBigNum a, VBigNum b)
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
		printf("inv-step, r=");
		v_bignum_print_hex(r);
	}
	return t;
}

struct inv_state
{
	VBigNum	d, x, y;
};

static struct inv_state inv_step(VBigNum a, VBigNum b)
{
	struct inv_state	prim, next;
	VBigNum			q, r;

	if(v_bignum_eq_zero(b))
	{
		struct inv_state	r = { a, v_bignum_new_one(), v_bignum_new_zero() };
		return r;
	}
	q      = v_bignum_div(a, b, &r);
	prim   = inv_step(b, r);
	printf("q=");
	v_bignum_print_hex(q);
	next.d = prim.d;
	next.x = prim.y;
/*	printf("subtracting from: ");
	v_bignum_print_hex(prim.x);
	printf("subtracting: ");
	v_bignum_print_hex(q);
	printf("times: ");
	v_bignum_print_hex(prim.y);
*/	printf("q*yp: ");
	v_bignum_print_hex(v_bignum_mul(q, prim.y));
	next.y = v_bignum_sub(prim.x, v_bignum_mul(q, prim.y));
	printf("step: ");
	v_bignum_print_hex(next.x);
	printf("and: ");
	v_bignum_print_hex(next.y);

	return next;
}

VBigNum v_e_math_inv3(VBigNum a, VBigNum b)
{
	struct inv_state s = inv_step(a, b);

#if 0
	if(v_bignum_bit_test(s.x, V_BIGNUM_BITS - 1))
	{
		printf("Sign-inverting inverse\n");
		s.x = v_bignum_add(a, b);
/*		s.x = v_bignum_add_ushort(v_bignum_not(s.x), 1);*/
	}
#endif
	return s.x;
}

/* Compute multiplicative inverse of a modulo b. This is the Extended Euclidian algorithm. */
VBigNum v_e_math_inv4(VBigNum a, VBigNum b)
{
	VBigNum	uprev = a, u = b, xprev = v_bignum_new_zero(), x = v_bignum_new_one(),
		yprev = v_bignum_new_one(), y = v_bignum_new_zero();

	while(!v_bignum_eq_zero(u))
	{
		VBigNum	q = v_bignum_div(uprev, u, NULL), tmp;

		printf("q=");
		v_bignum_print_hex(q);
		tmp = v_bignum_sub(uprev, v_bignum_mul(u, q)); uprev = u; u = tmp;
		tmp = v_bignum_sub(xprev, v_bignum_mul(x, q)); xprev = x; x = tmp;
		printf("xprev=");
		v_bignum_print_hex(xprev);
/*		tmp = v_bignum_sub(yprev, v_bignum_mul(y, q)); yprev = y; y = tmp;*/
	}
	return xprev;
}

VBigNum v_e_math_inv5(VBigNum a, VBigNum b)
{
	VBigNum	p = v_bignum_new_one(), q = v_bignum_new_zero(), r = q, s = p, k, c, new_r, new_s;

	while(!v_bignum_eq_zero(b))
	{
		k = v_bignum_div(a, b, &c);
		v_bignum_print_hex(k);
		a = b;
		b = c;
		new_r = v_bignum_sub(p, v_bignum_mul(k, r));
		new_s = v_bignum_sub(q, v_bignum_mul(k, s));
		p = r;
		q = s;
		r = new_r;
		s = new_s;
		v_bignum_print_hex(p);
	}
	return p;
}

/* From Knuth, via BigDigits. */
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

/* Wikipedia. */
VBigNum v_e_math_inv2(VBigNum a, VBigNum b)
{
	VBigNum	p, q, r, s, c, quot, new_r, new_s;

	p = v_bignum_new_one();
	q = v_bignum_new_zero();
	r = q;
	s = p;

	while(!v_bignum_eq_zero(b))
	{
		quot = v_bignum_div(a, b, &c);

		a = b;
		b = c;
		new_r = v_bignum_sub(p, v_bignum_mul(quot, r));
		new_s = v_bignum_sub(q, v_bignum_mul(quot, s));
		p = r;
		q = s;
		r = new_r;
		s = new_s;
		printf("inv-step, r=");
		v_bignum_print_hex(r);
	}
	return p;	/* Whatever ;/ */
}

VBigNum v_e_math_compute_gcd_pontus(VBigNum a, VBigNum b)
{
	VBigNum a_null, b_null, t_null, s_null, s, t, q, r, temp;

	printf("GCD begin\n");
	a_null = a;	
	b_null = b;
	t_null = v_bignum_new_one();
	s_null = v_bignum_new_one();
	s = v_bignum_new_zero();
	q = v_bignum_div(a_null, b_null, NULL);
	r = v_bignum_sub(a_null, v_bignum_mul(q, b_null));
	t = v_bignum_new_one();
	while(!v_bignum_eq_zero(r))
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
	printf(" done\n");
	return r;
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
/*
VBigNum v_e_math_gen_large_prime(unsigned int a)
{
	return v_get_prime();
}*/

/* Comparison, returns a >= b. !(a < b) */
/*
void v_e_create_key(uint sead_a, uint sead_b, uint sead_c, VEValue *public_key,
VEValue *private_key, VEValue *n)
{
        VEValue p, q, n, phi;
        p = v_e_math_gen_large_prime(sead_a);
        q = v_e_math_gen_large_prime(sead_b);
        n = v_e_math_mult(p, q);
        phi = v_e_math_mult(v_e_math_subtract_int(p, 1), v_e_math_subtract_int(q, 1));

        b = v_e_math_rand(VEValue sead_c, phi);
        while(v_e_math_set(1) != v_e_math_compute_gcd(b, phi))
        {
                b = v_e_math_add_int(b, 1);
                b = v_e_math_mod(b, phi);
        }
        a = v_e_math_inv(b, phi);
        public_key[0] = n;
        public_key[1] = b;
        private_key[0] = a;
}
*/

void v_e_create_key_real(VBigNum *public_key, VBigNum *private_key, VBigNum *n)
{
	VBigNum p, q, phi, e, d;

/*	{
		VBigNum	a, b, inv;

		a = v_bignum_new_string("0x10001");
		b = v_bignum_new_string("0x6918EF903A166FFD2FD41617671D229A9BB131C9A3C9F68FEB1"
					 "A2D424126FCC3417A43AEC8BD31EF4BA6CB3293B1E8FE8BB45A9B7"
					 "965B716CEDE116F7610334209170AD9CA9963676FFC908403428DF"
					 "7572DE9E04455FFF901CB8D7C03EB5963BB735F898B40B9DBD56C8"
					 "9DEA573D779242817D9E9EADC8DDE1E13708E3F98BDFB2865C284B"
					 "9A5EF910F227A99BD8B17CC602D22D6A8C2DD866CB75C83F03ADFE"
					 "1C74DDE276DF94CB06136E76DF3F456E446A7153B3B80D2C5866FA"
					 "0552BD099A39AD2355FC0A32D40C592CCF199C444C2A06662097D1"
					 "D48819FEDC4242E49636869B045DAB9336F06CE3FF7F4FE94028CE"
					 "5068DEEC5900AF5F7FB558BFF4658");
		printf("a=");
		v_bignum_print_hex(a);
		printf("b=");
		v_bignum_print_hex(b);
		inv = v_e_math_inv(a, b);
		printf("inv=");
		v_bignum_print_hex(inv);
		printf("TEST: ");
		v_bignum_print_hex(v_bignum_mod(v_bignum_mul(inv, a), b));
		
		inv = v_e_math_inv2(a, b);
		printf("inv2=");
		v_bignum_print_hex(inv);
		exit(1);
	}
*/
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

#if 0
	static unsigned int random = 6;
	unsigned int i;
	random++;
	for(i = 0; i < V_BIGNUM_BITS / 8; i++)
	{
		public_key[i] = ((i + random) % 256); 
		private_key[i] = 256 - public_key[i];
		n[i] = random;
	}
#endif
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
