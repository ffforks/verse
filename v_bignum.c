/*
 * Routines for working with big (2048-bit) unsigned integers, and
 * doing common maths operations on them. Written by Emil Brink.
 * 
 * Part of the Verse core, see license details elsewhere.
 * 
 * All "big numbers" have the same length, V_BIGNUM_BITS bits to be
 * specific (2,048 at time of writing). They are represented as a
 * vector of unsigned shorts, x, with x[0] being the least significant
 * bits in host order. This means x[0] & 1 tests bit zero of the
 * bignum, and so on. Verse's uint16 and uint32 types are *not* used,
 * since there is no need to limit the bits. If your machine has 32-
 * bit shorts and 64-bit ints, this code should cope.
 * 
 * By using unsigned shorts, which are assumed to be half the size of
 * an unsigned int, we can easily do intermediary steps in int-sized
 * variables, and thus get space for manual carry-management.
 * 
 * The original focus for this code has been, um, speed of implementation
 * first, which (to me) means simplicity over performance in very many
 * cases. I'm not a numerical kind of guy. These routines were written
 * from scratch in less than three days, which did not leave much time
 * to meditate over FFTs and Karatsubas. Reasonable improvements are
 * of course welcome, although this code *should* not be a bottleneck.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v_bignum.h"

#define	V_BIGNUM_STANDALONE		/* Define to include main(), for testing. */
#undef	V_BIGNUM_STANDALONE

/* ----------------------------------------------------------------------------------------- */

VBigNum v_bignum_new_zero(void)
{
	VBigNum	a;

	memset(&a.x, 0, sizeof a.x);
	return a;
}

VBigNum v_bignum_new_one(void)
{
	VBigNum	a;

	memset(a.x, 0, sizeof a.x);
	a.x[0] = 1;
	return a;
}

VBigNum v_bignum_new_ushort(unsigned short low)
{
	VBigNum	a;

	memset(a.x, 0, sizeof a.x);
	a.x[0] = low;
	return a;
}

/* Create a new bignum by extracting bits [msb-(bits-1),msb] from <src>. The new number is right-adjusted. */
VBigNum v_bignum_new_bignum(VBigNum a, unsigned int msb, unsigned int bits)
{
	VBigNum	n = v_bignum_new_zero();
	int	i;

	/* Extract the bits by simply testing source bits. Could be optimized. */
	for(i = 0; i < bits; i++)
	{
		if(v_bignum_bit_test(a, msb - (bits - 1) + i))
			n.x[i / (CHAR_BIT * sizeof *n.x)] |= 1 << (i % (CHAR_BIT * sizeof *n.x));
	}
	return n;
}

VBigNum v_bignum_new_string(const char *string)
{
	size_t	len = strlen(string);
	int	i, here, d;
	VBigNum	x;

	if(strncmp(string, "0x", 2) == 0)
	{
		string += 2;
		len -= 2;
	}
	x = v_bignum_new_zero();
	for(i = len - 1, d = 0; i >= 0; i--, d++)
	{
		here = toupper(string[i]);
		if(!isxdigit(here))
			break;
		here -= isdigit(here) ? '0' : 'A' - 10;
		x.x[d >> 2] |= here << 4 * (d & 3);
	}
	return x;
}

VBigNum v_bignum_new_random(void)
{
	VBigNum	a;
	int	i;

	for(i = 0; i < sizeof a.x / sizeof *a.x; i++)
		a.x[i] = rand();
	return a;
}

/* ----------------------------------------------------------------------------------------- */

void v_bignum_print_hex(VBigNum a)
{
	int	i;

	printf("0x");
	for(i = 0; i < sizeof a.x / sizeof *a.x; i++)
		printf("%04X", a.x[sizeof a.x / sizeof *a.x - i - 1]);
	printf("\n");
}

/* ----------------------------------------------------------------------------------------- */

/* Returns value of bit <i> in <a>. */
int v_bignum_bit_test(VBigNum a, unsigned int i)
{
	if(i < V_BIGNUM_BITS)
		return (a.x[i / (CHAR_BIT * sizeof *a.x)] & (1 << (i % (CHAR_BIT * sizeof *a.x)))) != 0;
	return 0;
}

/* Return index of most significant bit, or -1 if a is zero. */
int v_bignum_bit_msb(VBigNum a)
{
	int	i, msb = V_BIGNUM_BITS - 1;

	for(i = (sizeof a.x / sizeof *a.x) - 1; i >= 0; i--, msb -= CHAR_BIT * sizeof *a.x)
	{
		if(a.x[i] != 0)	/* Found word containing bits? */
		{
			unsigned int	mask;

			for(mask = 1 << (CHAR_BIT * sizeof *a.x - 1); mask != 0; mask >>= 1, msb--)
				if(a.x[i] & mask)
					return msb;
			/* This can't happen. */
		}
	}
	return -1;
}

