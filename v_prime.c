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

#define CYCLES		2 /*200*/
#define SIZE		100
#define MAX_TESTS	10000

/*
 * square-and-multiply, enl s. 170 in Stinson.
 */
static VBigNum sqr_and_mul(VBigNum x, VBigNum c, VBigNum n)
{
	int i, l;
	VBigNum bin, rop;

	rop = v_bignum_new_one();
	bin = v_bignum_add_ushort(v_bignum_new_zero(), 2);
	for(l = V_BIGNUM_BITS - 1; l != -1 && !v_bignum_bit_test(c, l); l--);

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

static int miller_rabin(VBigNum n)
{
	VBigNum a, b, m;
	VBigNum q, r;
	int k;

	m = v_bignum_add_ushort(n, 1);

	do
	{
		a = v_bignum_new_random();
	}
	while(v_bignum_eq(a, v_bignum_new_zero()));

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
	b = sqr_and_mul(a, m, n);
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

int v_prime_test(VBigNum q)
{
	int	i;

	for(i = 0; i < CYCLES; i++)
	{
		if(!miller_rabin(q))
			return FALSE;
	}
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
