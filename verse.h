#if !defined(VERSE_H)
#define	VERSE_H


#if !defined(VERSE_HEADER_H)
#define	VERSE_HEADER_H

/*
#ifdef _MSC_VER
typedef __int64 int64;
typedef unsigned __int64 uint64;
#elif __GNUC__
typedef long long int64;
typedef unsigned long long uint64;
#endif
*/

typedef unsigned char boolean;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char uint8;
typedef char int8;
typedef float real32;
typedef double real64;

#define V_REAL64_MAX         1.7976931348623158e+308 /* max value */
#define V_REAL32_MAX         3.40282347e+38F


#define TRUE 1
#define FALSE 0

typedef enum {
	V_NT_OBJECT = 0, 
	V_NT_GEOMETRY, 
	V_NT_MATERIAL, 
	V_NT_BITMAP, 
	V_NT_TEXT, 
	V_NT_PARTICLE,
	V_NT_CURVE, 
	V_NT_NUM_TYPES, 
	V_NT_SYSTEM = V_NT_NUM_TYPES, 
	V_NT_NUM_TYPES_NETPACK
}VNodeType;

typedef uint32		VNodeID;
typedef uint16		VLayerID;		/* Commonly used to identify layers. */
typedef uint16		VNMFragmentID;
typedef uint16		VNMBufferID;
typedef uint16		VNPEffectID;

typedef void *		VSession;

typedef enum {
	VN_O_METHOD_PTYPE_INTEGER8 = 0,
	VN_O_METHOD_PTYPE_INTEGER16,
	VN_O_METHOD_PTYPE_INTEGER32,

	VN_O_METHOD_PTYPE_UINTEGER8,
	VN_O_METHOD_PTYPE_UINTEGER16,
	VN_O_METHOD_PTYPE_UINTEGER32,

	VN_O_METHOD_PTYPE_REAL32,
	VN_O_METHOD_PTYPE_REAL64,

	VN_O_METHOD_PTYPE_STRING,

	VN_O_METHOD_PTYPE_VNODE,
	VN_O_METHOD_PTYPE_VLAYER,

	VN_O_METHOD_PTYPE_2_VECTOR32,
	VN_O_METHOD_PTYPE_3_VECTOR32,
	VN_O_METHOD_PTYPE_4_VECTOR32,

	VN_O_METHOD_PTYPE_2_VECTOR64,
	VN_O_METHOD_PTYPE_3_VECTOR64,
	VN_O_METHOD_PTYPE_4_VECTOR64,

	VN_O_METHOD_PTYPE_4_MATRIX32,
	VN_O_METHOD_PTYPE_9_MATRIX32,
	VN_O_METHOD_PTYPE_16_MATRIX32,

	VN_O_METHOD_PTYPE_4_MATRIX64,
	VN_O_METHOD_PTYPE_9_MATRIX64,
	VN_O_METHOD_PTYPE_16_MATRIX64,
}VNOParamType;

typedef	union{
	int8		integer8;
	int16		integer16;
	int32		integer32;
	uint8		uinteger8;
	uint16		uinteger16;
	uint32		uinteger32;
	real32		real32;
	real64		real64;
	char		*string;
	VNodeID		vnode;
	VLayerID	vlayer;
	real32		vector32[4];
	real64		vector64[4];
	real32		matrix32[16];
	real64		matrix64[16];
}VNOParam;

#define VN_TAG_MAX_BLOB_SIZE 500

typedef enum {
	VN_TAG_BOOLEAN = 0,
	VN_TAG_INTEGER,
	VN_TAG_REAL,
	VN_TAG_STRING,
	VN_TAG_VECTOR,
	VN_TAG_LINK,
	VN_TAG_ANIMATION,
	VN_TAG_BLOB,
	VN_TAG_TYPE_COUNT
} VNTagType;

typedef union{
	boolean active;
	uint32	integer;
	real64	real;
	char	*string;
	real64	vector[3];
	VNodeID	link;
	struct{
		VNodeID curve;
		uint32 start;
		uint32 end;
	}animation;
	struct{
		uint16	blob_size;
		void	*blob;
	}blob;
} VNTag;

typedef enum {
	VN_S_CONNECT_NAME_SIZE = 32,
	VN_S_CONNECT_KEY_SIZE = 4,
	VN_S_CONNECT_DATA_SIZE = 32
} VNSConnectConstants;

typedef enum {
	VN_FORMAT_REAL32,
	VN_FORMAT_REAL64
} VNORealFormat;

typedef enum {
	VN_O_TAG_GROUP_SIZE = 16,
	VN_O_TAG_NAME_SIZE = 16,
	VN_O_TAG_FULL_NAME_SIZE = 64,
	VN_O_TAG_STRING_SIZE = 128
} VNOTagConstants;


