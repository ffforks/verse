
#include <stdlib.h>
#include <stdio.h>
#include "verse_header.h"
#include "v_cmd_gen.h"
#include "v_cmd_buf.h"

#if defined(V_GENERATE_FUNC_MODE)


void v_gen_audio_cmd_def(void)
{
	v_cg_new_cmd(V_NT_CURVE,		"a_layer_create", 160, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"curve_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_NAME,		"name");
	v_cg_alias(FALSE, "a_layer_destroy", "if(name[0] == 0)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_CURVE,		"a_layer_subscribe", 161, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"curve_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_ENUM_NAME,	"VNATransferType");
	v_cg_add_param(VCGP_ENUM,		"transfer");
	v_cg_alias(FALSE, "a_layer_unsubscribe", "if(transfer > VN_A_COMPRESSED_UNSUSTAINED)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_CURVE,		"a_audio", 162, VCGCT_NORMAL); 
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
	v_cg_end_cmd();
}

#endif
