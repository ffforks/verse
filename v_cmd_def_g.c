
#include <stdlib.h>
#include <stdio.h>
#include "verse_header.h"
#include "v_cmd_gen.h"
#include "v_cmd_buf.h"

#if defined(V_GENERATE_FUNC_MODE)

void v_gen_geometry_cmd_def()
{
	unsigned int order[2] = {0, 2};

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_layer_create", 48, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_NAME,		"name");
	v_cg_add_param(VCGP_ENUM_NAME,	"VNGLayerType");
	v_cg_add_param(VCGP_ENUM,		"type");
	v_cg_add_param(VCGP_UINT32,		"def_integer");
	v_cg_add_param(VCGP_REAL64,		"def_real");
	v_cg_alias(FALSE, "g_layer_destroy", "if(name[0] == 0)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_layer_subscribe", 49, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_ENUM_NAME,	"VNORealFormat");
	v_cg_add_param(VCGP_ENUM,		"type");
	v_cg_alias(TRUE, "g_layer_unsubscribe", NULL, 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_vertex_set_real32_xyz", 50, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"vertex_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_REAL32,		"x");
	v_cg_add_param(VCGP_REAL32,		"y");
	v_cg_add_param(VCGP_REAL32,		"z");
	v_cg_alias(FALSE, "g_vertex_delete_real32", "if(x == V_REAL32_MAX || y == V_REAL32_MAX || z == V_REAL32_MAX)", 2, order);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_vertex_set_real64_xyz", 51, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"vertex_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_REAL64,		"x");
	v_cg_add_param(VCGP_REAL64,		"y");
	v_cg_add_param(VCGP_REAL64,		"z");
	v_cg_alias(FALSE, "g_vertex_delete_real64", "if(x == V_REAL64_MAX || y == V_REAL64_MAX || z == V_REAL64_MAX)", 2, order);
	v_cg_end_cmd();
	
	v_cg_new_cmd(V_NT_GEOMETRY,		"g_vertex_set_uinteger32", 52, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"vertex_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_UINT32,		"value");
	v_cg_end_cmd();
	
	v_cg_new_cmd(V_NT_GEOMETRY,		"g_vertex_set_real64", 53, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"vertex_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_REAL64,		"value");
	v_cg_end_cmd();
/*
	v_cg_new_cmd(V_NT_GEOMETRY,		"g_vertex_delete", VCGCT_NORMAL); 
	v_cg_add_param(VCGP_UINT32,		"vertex_id");
	v_cg_end_cmd();
*/
	v_cg_new_cmd(V_NT_GEOMETRY,		"g_polygon_set_corner_uinteger32", 54, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"polygon_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_UINT32,		"v0");
	v_cg_add_param(VCGP_UINT32,		"v1");
	v_cg_add_param(VCGP_UINT32,		"v2");
	v_cg_add_param(VCGP_UINT32,		"v3");
	v_cg_alias(FALSE, "g_polygon_delete", "if(v0 == -1 || v1 == -1 || v2 == -1)", 2, order);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_polygon_set_corner_real64", 55, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"polygon_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_REAL64,		"v0");
	v_cg_add_param(VCGP_REAL64,		"v1");
	v_cg_add_param(VCGP_REAL64,		"v2");
	v_cg_add_param(VCGP_REAL64,		"v3");
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_polygon_set_face_uinteger8", 56, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"polygon_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_UINT8,		"value");
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_polygon_set_face_uinteger32", 57, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"polygon_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_UINT32,		"value");
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_polygon_set_face_real64", 58, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"layer_id");
	v_cg_add_param(VCGP_UINT32,		"polygon_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_REAL64,		"value");
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_crease_set_vertex", 59, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_NAME,		"layer");
	v_cg_add_param(VCGP_UINT32,		"def_crease");
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_crease_set_edge", 60, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_NAME,		"layer");
	v_cg_add_param(VCGP_UINT32,		"def_crease");
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_GEOMETRY,		"g_bone_create", 61, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_UINT32,		"bone_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_NAME,		"name");
	v_cg_add_param(VCGP_UINT32,		"parent");
	v_cg_add_param(VCGP_REAL64,		"pos_x");
	v_cg_add_param(VCGP_REAL64,		"pos_y");
	v_cg_add_param(VCGP_REAL64,		"pos_z");
	v_cg_add_param(VCGP_REAL64,		"rot_x");
	v_cg_add_param(VCGP_REAL64,		"rot_y");
	v_cg_add_param(VCGP_REAL64,		"rot_z");
	v_cg_alias(FALSE, "g_bone_destroy", "if(name[0] == 0)", 2, NULL);
	v_cg_end_cmd();
}

#endif
