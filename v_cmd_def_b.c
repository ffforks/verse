
#include <stdlib.h>
#include <stdio.h>

#include "verse_header.h"
#include "v_cmd_gen.h"
#include "v_cmd_buf.h"

#if defined(V_GENERATE_FUNC_MODE)

void v_gen_bitmap_cmd_def(void)
{
	v_cg_new_cmd(V_NT_BITMAP,		"b_dimensions_set", 80, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_UINT16,		"width");
	v_cg_add_param(VCGP_UINT16,		"height");
	v_cg_add_param(VCGP_UINT16,		"depth");
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_BITMAP,		"b_layer_create", 81, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_NAME,		"name");
	v_cg_add_param(VCGP_ENUM_NAME,	"VNBLayerType");
	v_cg_add_param(VCGP_ENUM,		"type");
	v_cg_alias(FALSE, "b_layer_destroy", "if(name[0] == 0)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_BITMAP,		"b_layer_subscribe", 82, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_UINT8,		"level");
	v_cg_alias(FALSE, "b_layer_unsubscribe", "if(level == 255)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_BITMAP,		"b_tile_set", 83, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT16,		"tile_x");
	v_cg_add_param(VCGP_UINT16,		"tile_y");
	v_cg_add_param(VCGP_UINT16,		"z");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_ENUM_NAME,	"VNBLayerType");
	v_cg_add_param(VCGP_ENUM,		"type");
	v_cg_add_param(VCGP_POINTER_TYPE, "VNBTile");
	v_cg_add_param(VCGP_POINTER,	"tile");

	v_cg_add_param(VCGP_PACK_INLINE, "\t{\n"
	"\t\tunsigned int i;\n"
	"\t\tswitch(type)\n"
	"\t\t{\n"
	"\t\t\tcase VN_B_LAYER_UINT1 :\n"
	"\t\t\t\tbuffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], tile->vuint1);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_UINT8 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], tile->vuint8[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_UINT16 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], tile->vuint16[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_REAL32 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_real32(&buf[buffer_pos], tile->vreal32[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_REAL64 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_real64(&buf[buffer_pos], tile->vreal64[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t}\n"
	"\t}\n");

	v_cg_add_param(VCGP_UNPACK_INLINE, "\t{\n\tunsigned int i;\n"
	
	"\t\tVNBTile tile;\n"
	"\t\tswitch(type)\n"
	"\t\t{\n"
	"\t\t\tcase VN_B_LAYER_UINT1 :\n"
	"\t\t\t\tbuffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &tile.vuint1);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_UINT8 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &tile.vuint8[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_UINT16 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &tile.vuint16[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_REAL32 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_real32(&buf[buffer_pos], &tile.vreal32[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_REAL64 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_unpack_real64(&buf[buffer_pos], &tile.vreal64[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t}\n"
	"\t\tif(func_b_tile_set != NULL && type <= VN_B_LAYER_REAL64)\n"
	"\t\t\tfunc_b_tile_set(v_fs_get_user_data(83), node_id, layer_id, tile_x, tile_y, z, type, &tile);\n"
	"\t\treturn buffer_pos;\n"
	"\t}\n");
	v_cg_end_cmd();
}

#endif
