/*
** v_pack.c
**
** These functions are used to pack and unpack various quantities to/from network
** packet buffers. They do not care about alignment, operating at byte level internally.
** The external byte-ordering used is big-endian (aka "network byte order") for all
** quantities larger than a single byte.
*/

#include <stdlib.h>
#include <stdio.h>

#include "v_pack.h"

size_t vnp_raw_pack_uint8(void *buffer, uint8 data)
{
	*(uint8 *) buffer = data;

	return sizeof data;
}

size_t vnp_raw_unpack_uint8(const void *buffer, uint8 *data)
{
	*data = *(uint8 *) buffer;

	return sizeof *data;
}

size_t vnp_raw_pack_uint16(void *buffer, uint16 data)
{
	*(uint8 *) buffer = (data & 0xFF00) >> 8;
	*((uint8 *) buffer + 1) = data & 0xFF;

	return sizeof data;
}

size_t vnp_raw_unpack_uint16(const void *buffer, uint16 *data)
{
	register const uint8 *p = buffer;
	register uint16	tmp = 0;

	tmp |= ((uint16) *p++) << 8;
	tmp |= ((uint16) *p++);
	*data = tmp;

	return sizeof *data;
}

size_t vnp_raw_pack_uint24(void *buffer, uint32 data)
{
	register uint8 *p = buffer;

	data >>= 8;
	*(p++) = (data >> 24) & 0xFF;
	*(p++) = (data >> 16) & 0xFF;
	*(p++) = (data >> 8)  & 0xFF;

	return 3;
}

size_t vnp_raw_unpack_uint24(const void *buffer, uint32 *data)
{
	register const uint8 *p = buffer;
	register uint32	tmp = 0;

	tmp |= ((uint32) *p++) << 24;
	tmp |= ((uint32) *p++) << 16;
	tmp |= ((uint32) *p++) << 8;
	*data = tmp << 8;

	return 3;
}

size_t vnp_raw_pack_uint32(void *buffer, uint32 data)
{
	register uint8 *p = buffer;

	*(p++) = (data >> 24) & 0xFF;
	*(p++) = (data >> 16) & 0xFF;
	*(p++) = (data >> 8)  & 0xFF;
	*(p++) = data & 0xFF;

	return sizeof data;
}

size_t vnp_raw_unpack_uint32(const void *buffer, uint32 *data)
{
	register const uint8 *p = buffer;
	register uint32	tmp = 0;
	tmp |= ((uint32) *p++) << 24;
	tmp |= ((uint32) *p++) << 16;
	tmp |= ((uint32) *p++) << 8;
	tmp |= ((uint32) *p++);
	*data = tmp;

	return sizeof *data;
}

size_t vnp_raw_pack_real32(void *buffer, real32 data)
{
	union { uint32 uint; real32 real; } punt;
	punt.real = data;
	return vnp_raw_pack_uint32(buffer, punt.uint);
/*	uint32	tmp;
	tmp = *((uint32 *) &data);
	return vnp_raw_pack_uint32(buffer, tmp);
*/
}

size_t vnp_raw_unpack_real32(const void *buffer, real32 *data)
{
	return vnp_raw_unpack_uint32(buffer, (uint32 *) data);
}

size_t vnp_raw_pack_real64(void *buffer, real64 data)
{
	union { uint32 uint[2]; real64 real; } punt;
	uint32	size;

	punt.real = data;
	size = vnp_raw_pack_uint32(buffer, punt.uint[0]);
	buffer += size;
	size += vnp_raw_pack_uint32(buffer, punt.uint[1]);
	return size;
}

size_t vnp_raw_unpack_real64(const void *buffer, real64 *data)
{
	union { uint32 uint[2]; real64 real; } punt;
	uint32	size;

	size =  vnp_raw_unpack_uint32(buffer, &punt.uint[0]);
	size += vnp_raw_unpack_uint32(buffer + size, &punt.uint[1]);
	*data = punt.real;
	return size;
}

size_t vnp_raw_pack_string(void *buffer, const char *string, size_t max_size)
{
	unsigned int i = 0;
	char *p = buffer;
	if(string != 0)
		for(; i < max_size && string[i] != 0; i++)
			p[i] = string[i];
	p[i] = 0;
	return ++i;
}

size_t vnp_raw_unpack_string(const void *buffer, char *string, size_t max_size, size_t max_size2)
{
	unsigned int i;
	const char *p = buffer;

	max_size--;
	max_size2--;
	for(i = 0; i < max_size && i < max_size2 && p[i] != 0; i++)
		string[i] = p[i];
	string[i] = 0;
	return ++i;
}
