/*
 * Program to generate primes of the form p = 2 * q + 1,
 * where p and q are both primes.
 *
 * Originally written by Pontus Nyman <f97-pny@nada.kth.se>,
 * ported to Verse's bignums and rewritten from scratch by
 * Emil Brink.
 */

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "v_bignum.h"
#include "v_encryption.h"
#include "verse_header.h"

#define	BITS		(CHAR_BIT * V_ENCRYPTION_LOGIN_KEY_SIZE)

#define CYCLES	10	/* Number of times to apply Miller-Rabin test. */

/* Test divisibility of <n> against table of small known primes. Returns 1 if n looks prime, 0 if it IS not. */
static int quick_filter(const VBigDig *n)
{
	VBigDig VBIGNUM(m, 16), VBIGNUM(tmp, BITS / 2);
	const unsigned int	prime[] = { 3, 5, 7, 11, 13, 17, 19, 23, 39, 31, 37, 41, 43, 47, 53 };
	int	i;

	for(i = 0; i < sizeof prime / sizeof *prime; i++)
	{
		v_bignum_set_bignum(tmp, n);
		v_bignum_set_digit(m, prime[i]);
		v_bignum_mod(tmp, m);
		if(v_bignum_eq_zero(tmp))
		{
			printf("quick-killed using %u\n", prime[i]);
			return 0;
		}
	}
	return 1;
}

/* The Miller-Rabin primality test. Returns 1 if the candidate looks prime, 0 if
 * it IS NOT prime. Assumes that n is BITS / 2 bits, so that its square fits in BITS.
*/
static int miller_rabin(const VBigDig *n)
{
	int	i, k;
	VBigDig	VBIGNUM(a, BITS / 2), VBIGNUM(d, BITS), VBIGNUM(nmo, BITS / 2), VBIGNUM(x, BITS);

/*	printf("Miller-Rabin testing: ");
	v_bignum_print_hex_lf(n);
*/
	/* Pick a "witness", a number in the [1, n) range. */
	v_bignum_set_random(a);
	v_bignum_mod(a, n);
/*	printf("witness: ");
	v_bignum_print_hex_lf(a);
*/
	v_bignum_set_one(d);
	v_bignum_set_bignum(nmo, n);
	v_bignum_sub_digit(nmo, 1);	/* nmo = n - 1 (say it). */
	k = v_bignum_bit_msb(nmo);
	for(i = k; i >= 0; i--)
	{
		v_bignum_set_bignum(x, d);
		v_bignum_mul(d, d);
		v_bignum_mod(d, n);
		if(v_bignum_eq_one(d) && !v_bignum_eq_one(x) && !v_bignum_eq(x, nmo))
			return 0;	/* Composite found. */
		if(v_bignum_bit_test(nmo, i))
		{
			v_bignum_mul(d, a);
			v_bignum_mod(d, n);
		}
	}
	return v_bignum_eq_one(d);	/* It might be prime. */
}

/* Test q for primality, returning 1 if it seems prime, 0 if it certainly IS not. */
int v_prime_test(const VBigDig *q)
{
	int	i;

	if(!quick_filter(q))
		return 0;

	for(i = 0; i < CYCLES; i++)
	{
		if(!miller_rabin(q))
			return 0;
	}
	return 1;
}

void v_prime_set_random(VBigDig *x)
{
	int	count = 1, bits = v_bignum_bit_size(x);

	/* Create initial candidate, making sure it's both odd and non-zero. */
	v_bignum_set_random(x);
	/* Set topmost two bits, makes sure products are big. */
	v_bignum_bit_set(x, bits - 1);
	v_bignum_bit_set(x, bits - 2);
	/* Set lowermost bit, makes sure it is odd (better prime candidate that way). */
	v_bignum_bit_set(x, 0);

	while(!v_prime_test(x))
	{
		printf(" test failed, retrying count=%d...\n", ++count);
		v_bignum_add_digit(x, 2);	/* Simple step to next candidate. */
		if(v_bignum_eq_one(x))
		{
			printf("prime generator overflow, not good\n");
		}
		printf("candidate: ");
		v_bignum_print_hex_lf(x);
	}
	printf("\n***FOUND PRIME: ");
	v_bignum_print_hex_lf(x);
}

/* Big (small?) primes from <http://www.utm.edu/research/primes/lists/small/small3.html#300>. */
void v_prime_set_table(VBigDig *x, unsigned int i)
{
	if(i == 0)
		v_bignum_set_string_hex(x, "0x4DF72D07B4B71C8DACB6CFFA954F8D88254B6277099308BAF003FAB73227F34029643B5A263F66E0D3C3FA297EF71755EFD53B8FB6CB812C6BBF7BCF179298BD9947C4C8B14324140A2C0F5FAD7958A69050A987A6096E9F055FB38EDF0C5889ECA4A0CFA99B45FBDEEE4C696B328DDCEAE4723945901EC025076B12B");
	else if(i == 1)
		v_bignum_set_string_hex(x, "0xCB50E82A8583F44EE0025942E7362991B24E12663A0DDC234A57B0F7B4FF7B025BF5A6707DEDC2898E70B739042C95A996283DFFDF67558768784553C61E302E8812BC90F0BB0696870CFB910B560CEFED8D99BBF7A00B31CCDBD56F3594E5A653CFD127D2167B13119E5C45C3F76B4E3D904A9BC0CBB43C33AA7F23B");
	else if(i == 2)
		v_bignum_set_string_hex(x, "0x7A364AB3DE755F924642BD5273524234F78395DA1ED9098F39AF4FE248288B0CB7F1C27214588969479D7DC9F0D327B5544DD4C095AA1FA271DF421FE9EE460855CC8423D223E2C85DC793F6BABDCA7FC804EA1F408F867DB053BFD98C45085EA5D805C78D2863BACDFCAF4C6147EBB74A9056045074785714C0B84ED");
	else if(i == 3)
		v_bignum_set_string(x, "250556952327646214427246777488032351712139094643988394726193347352092526616305469220133287929222242315761834129196430398011844978805263868522770723615504744438638381670321613949280530254014602887707960375752016807510602846590492724216092721283154099469988532068424757856392563537802339735359978831013");
	else if(i == 4)
		v_bignum_set_string(x, "290245329165570025116016487217740287508837913295571609463914348778319654489118435855243301969001872061575755804802874062021927719647357060447135321577028929269578574760547268310055056867386875959045119093967972205124270441648450825188877095173754196346551952542599226295413057787340278528252358809329");
	else
		v_bignum_set_string(x, "65537");	/* It ain't big, but it's prime. */
}