/* Return a << count. */
VBigNum v_bignum_bit_shift_left(VBigNum a, unsigned int count)
{
	unsigned int	x, carry;
	int		i;

	if(count == 0)
		return a;
	if(count >= 16)
	{
		unsigned int	places = count / 16;

		for(i = (sizeof a.x / sizeof *a.x) - 1; i >= places; i--)
			a.x[i] = a.x[i - places];
		for(; i >= 0; i--)
			a.x[i] = 0;
		count -= places * 16;
		if(count == 0)
			return a;
	}
	for(i = carry = 0; i < (sizeof a.x / sizeof *a.x) - 1; i++)
	{
		x = a.x[i];
		x <<= count;
		x |= carry;
		carry = x >> 16;
		a.x[i] = x;
	}
	return a;
}

/* Return a >> count, i.e. shift a by count bits to the right (towards the lsb). */
VBigNum v_bignum_bit_shift_right(VBigNum a, unsigned int count)
{
	unsigned int	x, carry;
	int		i;

	if(count == 0)
		return a;
	/* Quick-shift whole words by simply copying them towards a.x[0]. */
	if(count >= 16)
	{
		unsigned int	places = count / 16;

		for(i = 0; i < (sizeof a.x / sizeof *a.x) - places; i++)
			a.x[i] = a.x[i + places];
		for(; i < sizeof a.x / sizeof *a.x; i++)
			a.x[i] = 0;
		count -= places * 16;
		if(count == 0)	/* Done? */
			return a;
	}
	for(i = (sizeof a.x / sizeof *a.x) - 1, carry = 0; i >= 0; i--)
	{
		x = a.x[i] << 16;		/* Set temporary's high word to next word of number. */
		x >>= count;
		x |= carry;
		carry  = (x & 0xffff) << 16;	/* Compute carry word. */
		a.x[i] = x >> 16;
	}
	return a;
}

/* ----------------------------------------------------------------------------------------- */

/* Computes a == b. Fairly simple. */
int v_bignum_eq(VBigNum a, VBigNum b)
{
	return memcmp(a.x, b.x, sizeof a.x) == 0;
}

/* Computes a >= b. */
int v_bignum_gte(VBigNum a, VBigNum b)
{
	int	i, j, k;

	/* Find index of highest non-zero digit in each number. */
	for(i = sizeof a.x / sizeof *a.x - 1; i >= 0; i--)
		if(a.x[i] > 0)
			break;
	for(j = sizeof b.x / sizeof *b.x - 1; j >= 0; j--)
		if(b.x[j] > 0)
			break;

	/* Quick answers exists for different-sized numbers. Find them. */
	if(i < j)
		return 0;
	if(i > j)
		return 1;
	/* If number of used digits are the same in both, compare digits. */
	for(k = i; k >= 0; k--)
	{
		if(a.x[k] < b.x[k])
			return 0;
		else if(a.x[k] > b.x[k])
			return 1;
	}
	return 0;
}

/* ----------------------------------------------------------------------------------------- */

VBigNum v_bignum_add_ushort(VBigNum a, unsigned short b)
{
	unsigned int	s;

	s = a.x[0] + b;
	a.x[0] = s;
	if(s > 0xffff)	/* Need to do carry distribution? */
	{
		unsigned int	i;

		for(i = 1; i < sizeof a.x / sizeof *a.x; i++)
		{
			if(++a.x[i])
				break;
		}
	}
	return a;
}

VBigNum v_bignum_sub_ushort(VBigNum a, unsigned short b)
{
	unsigned int	d;

	d = a.x[0] - b;
	a.x[0] = d;
	if(d > 0xffff)	/* Need to do carry distribution? */
	{
		unsigned int	i;

		for(i = 1; i < sizeof a.x / sizeof *a.x; i++)
		{
			a.x[i]--;
			if(a.x[i] < 0xffff)
				break;
		}
	}
	return a;
}

/* Multiply big integer by single "digit". */
VBigNum v_bignum_mul_ushort(VBigNum a, unsigned short b)
{
	unsigned int	i, carry, p;

	for(i = carry = 0; i < sizeof a.x / sizeof *a.x; i++)
	{
		p = b * a.x[i] + carry;
		a.x[i] = p;
		carry = p >> 16;
	}
	return a;
}

/* ----------------------------------------------------------------------------------------- */

/* Return a + b. */
VBigNum v_bignum_add(VBigNum a, VBigNum b)
{
	unsigned int	i, carry = 0, s;
	VBigNum		sum;

	for(i = 0; i < sizeof a.x / sizeof *a.x; i++)
	{
		s = a.x[i] + b.x[i] + carry;
		sum.x[i] = s;
		carry = s > 0xffff;
	}
	return sum;
}

