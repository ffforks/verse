/*
 * Program to generate primes of the form p = 2 * q + 1,
 * where p and q are both primes.
 *
 * Written by Pontus Nyman <f97-pny@nada.kth.se>.
 */

#include <stdio.h>
#include <stdlib.h>
#include "v_bignum.h"
#include "verse_header.h"

#define CYCLES		200
#define SIZE		100
#define MAX_TESTS	10000

/* Globals. */
/*
gmp_randstate_t rand_state;
mpz_t plus_1;
mpz_t minus_1;
mpz_t p;
char *str;
*/

#if 0
//int main()
//{
//	VBigNum q, p;
//	int i;

/*	mpz_init(p);
	mpz_init(q);

	mpz_init_set_ui(plus_1,1);
	mpz_init_set_si(minus_1,-1);
*/

	// generera stort udda tal.
//	q = v_bignum_new_zero();
//	while(v_bignum_eq(q, v_bignum_new_zero()))
//		q = v_bignum_new_random();
/*
	mpz_urandomb(q, rand_state, SIZE);  
	mpz_setbit(q, SIZE-1); // q ska vara udda...
	mpz_setbit(q, 0); // ...och tillräckligt stort.
*/
/*	i = 0;
	while(i < MAX_TESTS)
	{
		if(miller_rabin(q)) 
		{
			p = v_bignum_mul_ushort(q, 2);
			p = v_bignum_add_ushort(p, 1);
			if(miller_rabin(p))
			{
				return TRUE;
			}
		}
		q = v_bignum_mul_ushort(q, 2);
		i++;
	}
	return FALSE;*/
//}
#endif


/*
 * square-and-multiply, enl s. 170 in Stinson.
 */
static VBigNum v_sqr_and_mul(VBigNum x, VBigNum c, VBigNum n)
{
	int i, l;
	VBigNum bin, rop;

	rop = v_bignum_new_one();
	bin = v_bignum_add_ushort(v_bignum_new_zero(), 2);
	for(l = V_BIGNUM_BITS - 1; l != -1 && !v_bignum_bit_test(c, l); l--);

/*	while(v_bignum_gte(c, bin))
	{
#if 0
		bin = v_bignum_mul_ushort(bin, 2);
#endif
		bin = v_bignum_bit_shift_left(bin, 1);
	    l++;
		printf("r = %u \n", l);
	}*/
/*	printf("r = %u \n", l);*/
	for(i = l - 1; i >= 0; i--)
	{
		rop = v_bignum_mul(rop, rop);	/* z = z^2 */
		rop = v_bignum_mod(rop, n);	/* z = z mod n */
		if(v_bignum_bit_test(c, i) == 1)
		{
			rop = v_bignum_mul(rop, x);
			rop = v_bignum_mod(rop, n);
		}
/*		printf("i = %u \n", i);*/
	}
	return rop;
}

int v_miller_rabin(VBigNum n)
{
	VBigNum a, b, m;
	VBigNum q, r;
	int k;

/*	mpz_init(a);
	mpz_init(b);
	mpz_init(m);
	mpz_init(q);
	mpz_init(r);
*/
	m = v_bignum_add_ushort(n, 1);

	a = v_bignum_new_random();
	while(v_bignum_eq(a, v_bignum_new_zero()))
		a = v_bignum_new_random();

	/* Compute k och m according to n - 1 = 2^k * m. */
	k = 0;
	q = v_bignum_div(m, v_bignum_new_ushort(2), &r);
	while(v_bignum_eq(r, v_bignum_new_zero()))
	{
		m = q;
		k++;
		q = v_bignum_div(m, v_bignum_new_ushort(2), &r);
	}

	/* Test for prime. */

	b = v_sqr_and_mul(a, m, n);

	if(v_bignum_eq(v_bignum_mod(b, n), v_bignum_mod(v_bignum_new_one(), n)))
		return TRUE;

	while(k > 0)
	{
		if(v_bignum_eq(v_bignum_mod(b, n), v_bignum_mod(v_bignum_sub_ushort(v_bignum_new_zero(), 1), n)))
			return TRUE;
		else
		{
			b = v_bignum_mul(b, b);
			b = v_bignum_mod(b, n);
		}
		k--;
	}
	return FALSE;
}


/*
 * Primtalstestare som kör Miller-Rabins test CYCLES antal ggr.
 */

 
/*
int prime_test(VBigNum n)
{
  int i;
  
  for (i = 0; i < CYCLES; i++) {
    if (!v_miller_rabin(n))
      return 0;
  }
  return 1;
}
*/


/*
int main()
{
	VBigNum q, p;
	int i;

	q = v_bignum_new_zero();
	while(v_bignum_eq(q, v_bignum_new_zero()))
		q = v_bignum_new_random();
	i = 0;
	while(i < MAX_TESTS)
	{
		if(v_miller_rabin(q)) 
		{
			p = v_bignum_mul_ushort(q, 2);
			p = v_bignum_add_ushort(p, 1);
			if(v_miller_rabin(p))
			{
				return TRUE;
			}
		}
		q = v_bignum_mul_ushort(q, 2);
		i++;
	}
	return FALSE;
}*/

int v_prime_test(VBigNum q)
{
/*	VBigNum p;*/
	int i;
/*	i = 0;
/*	while(i < MAX_TESTS)
	{
		if(v_miller_rabin(q)) 
		{
			p = v_bignum_mul_ushort(q, 2);
			p = v_bignum_add_ushort(p, 1);
			if(v_miller_rabin(p))
			{
				return FALSE;
				exit(0);
			}
		}
		q = v_bignum_mul_ushort(q, 2);
		i++;
	}*/
	for(i = 0; i < CYCLES; i++)
	    if(!v_miller_rabin(q))
			return FALSE;
	return TRUE;
}

VBigNum v_prime_new_random(void)
{
	VBigNum prime;

	prime = v_bignum_new_random();
	while(v_bignum_eq(prime, v_bignum_new_zero()))
	{
		prime = v_bignum_new_random();
	}
	while(!v_prime_test(prime))
	{
		printf("not a prime....\n");
		prime = v_bignum_new_zero();
		while(v_bignum_eq(prime, v_bignum_new_zero()))
			prime = v_bignum_new_random();
	}
	printf("prime!!!!\n");
	return prime;
}