typedef enum {
	VN_O_METHOD_GROUP_SIZE = 64,
	VN_O_METHOD_NAME_SIZE = 64,
	VN_O_METHOD_SIG_SIZE = 256
} VNOMethodConstants;

typedef enum {
	VN_G_LAYER_VERTEX_XYZ = 0,
	VN_G_LAYER_VERTEX_UINTEGER32,
	VN_G_LAYER_VERTEX_REAL64,
	VN_G_LAYER_POLYGON_CORNER_UINTEGER32 = 128,
	VN_G_LAYER_POLYGON_CORNER_REAL64,
	VN_G_LAYER_POLYGON_FACE_UINTEGER8,
	VN_G_LAYER_POLYGON_FACE_UINTEGER32,
	VN_G_LAYER_POLYGON_FACE_REAL64
} VNGLayerType;

typedef enum {
	VN_M_LIGHT_DIRECT = 0,
	VN_M_LIGHT_AMBIENT,
	VN_M_LIGHT_DIRECT_AND_AMBIENT,
	VN_M_LIGHT_BACK
} VNMLightType;

typedef enum {
	VN_M_NOISE_PERLIN = 0
} VNMNoiseType;

typedef enum {
	VN_M_RAMP_SQUARE = 0,
	VN_M_RAMP_LINEAR
} VNMRampType;

typedef enum {
	VN_M_RAMP_RED = 0,
	VN_M_RAMP_GREEN,
	VN_M_RAMP_BLUE
} VNMRampChannel;

typedef struct {
	double			pos;
	double			red;
	double			green;
	double			blue;
} VNMRampPoint;

typedef enum {
	VN_M_BLEND_FADE = 0,
	VN_M_BLEND_ADD,
	VN_M_BLEND_SUBTRACT,
	VN_M_BLEND_MULTIPLY,
	VN_M_BLEND_DIVIDE,
	VN_M_BLEND_DOT
} VNMBlendType;

typedef enum {
	VN_M_FT_COLOR = 0,
	VN_M_FT_LIGHT,
	VN_M_FT_REFLECTION,
	VN_M_FT_TRANSPARENCY,
	VN_M_FT_GEOMETRY,
	VN_M_FT_TEXTURE,
	VN_M_FT_TAG,
	VN_M_FT_NOISE,
	VN_M_FT_BLENDER,
	VN_M_FT_MATRIX,
	VN_M_FT_RAMP,
	VN_M_FT_ALTERNATIVE,
	VN_M_FT_OUTPUT,
} VNMFragmentType;

typedef union{
	struct{
		real64 red;
		real64 green;
		real64 blue;
	}color;
	struct{
		uint8 type;
		real64 normal_falloff; 
		VNodeID brdf;
		char brdf_r[16];
		char brdf_g[16];
		char brdf_b[16];
	}light;
	struct{
		real64 normal_falloff;
	}reflection;
	struct{
		real64 normal_falloff;
		real64 refraction_index;
	}transparency;
	struct{
		char layer_r[16];
		char layer_g[16];
		char layer_b[16];
	}geometry;
	struct{
		VNodeID bitmap;
		char layer_r[16];
		char layer_g[16];
		char layer_b[16];
		VNMFragmentID control;
	}texture;
	struct{
		char name[16];
		char group[16];
	}tag;
	struct{
		uint8 type;
		VNMFragmentID mapping;
	}noise;
	struct{
		uint8 type;
		VNMFragmentID data_a;
		VNMFragmentID data_b; 
		VNMFragmentID control;
	}blender;
	struct{
		real64 matrix[16];
		VNMFragmentID data;
	}matrix;
	struct{
		uint8 type;
		uint8 channel;
		VNMFragmentID control; 
		uint8 point_count; 
		VNMRampPoint ramp[48];
	}ramp;
	struct{
		VNMFragmentID alt_a;
		VNMFragmentID alt_b;
	}alternative;
	struct{
		char type[16];
		VNMFragmentID front;
		VNMFragmentID back;
	}output;
}VMatFrag;

typedef enum {
	VN_B_LAYER_INTEGER1 = 0,
	VN_B_LAYER_INTEGER8,
	VN_B_LAYER_INTEGER16,
	VN_B_LAYER_REAL32,
	VN_B_LAYER_REAL64
} VNBLayerType;

#define VN_B_TILE_SIZE 4

typedef enum {
	VN_C_CONTENT_LANGUAGE_SIZE = 32,
	VN_C_CONTENT_INFO_SIZE = 256,
	VN_C_BUFFER_NAME_SIZE = 32,
	VN_C_TEXT_SIZE = 256
} VNCConstants;

typedef enum {
	VN_P_FORCE_RADIAL = 0,
	VN_P_FORCE_DIRECTIONAL
} VNPForceType;

