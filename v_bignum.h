/*
 * Verse routines for big integer operations.
 * Handy in heavy encryption done during connect.
*/

#include <limits.h>

/* ----------------------------------------------------------------------------------------- */

#define	V_BIGNUM_BITS	2048	/* Don't change this unless you know what you're doing. */

/* The integer, as a sequence of bits in little-endian (!) order.
 * This means that VBigNum.x[0] contains the least significant bits.
*/
typedef struct
{
	unsigned short	x[V_BIGNUM_BITS / (CHAR_BIT * sizeof (unsigned short))];
} VBigNum;

/* ----------------------------------------------------------------------------------------- */

extern VBigNum	v_bignum_new_zero(void);
extern VBigNum	v_bignum_new_one(void);
extern VBigNum	v_bignum_new_ushort(unsigned short a);
extern VBigNum	v_bignum_new_bignum(VBigNum a, unsigned int msb, unsigned int bits);
extern VBigNum	v_bignum_new_string(const char *hex);
extern VBigNum	v_bignum_new_random(void);

extern void	v_bignum_print_hex(VBigNum a);

extern int	v_bignum_bit_test(VBigNum a, unsigned int bit);
extern int	v_bignum_bit_msb(VBigNum a);
extern VBigNum	v_bignum_bit_shift_left(VBigNum a, unsigned int count);
extern VBigNum	v_bignum_bit_shift_right(VBigNum a, unsigned int count);

extern int	v_bignum_gte(VBigNum a, VBigNum b);

extern VBigNum	v_bignum_add_ushort(VBigNum a, unsigned short b);
extern VBigNum	v_bignum_sub_ushort(VBigNum a, unsigned short b);
extern VBigNum	v_bignum_mul_ushort(VBigNum a, unsigned short b);

extern VBigNum	v_bignum_add(VBigNum a, VBigNum b);
extern VBigNum	v_bignum_sub(VBigNum a, VBigNum b);
extern VBigNum	v_bignum_mul(VBigNum a, VBigNum b);
extern VBigNum	v_bignum_div(VBigNum a, VBigNum b, VBigNum *remainder);
extern VBigNum	v_bignum_mod(VBigNum a, VBigNum b);
