
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
	v_cg_add_param(VCGP_NAME,		"name");
	v_cg_alias(FALSE, "a_layer_destroy", "if(name[0] == 0)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_AUDIO,		"a_layer_subscribe", 161, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_ENUM_NAME,	"VNATransferType");
	v_cg_add_param(VCGP_ENUM,		"transfer");
	v_cg_alias(FALSE, "a_layer_unsubscribe", "if(transfer > VN_A_COMPRESSED_UNSUSTAINED)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_AUDIO,		"a_audio", 162, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT16,		"id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_UINT32,		"time");
	v_cg_add_param(VCGP_UINT16,		"length");
	v_cg_add_param(VCGP_ENUM_NAME,	"VNATransferType");
	v_cg_add_param(VCGP_ENUM,		"transfer");
	v_cg_add_param(VCGP_ENUM_NAME,	"VNALayerType");
	v_cg_add_param(VCGP_ENUM,		"type");
	v_cg_add_param(VCGP_POINTER,	"data");

	v_cg_add_param(VCGP_PACK_INLINE, "\t{\n"
	"\t\tunsigned int i;\n"
	"\t\tswitch(type)\n"
	"\t\t{\n"
	"\t\t\tcase VN_A_LAYER_INTEGER8 :\n"
	"\t\t\t\tfor(i = 0; i < length; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_INTEGER16 :\n"
	"\t\t\t\tfor(i = 0; i < length; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], ((uint16*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_INTEGER24 :\n"
	"\t\t\t\tfor(i = 0; i < length; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint24(&buf[buffer_pos], ((uint32*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_INTEGER32 :\n"
	"\t\t\t\tfor(i = 0; i < length; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint32(&buf[buffer_pos], ((uint32*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_REAL32 :\n"
	"\t\t\t\tfor(i = 0; i < length; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_real32(&buf[buffer_pos], ((real32*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_A_LAYER_REAL64 :\n"
	"\t\t\t\tfor(i = 0; i < length; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], ((real64*)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t}\n"
	"\t}\n");

	v_cg_add_param(VCGP_UNPACK_INLINE, "\t{\n"
	"\t\tunsigned int i;\n"
	"\t\tswitch(type)\n"
	"\t\t{\n"
	"\t\t\tcase VN_A_LAYER_INTEGER8 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint8 data[1500];\n"
	"\t\t\t\tfor(i = 0; i < length && length < 1500; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_audio != NULL)\n"
	"\t\t\t\t\tfunc_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_INTEGER16 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint16 data[750];\n"
	"\t\t\t\tfor(i = 0; i < length && length < 750; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_audio != NULL)\n"
	"\t\t\t\t\tfunc_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_INTEGER24 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint32 data[325];\n"
	"\t\t\t\tfor(i = 0; i < length && length < 325; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint24(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_audio != NULL)\n"
	"\t\t\t\t\tfunc_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_INTEGER32 :\n"
	"\t\t\t{\n"
	"\t\t\t\tuint32 data[325];\n"
	"\t\t\t\tfor(i = 0; i < length && length < 325; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint32(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_audio != NULL)\n"
	"\t\t\t\t\tfunc_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_REAL32 :\n"
	"\t\t\t{\n"
	"\t\t\t\treal32 data[1500];\n"
	"\t\t\t\tfor(i = 0; i < length && length < 325; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_audio != NULL)\n"
	"\t\t\t\t\tfunc_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t\tcase VN_A_LAYER_REAL64 :\n"
	"\t\t\t{\n"
	"\t\t\t\treal64 data[162];\n"
	"\t\t\t\tfor(i = 0; i < length && length < 162; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &data[i]);\n"
	"\t\t\t\tif(func_a_audio != NULL)\n"
	"\t\t\t\t\tfunc_a_audio(v_fs_get_user_data(162), node_id, layer_id, id, time, length, (VNATransferType)transfer, (VNALayerType)type, data);\n"
	"\t\t\t\treturn buffer_pos;\n"
	"\t\t\t}\n"
	"\t\t}\n"
	"\t}\n");


	v_cg_end_cmd();
}

#endif