typedef enum {
	VN_P_SPACE_LOCAL = 0,
	VN_P_SPACE_GLOBAL,
	VN_P_SPACE_PARTICLE
} VNPSpace;

extern void verse_set_connect_port(uint16 port);
extern void verse_callback_set(void *send_func, void *callback, void *user_data);
extern void verse_callback_update(uint32 milliseconds);
extern void verse_session_set(VSession session);
extern void verse_session_destroy(VSession session);
extern size_t verse_session_get_size(void);

extern void *verse_pack_method_call(uint32 param_count, VNOParam *params, VNOParamType *param_type);
extern boolean verse_unpack_method_call(void *data, uint32 param_count, VNOParam *params, VNOParamType *param_type);
/*
#define V_PRINT_SEND_COMMANDS
#define V_PRINT_RECIVE_COMMANDS
*/
#endif		/* V_VLL_H */

extern VSession *verse_send_connect(const char *name, const char *pass, const char *address);
extern VSession *verse_send_connect_accept(VNodeID avatar, const char *address);
extern void verse_send_connect_deny(void *address);
extern void verse_send_connect_terminate(const char *bye);
extern void verse_send_get_time(uint32 time);
extern void verse_send_ping(const char *address, const char *text);
extern void verse_send_node_list(uint32 mask);
extern void verse_send_node_create(VNodeID node_id, VNodeType type, VNodeID owner_id);
extern void verse_send_node_destroy(VNodeID node_id);
extern void verse_send_node_subscribe(VNodeID node_id);
extern void verse_send_node_unsubscribe(VNodeID node_id);
extern void verse_send_tag_group_create(VNodeID node_id, uint16 group_id, const char *name);
extern void verse_send_tag_group_destroy(VNodeID node_id, uint16 group_id);
extern void verse_send_tag_group_subscribe(VNodeID node_id, uint16 group_id);
extern void verse_send_tag_group_unsubscribe(VNodeID node_id, uint16 group_id);
extern void verse_send_tag_create(VNodeID node_id, uint16 group_id, uint16 tag_id, const char *name, VNTagType type, VNTag *tag);
extern void verse_send_tag_destroy(VNodeID node_id, uint16 group_id, uint16 tag_id);
extern void verse_send_node_name_set(VNodeID node_id, const char *name);

extern void verse_send_o_transform_pos_real32(VNodeID node_id, uint32 time, real32 *pos, real32 *speed, real32 *accelerate, real32 *drag_normal, real32 drag);
extern void verse_send_o_transform_rot_real32(VNodeID node_id, uint32 time, real32 *rot, real32 *speed, real32 *accelerate, real32 *drag_normal, real32 drag);
extern void verse_send_o_transform_scale_real32(VNodeID node_id, real32 scale_x, real32 scale_y, real32 scale_z);
extern void verse_send_o_transform_pos_real64(VNodeID node_id, uint32 time, real64 *pos, real64 *speed, real64 *accelerate, real64 *drag_normal, real64 drag);
extern void verse_send_o_transform_rot_real64(VNodeID node_id, uint32 time, real64 *rot, real64 *speed, real64 *accelerate, real64 *drag_normal, real64 drag);
extern void verse_send_o_transform_scale_real64(VNodeID node_id, real64 scale_x, real64 scale_y, real64 scale_z);
extern void verse_send_o_transform_subscribe(VNodeID node_id, VNORealFormat type);
extern void verse_send_o_transform_unsubscribe(VNodeID node_id, VNORealFormat type);
extern void verse_send_o_set_light(VNodeID node_id, real64 light_r, real64 light_g, real64 light_b);
extern void verse_send_o_link_set(VNodeID node_id, uint16 link_id, VNodeID link, const char *name, uint32 target_id);
extern void verse_send_o_link_destroy(VNodeID node_id, uint16 link_id);
extern void verse_send_o_method_group_create(VNodeID node_id, uint8 group_id, const char *name);
extern void verse_send_o_method_group_destroy(VNodeID node_id, uint8 group_id);
extern void verse_send_o_method_group_subscribe(VNodeID node_id, uint16 group_id);
extern void verse_send_o_method_group_unsubscribe(VNodeID node_id, uint16 group_id);
extern void verse_send_o_method_create(VNodeID node_id, uint16 group_id, uint16 method_id, const char *name, uint8 param_count, VNOParamType *param_types, char * *param_names);
extern void verse_send_o_method_destroy(VNodeID node_id, uint16 group_id, uint16 method_id);
extern void verse_send_o_method_send(VNodeID node_id, uint16 group_id, uint16 method_id, VNodeID sender, void *params);

