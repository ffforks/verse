/*
** v_pack.h
**
** These functions are used to pack and unpack various quantities to/from network
** packet buffers. They do not care about alignment, operating at byte level internally.
** The external byte-ordering used is big-endian (aka "network byte order") for all
** quantities larger than a single byte.
*/

#include "verse_header.h"

extern size_t vnp_raw_pack_uint8(void *buffer, uint8 data);
extern size_t vnp_raw_unpack_uint8(const void *buffer, uint8 *data);

extern size_t vnp_raw_pack_uint16(void *buffer, uint16 data);
extern size_t vnp_raw_unpack_uint16(const void *buffer, uint16 *data);

extern size_t vnp_raw_pack_uint32(void *buffer, uint32 data);
extern size_t vnp_raw_unpack_uint32(const void *buffer, uint32 *data);

extern size_t vnp_raw_pack_real32(void *buffer, real32 data);
extern size_t vnp_raw_unpack_real32(const void *buffer, real32 *data);

extern size_t vnp_raw_pack_real64(void *buffer, real64 data);
extern size_t vnp_raw_unpack_real64(const void *buffer, real64 *data);

extern size_t vnp_raw_pack_string(void *buffer, const char *string, size_t max_size);
extern size_t vnp_raw_unpack_string(const void *buffer, char *string, size_t max_size, size_t max_size2);