/* Compute a - b. */
VBigNum v_bignum_sub(VBigNum a, VBigNum b)
{
	unsigned int	i, carry = 0, d;
	VBigNum		diff;

	for(i = 0; i < sizeof a.x / sizeof *a.x; i++)
	{
		d = a.x[i] - b.x[i] - carry;
		diff.x[i] = d;
		carry = d > 0xffff;
	}
	return diff;
}

/* Return a * BASE ^ places, i.e. shift <a> <places> "digits" to the left, padding with zeros. */
static VBigNum v_bignum_digit_shift_left(VBigNum a, unsigned short count)
{
	int	i;

	if(count == 0)		/* Would work, but why bother? */
		return a;
	for(i = (sizeof a.x / sizeof *a.x) - 1; i >= count; i--)
		a.x[i] = a.x[i - count];
	for(; i >= 0; i--)
		a.x[i] = 0;
	return a;
}

/* Compute a * b. Simply the pen-and-paper method, nothing fancy. */
VBigNum v_bignum_mul(VBigNum a, VBigNum b)
{
	unsigned int	i;
	VBigNum		temp, product;

	product = v_bignum_new_zero();
	for(i = 0; i < sizeof b.x / sizeof *b.x; i++)
	{
		temp = v_bignum_mul_ushort(a, b.x[i]);
		temp = v_bignum_digit_shift_left(temp, i);
		product = v_bignum_add(product, temp);
	}
	return product;
}

/* Returns a / b, sets *remainer to a % b. Pen-and-paper method, nothing fancy. */
VBigNum v_bignum_div(VBigNum a, VBigNum b, VBigNum *remainder)
{
	VBigNum	q, work;
	int	msba = v_bignum_bit_msb(a), msbb = v_bignum_bit_msb(b), next;

	/* Sanity-check inputs. */
	if(msbb > msba)
	{
		*remainder = b;
		return v_bignum_new_zero();
	}
	if(msbb == 0 && ((b.x[0] & 1) == 0))	/* Avoid division by zero. */
		return v_bignum_new_zero();

	/* Begin by setting result to zero, and "work" to the |b| highest bits of a. */
	q    = v_bignum_new_zero();
	work = v_bignum_new_bignum(a, msba, msbb + 1);
	for(next = msba - (msbb + 1); next >= -1; next--)	/* Loop over rest of bits. */
	{
		q = v_bignum_bit_shift_left(q, 1);
		if(v_bignum_gte(work, b))
		{
			q.x[0] |= 1;
			work = v_bignum_sub(work, b);
		}
		if(next >= 0)	/* This is kind of a trick to get use the etra iteration when next==-1. */
		{
			work = v_bignum_bit_shift_left(work, 1);
			if(v_bignum_bit_test(a, next))
				work.x[0] |= 1;
		}
	}
	*remainder = work;
	return q;
}

/* Return a % b. Simply uses division and returns the remainder. */
VBigNum v_bignum_mod(VBigNum a, VBigNum b)
{
	VBigNum	rem;

	v_bignum_div(a, b, &rem);
	return rem;
}

/* Division, returns a / b and sets <remainder> to any remainder. For a >> b, this is *glacial*. */
VBigNum v_bignum_div_slow(VBigNum a, VBigNum b, VBigNum *remainder)
{
	VBigNum	q;

	q = v_bignum_new_zero();
	while(v_bignum_gte(a, b))
	{
		q = v_bignum_add_ushort(q, 1);
		a = v_bignum_sub(a, b);
	}
	*remainder = a;
	return q;
}

/* ----------------------------------------------------------------------------------------- */

#if defined V_BIGNUM_STANDALONE

int main(void)
{
/*	VBigNum	a = bignum_from_string("39A27483D4D6851BAC7CDF637861367A1431FDB1450489EF1EB2"),
		b = bignum_from_string("4000000000000000"), q, r;
*/
	VBigNum	a = v_bignum_new_string("21"), b = v_bignum_new_string("3"), q, r;
	VBigNum	c = v_bignum_new_string("39A27483D4D6851BAC7CDF637861367A1431FDB1450489EF1EB2deadcafebabe"),
		d = v_bignum_new_string("deafbabe");

	printf("c==c? %d\n", v_bignum_eq(c, c));
	printf("c==d? %d\n", v_bignum_eq(c, d));

	v_bignum_print_hex(a);
	v_bignum_print_hex(b);
	q = v_bignum_div(a, b, &r);
	printf("q: ");
	v_bignum_print_hex(q);
	printf("r: ");
	v_bignum_print_hex(r);

	printf("----------------------------------------------------------\n");
	
	v_bignum_print_hex(c);
	v_bignum_print_hex(d);
	q = v_bignum_div(c, d, &r);
	printf("q: ");
	v_bignum_print_hex(q);
	printf("r: ");
	v_bignum_print_hex(r);

	return 0;
}

#endif		/* V_BIGNUM_STANDALONE */
