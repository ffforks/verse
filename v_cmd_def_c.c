
#include <stdlib.h>
#include <stdio.h>
#include "verse_header.h"
#include "v_cmd_gen.h"
#include "v_cmd_buf.h"

#if defined(V_GENERATE_FUNC_MODE)

void v_gen_curve_cmd_def(void)
{
	v_cg_new_cmd(V_NT_CURVE,		"c_curve_create", 128, VCGCT_NORMAL);
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"curve_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_NAME,		"name");
	v_cg_alias(FALSE, "c_curve_destroy", "if(name[0] == 0)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_CURVE,		"c_curve_subscribe", 129, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"curve_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_alias(TRUE, "c_curve_unsubscribe", "if(!alias_bool)", 2, NULL);
	v_cg_end_cmd();

	v_cg_new_cmd(V_NT_CURVE,		"c_curve_key_set", 130, VCGCT_NORMAL); 
	v_cg_add_param(VCGP_NODE_ID,	"node_id");
	v_cg_add_param(VCGP_LAYER_ID,	"curve_id");
	v_cg_add_param(VCGP_UINT32,		"key_id");
	v_cg_add_param(VCGP_END_ADDRESS, NULL);
	v_cg_add_param(VCGP_REAL64,		"pre_value");
	v_cg_add_param(VCGP_UINT32,		"pre_pos");
	v_cg_add_param(VCGP_REAL64,		"value");
	v_cg_add_param(VCGP_REAL64,		"pos");
	v_cg_add_param(VCGP_REAL64,		"post_value");
	v_cg_add_param(VCGP_UINT32,		"post_pos");
	v_cg_alias(FALSE, "c_curve_key_destroy", "if(pre_value == 0)", 3, NULL);
	v_cg_end_cmd();
}

#endif