extern void verse_send_g_layer_create(VNodeID node_id, VLayerID layer_id, const char *name, VNGLayerType type, uint32 def_integer, real64 def_real);
extern void verse_send_g_layer_destroy(VNodeID node_id, VLayerID layer_id);
extern void verse_send_g_layer_subscribe(VNodeID node_id, VLayerID layer_id, VNORealFormat type);
extern void verse_send_g_layer_unsubscribe(VNodeID node_id, VLayerID layer_id);
extern void verse_send_g_vertex_set_real32_xyz(VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real32 x, real32 y, real32 z);
extern void verse_send_g_vertex_delete_real32(VNodeID node_id, uint32 vertex_id);
extern void verse_send_g_vertex_set_real64_xyz(VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real64 x, real64 y, real64 z);
extern void verse_send_g_vertex_delete_real64(VNodeID node_id, uint32 vertex_id);
extern void verse_send_g_vertex_set_uinteger32(VNodeID node_id, VLayerID layer_id, uint32 vertex_id, uint32 value);
extern void verse_send_g_vertex_set_real64(VNodeID node_id, VLayerID layer_id, uint32 vertex_id, real64 value);
extern void verse_send_g_polygon_set_corner_uinteger32(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint32 v0, uint32 v1, uint32 v2, uint32 v3);
extern void verse_send_g_polygon_delete(VNodeID node_id, uint32 polygon_id);
extern void verse_send_g_polygon_set_corner_real64(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real64 v0, real64 v1, real64 v2, real64 v3);
extern void verse_send_g_polygon_set_face_uinteger8(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint8 value);
extern void verse_send_g_polygon_set_face_uinteger32(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, uint32 value);
extern void verse_send_g_polygon_set_face_real64(VNodeID node_id, VLayerID layer_id, uint32 polygon_id, real64 value);
extern void verse_send_g_crease_set_vertex(VNodeID node_id, const char *layer, uint32 def_crease);
extern void verse_send_g_crease_set_edge(VNodeID node_id, const char *layer, uint32 def_crease);
extern void verse_send_g_bone_create(VNodeID node_id, uint32 bone_id, const char *name, uint32 parent, real64 pos_x, real64 pos_y, real64 pos_z, real64 rot_x, real64 rot_y, real64 rot_z);
extern void verse_send_g_bone_destroy(VNodeID node_id, uint32 bone_id);

extern void verse_send_m_fragment_create(VNodeID node_id, VNMFragmentID frag_id, VNMFragmentType type, VMatFrag *fragment);
extern void verse_send_m_fragment_destroy(VNodeID node_id, VNMFragmentID frag_id);

extern void verse_send_b_init_dimensions(VNodeID node_id, uint16 width, uint16 height, uint16 depth);
extern void verse_send_b_layer_create(VNodeID node_id, VLayerID layer_id, const char *name, VNBLayerType type);
extern void verse_send_b_layer_destroy(VNodeID node_id, VLayerID layer_id);
extern void verse_send_b_layer_subscribe(VNodeID node_id, VLayerID layer_id, uint8 level);
extern void verse_send_b_layer_unsubscribe(VNodeID node_id, VLayerID layer_id);
extern void verse_send_b_layer_set_tile(VNodeID node_id, VLayerID layer_id, uint16 tile_x, uint16 tile_y, uint16 z, VNBLayerType type, void *data);

extern void verse_send_t_set_language(VNodeID node_id, const char *language);
extern void verse_send_t_buffer_create(VNodeID node_id, VNMBufferID buffer_id, uint16 index, const char *name);
extern void verse_send_t_buffer_destroy(VNodeID node_id, VNMBufferID buffer_id);
extern void verse_send_t_buffer_subscribe(VNodeID node_id, VNMBufferID buffer_id);
extern void verse_send_t_buffer_unsubscribe(VNodeID node_id, VNMBufferID buffer_id);
extern void verse_send_t_insert_line(VNodeID node_id, VNMBufferID buffer_id, uint32 line, uint16 index, const char *text);
extern void verse_send_t_delete_line(VNodeID node_id, VNMBufferID buffer_id, uint32 line);

extern void verse_send_c_curve_create(VNodeID node_id, VLayerID curve_id, const char *name);
extern void verse_send_c_curve_destroy(VNodeID node_id, VLayerID curve_id);
extern void verse_send_c_curve_subscribe(VNodeID node_id, VLayerID curve_id);
extern void verse_send_c_curve_unsubscribe(VNodeID node_id, VLayerID curve_id);
extern void verse_send_c_curve_key_set(VNodeID node_id, VLayerID curve_id, uint32 key_id, real64 pre_value, uint32 pre_pos, real64 value, real64 post_value, uint32 post_pos);
extern void verse_send_c_curve_key_destroy(VNodeID node_id, VLayerID curve_id, uint32 key_id);


#endif

