typedef mg_vec2_t			vec2_t;
typedef mg_vec3_t			vec3_t;
typedef mg_vec4_t			vec4_t;
typedef mg_mat4_t			mat4_t;
typedef mg_handle_t			handle_t;
/* typedef mg_linheap_t		linheap_t; */
/* typedef mg_blkheap_t		blkheap_t; */

#define vec2						mg_vec2
#define vec2_add                    mg_vec2_add
#define vec2_sub                    mg_vec2_sub
#define vec2_scale                  mg_vec2_scale
#define vec2_dot                    mg_vec2_dot
#define vec2_length                 mg_vec2_length
#define vec2_normalize              mg_vec2_normalize
#define vec2_rotate                 mg_vec2_rotate
#define vec2_mul                    mg_vec2_mul
#define vec2_div                    mg_vec2_div

#define vec3                   		mg_vec3
#define vec3_add                    mg_vec3_add
#define vec3_sub                    mg_vec3_sub
#define vec3_scale                  mg_vec3_scale
#define vec3_dot                    mg_vec3_dot
#define vec3_cross                  mg_vec3_cross
#define vec3_length                 mg_vec3_length
#define vec3_normalize              mg_vec3_normalize
#define vec3_mul                    mg_vec3_mul
#define vec3_div                    mg_vec3_div

#define vec4                        mg_vec4
#define vec4_add                    mg_vec4_add
#define vec4_sub                    mg_vec4_sub
#define vec4_scale                  mg_vec4_scale
#define vec4_dot                    mg_vec4_dot
#define vec4_length         		mg_vec4_length
#define vec4_normalize				mg_vec4_normalize
#define vec4_mul                    mg_vec4_mul
#define vec4_div                    mg_vec4_div

#define mat4_identity               mg_mat4_identity
#define mat4_translate              mg_mat4_translate
#define mat4_translate_v            mg_mat4_translate_v
#define mat4_translate_remove       mg_mat4_translate_remove
#define mat4_print                  mg_mat4_print
#define mat4_rotate                 mg_mat4_rotate
#define mat4_rotate_v               mg_mat4_rotate_v
#define mat4_perspective_rh         mg_mat4_perspective_rh
#define mat4_perspective_lh         mg_mat4_perspective_lh
#define mat4_ortho_lh				mg_mat4_ortho_lh
#define mat4_lookat_rh              mg_mat4_lookat_rh
#define mat4_lookat_lh              mg_mat4_lookat_lh
#define mat4_scale                  mg_mat4_scale
#define mat4_scale_v                mg_mat4_scale_v
#define mat4_mul                    mg_mat4_mul

