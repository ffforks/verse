/*
**
*/

#include <stdlib.h>

#include "v_cmd_gen.h"

#if !defined(V_GENERATE_FUNC_MODE)

#include "verse.h"
#include "v_pack.h"

void * verse_pack_method_call(unsigned int param_count, const VNOParam *params, const VNOParamType *param_type)
{
	unsigned int i, j, buffer_pos;
	uint8 *buf;
	buf = malloc((sizeof *buf) * (1500 + 8 * 16));
	buffer_pos = vnp_raw_pack_uint16(buf, 0);
	for(i = 0; i < param_count; i++)
	{
		switch(param_type[i])
		{
			case VN_O_METHOD_PTYPE_INTEGER8 :
				buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], params[i].integer8);
			break;
			case VN_O_METHOD_PTYPE_INTEGER16 :
				buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], params[i].integer16);
			break;
			case VN_O_METHOD_PTYPE_INTEGER32 :
				buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], params[i].integer32);
			break;
			case VN_O_METHOD_PTYPE_UINTEGER8 :
				buffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], params[i].uinteger8);
			break;
			case VN_O_METHOD_PTYPE_UINTEGER16 :
				buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], params[i].uinteger16);
			break;
			case VN_O_METHOD_PTYPE_UINTEGER32 :
				buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], params[i].uinteger32);
			break;
			case VN_O_METHOD_PTYPE_REAL32 :
				buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], params[i].real32);
			break;
			case VN_O_METHOD_PTYPE_REAL64 :
				buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], params[i].real64);
			break;
			case VN_O_METHOD_PTYPE_STRING :
				buffer_pos = vnp_raw_pack_string(&buf[buffer_pos], params[i].string, (1500 + 8 * 16) - buffer_pos);
			break;
			case VN_O_METHOD_PTYPE_VNODE :
				buffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], params[i].vnode);
			break;
			case VN_O_METHOD_PTYPE_VLAYER :
				buffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], params[i].vlayer);
			break;
			case VN_O_METHOD_PTYPE_2_VECTOR32 :
				for(j = 0; j < 2; j++)
					buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], params[i].vector32[j]);
			break;
			case VN_O_METHOD_PTYPE_3_VECTOR32 :
				for(j = 0; j < 3; j++)
					buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], params[i].vector32[j]);
			break;
			case VN_O_METHOD_PTYPE_4_VECTOR32 :
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], params[i].vector32[j]);
			break;
			case VN_O_METHOD_PTYPE_2_VECTOR64 :
				for(j = 0; j < 2; j++)
					buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], params[i].vector64[j]);
			break;
			case VN_O_METHOD_PTYPE_3_VECTOR64 :
				for(j = 0; j < 3; j++)
					buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], params[i].vector64[j]);
			break;
			case VN_O_METHOD_PTYPE_4_VECTOR64 :
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], params[i].vector64[j]);
			break;
			case VN_O_METHOD_PTYPE_4_MATRIX32 :
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], params[i].matrix32[j]);
			break;
			case VN_O_METHOD_PTYPE_9_MATRIX32 :
				for(j = 0; j < 9; j++)
					buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], params[i].matrix32[j]);
			break;
			case VN_O_METHOD_PTYPE_16_MATRIX32 :
				for(j = 0; j < 16; j++)
					buffer_pos += vnp_raw_pack_float(&buf[buffer_pos], params[i].matrix32[j]);
			break;
			case VN_O_METHOD_PTYPE_4_MATRIX64 :
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], params[i].matrix64[j]);
			break;
			case VN_O_METHOD_PTYPE_9_MATRIX64 :
				for(j = 0; j < 9; j++)
					buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], params[i].matrix64[j]);
			break;
			case VN_O_METHOD_PTYPE_16_MATRIX64 :
				for(j = 0; j < 16; j++)
					buffer_pos += vnp_raw_pack_double(&buf[buffer_pos], params[i].matrix64[j]);
			break;
		}
		if(buffer_pos > 1500)
		{
			free(buf);
			return NULL;
		}
	}
	vnp_raw_pack_uint16(buf, (uint16)buffer_pos);
	return buf;
}

