
#include <stdlib.h>
#include <stdio.h>
#include "verse_header.h"
#include "v_cmd_gen.h"
#include "v_cmd_buf.h"


#if defined(V_GENERATE_FUNC_MODE)

void v_gen_bitmap_cmd_def()
{
	v_cg_new_cmd(V_NT_BITMAP,		"b_init_dimensions", 80, VCGCT_NORMAL);
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


	v_cg_new_cmd(V_NT_BITMAP,		"b_layer_set_tile", 83, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT16,		"tile_x");
	v_cg_add_param(VCGP_UINT16,		"tile_y");
	v_cg_add_param(VCGP_UINT16,		"z");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_ENUM_NAME,	"VNBLayerType");
	v_cg_add_param(VCGP_ENUM,		"type");
	v_cg_add_param(VCGP_POINTER,		"data");

	v_cg_add_param(VCGP_PACK_INLINE, "\t{\n"
	"\t\tunsigned int i;\n"
	"\t\tswitch(type)\n"
	"\t\t{\n"
	"\t\t\tcase VN_B_LAYER_INTEGER1 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE / 8; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8 *)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_INTEGER8 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint8(&buf[buffer_pos], ((uint8 *)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_INTEGER16 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_uint16(&buf[buffer_pos], ((uint16 *)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_REAL32 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_float(&buf[buffer_pos], ((float *)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t\tcase VN_B_LAYER_REAL64 :\n"
	"\t\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\t\tbuffer_pos += vnp_raw_pack_double(&buf[buffer_pos], ((double *)data)[i]);\n"
	"\t\t\tbreak;\n"
	"\t\t}\n"
	"\t}\n");

	v_cg_add_param(VCGP_UNPACK_INLINE, "\tswitch(type)\n"
	"\t{\n"
	"\t\tcase VN_B_LAYER_INTEGER1 :\n"
	"\t\t{\n"
	"\t\t\tunsigned int i;\n"
	"\t\t\tuint8 tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE / 8];\n"
	"\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE / 8; i++)\n"
	"\t\t\t\tbuffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &tile[i]);\n"
	"\t\t\tif(func_b_layer_set_tile != NULL)\n"
	"\t\t\t\tfunc_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);\n"
	"\t\t\treturn buffer_pos;\n"
	"\t\t}\n"
	"\t\tcase VN_B_LAYER_INTEGER8 :\n"
	"\t\t{\n"
	"\t\t\tunsigned int i;\n"
	"\t\t\tuint8 tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE];\n"
	"\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\tbuffer_pos += vnp_raw_unpack_uint8(&buf[buffer_pos], &tile[i]);\n"
	"\t\t\tif(func_b_layer_set_tile != NULL)\n"
	"\t\t\t\tfunc_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);\n"
	"\t\t\treturn buffer_pos;\n"
	"\t\t}\n"
	"\t\tbreak;\n"
	"\t\tcase VN_B_LAYER_INTEGER16 :\n"
	"\t\t{\n"
	"\t\t\tunsigned int i;\n"
	"\t\t\tuint16 tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE];\n"
	"\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\tbuffer_pos += vnp_raw_unpack_uint16(&buf[buffer_pos], &tile[i]);\n"
	"\t\t\tif(func_b_layer_set_tile != NULL)\n"
	"\t\t\t\tfunc_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);\n"
	"\t\t\treturn buffer_pos;\n"
	"\t\t}\n"
	"\t\tbreak;\n"
	"\t\tcase VN_B_LAYER_REAL32 :\n"
	"\t\t{\n"
	"\t\t\tunsigned int i;\n"
	"\t\t\tfloat tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE];\n"
	"\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\tbuffer_pos += vnp_raw_unpack_float(&buf[buffer_pos], &tile[i]);\n"
	"\t\t\tif(func_b_layer_set_tile != NULL)\n"
	"\t\t\t\tfunc_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);\n"
	"\t\t\treturn buffer_pos;\n"
	"\t\t}\n"
	"\t\tbreak;\n"
	"\t\tcase VN_B_LAYER_REAL64 :\n"
	"\t\t{\n"
	"\t\t\tunsigned int i;\n"
	"\t\t\tdouble tile[VN_B_TILE_SIZE * VN_B_TILE_SIZE];\n"
	"\t\t\tfor(i = 0; i < VN_B_TILE_SIZE * VN_B_TILE_SIZE; i++)\n"
	"\t\t\t\tbuffer_pos += vnp_raw_unpack_double(&buf[buffer_pos], &tile[i]);\n"
	"\t\t\tif(func_b_layer_set_tile != NULL)\n"
	"\t\t\t\tfunc_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, tile);\n"
	"\t\t\treturn buffer_pos;\n"
	"\t\t}\n"
	"\t\tbreak;\n"
	"\t\tfunc_b_layer_set_tile(user_data, node_id, layer_id, tile_x, tile_y, z, type, data);\n"
	"\t\treturn buffer_pos;\n"
	"\t}\n");

	v_cg_end_cmd();

}

#endif
