
#include <stdlib.h>
#include <stdio.h>
#include "verse_header.h"
#include "v_cmd_gen.h"
#include "v_cmd_buf.h"

#if defined(V_GENERATE_FUNC_MODE)

void v_gen_audio_cmd_def(void)
{
	v_cg_new_cmd(V_NT_AUDIO,		"a_layer_create", 160, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_ENUM_NAME,	"VNALayerType");
	v_cg_add_param(VCGP_ENUM,		"type");
	v_cg_add_param(VCGP_REAL64,		"frequency");
	v_cg_add_param(VCGP_NAME,		"name");
	v_cg_alias(FALSE, "a_layer_destroy", "if(name[0] == 0)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_AUDIO,		"a_layer_subscribe", 161, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_alias(TRUE, "a_layer_unsubscribe", NULL, 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_AUDIO,		"a_block_set", 162, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_ENUM_NAME,	"VNALayerType");
	v_cg_add_param(VCGP_ENUM,		"type");
	v_cg_add_param(VCGP_POINTER,	"data");

	v_cg_add_param(VCGP_PACK_INLINE, "\t{\n"
	"\t\tunsigned int i;\n"
	"\t\tswitch(type)\n"
	"\t\t{\n"
	"\t\t\tcase VN_A_LAYER_INT8 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT8; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_INT16 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT16; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], ((uint16*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_INT24 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT24; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint24(&buf[buffer_pos], ((uint32*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_INT32 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT32; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((uint32*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_REAL32 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL32; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_real32(&buf[buffer_pos], ((real32*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_REAL64 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL64; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], ((real64*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t}\n"
	"\t}\n");

	v_cg_add_param(VCGP_UNPACK_INLINE, "\t{\n"
	"\t\tunsigned int i;\n"
	"\t\tswitch(type)\n"
	"\t\t{\n"
	"\t\t\tcase VN_A_LAYER_INT8 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint8 data[1500];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT8; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_block_set != NULL)\n"
	"\t\t\t\t\tfunc_a_block_set(v_fs_get_user_data(162), node_id, layer_id, id, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_INT16 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint16 data[750];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT16; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_block_set != NULL)\n"
	"\t\t\t\t\tfunc_a_block_set(v_fs_get_user_data(162), node_id, layer_id, id, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_INT24 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint32 data[325];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT24; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint24(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_block_set != NULL)\n"
	"\t\t\t\t\tfunc_a_block_set(v_fs_get_user_data(162), node_id, layer_id, id, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_INT32 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint32 data[325];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT32; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_block_set != NULL)\n"
	"\t\t\t\t\tfunc_a_block_set(v_fs_get_user_data(162), node_id, layer_id, id, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_REAL32 :\n"
	"\t\t\t{\n"
	"\t\t\t\treal32 data[1500];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL32; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_block_set != NULL)\n"
	"\t\t\t\t\tfunc_a_block_set(v_fs_get_user_data(162), node_id, layer_id, id, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_REAL64 :\n"
	"\t\t\t{\n"
	"\t\t\t\treal64 data[162];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL64; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_block_set != NULL)\n"
	"\t\t\t\t\tfunc_a_block_set(v_fs_get_user_data(162), node_id, layer_id, id, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t}\n"
	"\t}\n");
	v_cg_alias(FALSE, "a_block_clear", "if(type > VN_A_LAYER_REAL64)", 3, NULL);
	v_cg_end_cmd();
/*
	
#define VN_AUDIO_SAMPLE_BLOCK_SIZE_INT8 1024
#define VN_AUDIO_SAMPLE_BLOCK_SIZE_INT16 512
#define VN_AUDIO_SAMPLE_BLOCK_SIZE_INT24 384
#define VN_AUDIO_SAMPLE_BLOCK_SIZE_INT32 256
#define VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL32 256
#define VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL64 128

*/
	v_cg_new_cmd(V_NT_AUDIO,		"a_stream_create", 163, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"stream_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_NAME,		"name");
	v_cg_alias(FALSE, "a_stream_destroy", "if(name[0] == 0)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_AUDIO,		"a_stream_subscribe", 164, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"stream_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_alias(TRUE, "a_stream_unsubscribe", NULL, 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_AUDIO,		"a_stream", 165, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"stream_id");
	v_cg_add_param(VCGP_UINT16,		"id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_UINT32,		"time_s");
	v_cg_add_param(VCGP_UINT32,		"time_f");
	v_cg_add_param(VCGP_ENUM_NAME,	"VNALayerType");
	v_cg_add_param(VCGP_ENUM,		"type");
	v_cg_add_param(VCGP_REAL64,		"frequency");
	v_cg_add_param(VCGP_POINTER,	"data");

	v_cg_add_param(VCGP_PACK_INLINE, "\t{\n"
	"\t\tunsigned int i;\n"
	"\t\tswitch(type)\n"
	"\t\t{\n"
	"\t\t\tcase VN_A_LAYER_INT8 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT8; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_INT16 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT16; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], ((uint16*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_INT24 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT24; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint24(&buf[buffer_pos], ((uint32*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_INT32 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT32; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((uint32*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_REAL32 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL32; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_real32(&buf[buffer_pos], ((real32*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_REAL64 :\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL64; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], ((real64*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t}\n"
	"\t}\n");

	v_cg_add_param(VCGP_UNPACK_INLINE, "\t{\n"
	"\t\tunsigned int i;\n"
	"\t\tswitch(type)\n"
	"\t\t{\n"
	"\t\t\tcase VN_A_LAYER_INT8 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint8 data[VN_AUDIO_SAMPLE_BLOCK_SIZE_INT8];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT8; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_stream != NULL)\n"
	"\t\t\t\t\tfunc_a_stream(v_fs_get_user_data(165), node_id, stream_id, id, time_s, time_f, (VNALayerType)type, frequency, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_INT16 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint16 data[VN_AUDIO_SAMPLE_BLOCK_SIZE_INT16];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT16; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_stream != NULL)\n"
	"\t\t\t\t\tfunc_a_stream(v_fs_get_user_data(165), node_id, stream_id, id, time_s, time_f, (VNALayerType)type, frequency, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_INT24 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint32 data[VN_AUDIO_SAMPLE_BLOCK_SIZE_INT24];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT24; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint24(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_stream != NULL)\n"
	"\t\t\t\t\tfunc_a_stream(v_fs_get_user_data(165), node_id, stream_id, id, time_s, time_f, (VNALayerType)type, frequency, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_INT32 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint32 data[VN_AUDIO_SAMPLE_BLOCK_SIZE_INT32];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_INT32; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_stream != NULL)\n"
	"\t\t\t\t\tfunc_a_stream(v_fs_get_user_data(165), node_id, stream_id, id, time_s, time_f, (VNALayerType)type, frequency, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_REAL32 :\n"
	"\t\t\t{\n"
	"\t\t\t\treal32 data[VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL32];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL32; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_stream != NULL)\n"
	"\t\t\t\t\tfunc_a_stream(v_fs_get_user_data(165), node_id, stream_id, id, time_s, time_f, (VNALayerType)type, frequency, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_REAL64 :\n"
	"\t\t\t{\n"
	"\t\t\t\treal64 data[VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL64];\n"
	"\t\t\t\tfor(i = 0; i < VN_AUDIO_SAMPLE_BLOCK_SIZE_REAL64; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_stream != NULL)\n"
	"\t\t\t\t\tfunc_a_stream(v_fs_get_user_data(165), node_id, stream_id, id, time_s, time_f, (VNALayerType)type, frequency, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t}\n"
	"\t}\n");

	v_cg_end_cmd();
}

#endif