boolean verse_unpack_method_call(void *data, unsigned int param_count, VNOParam *params, const VNOParamType *param_type)
{
	unsigned int i, j, buffer_pos, size;
	uint8 *buf;
	buf = data;
	size = vnp_raw_unpack_uint16(buf, 0);
	for(i = 0; i < param_count; i++)
	{
		switch(param_type[i])
		{
			case VN_O_METHOD_PTYPE_INTEGER8 :
				buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &params[i].integer8);
			break;
			case VN_O_METHOD_PTYPE_INTEGER16 :
				buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &params[i].integer16);
			break;
			case VN_O_METHOD_PTYPE_INTEGER32 :
				buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &params[i].integer32);
			break;
			case VN_O_METHOD_PTYPE_UINTEGER8 :
				buffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &params[i].uinteger8);
			break;
			case VN_O_METHOD_PTYPE_UINTEGER16 :
				buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &params[i].uinteger16);
			break;
			case VN_O_METHOD_PTYPE_UINTEGER32 :
				buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &params[i].uinteger32);
			break;
			case VN_O_METHOD_PTYPE_REAL32 :
				buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &params[i].real32);
			break;
			case VN_O_METHOD_PTYPE_REAL64 :
				buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &params[i].real64);
			break;
			case VN_O_METHOD_PTYPE_STRING :
				buffer_pos = vnp_raw_unpack_string(&buf[buffer_pos], params[i].string, (1500 + 8 * 16) - buffer_pos, -1);
			break;
			case VN_O_METHOD_PTYPE_VNODE :
				buffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &params[i].vnode);
			break;
			case VN_O_METHOD_PTYPE_VLAYER :
				buffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &params[i].vlayer);
			break;
			case VN_O_METHOD_PTYPE_2_VECTOR32 :
				for(j = 0; j < 2; j++)
					buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &params[i].vector32[j]);
			break;
			case VN_O_METHOD_PTYPE_3_VECTOR32 :
				for(j = 0; j < 3; j++)
					buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &params[i].vector32[j]);
			break;
			case VN_O_METHOD_PTYPE_4_VECTOR32 :
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &params[i].vector32[j]);
			break;
			case VN_O_METHOD_PTYPE_2_VECTOR64 :
				for(j = 0; j < 2; j++)
					buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &params[i].vector64[j]);
			break;
			case VN_O_METHOD_PTYPE_3_VECTOR64 :
				for(j = 0; j < 3; j++)
					buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &params[i].vector64[j]);
			break;
			case VN_O_METHOD_PTYPE_4_VECTOR64 :
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &params[i].vector64[j]);
			break;			
			case VN_O_METHOD_PTYPE_4_MATRIX32 :
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &params[i].matrix32[j]);
			break;
			case VN_O_METHOD_PTYPE_9_MATRIX32 :
				for(j = 0; j < 9; j++)
					buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &params[i].matrix32[j]);
			break;
			case VN_O_METHOD_PTYPE_16_MATRIX32 :
				for(j = 0; j < 16; j++)
					buffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &params[i].matrix32[j]);
			break;
			case VN_O_METHOD_PTYPE_4_MATRIX64 :
				for(j = 0; j < 4; j++)
					buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &params[i].matrix64[j]);
			break;
			case VN_O_METHOD_PTYPE_9_MATRIX64 :
				for(j = 0; j < 9; j++)
					buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &params[i].matrix64[j]);
			break;
			case VN_O_METHOD_PTYPE_16_MATRIX64 :
				for(j = 0; j < 16; j++)
					buffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &params[i].matrix64[j]);
			break;
		}
	}
	return TRUE;
}
#endif
