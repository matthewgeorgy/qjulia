#ifndef _MG_MATH_H_
#define _MG_MATH_H_

#include "mg_defs.h"
#include <math.h>

#define mg_sqrt(__n)		sqrtf(__n)
#define mg_sin(__n)        	sinf(__n)
#define mg_cos(__n)        	cosf(__n)
#define mg_tan(__n)        	tanf(__n)
#define mg_rads(__n)       	(__n * 0.017453f)
#define mg_degs(__n)       	(__n * 57.29578f)
#ifndef MG_PI
    #define MG_PI			3.1415926f
#endif // MG_PI

/* ============================ *
 * =====     Vector2D     ===== *
 * ============================ */

typedef struct _TAG_mg_vec2
{
    union
    {
        struct
        {
            f32		x,
                    y;
        };
		struct
		{
			f32		u,
					v;
		};
		struct
		{
			f32		r,
					g;
		};
        f32         elements[2];
    };
} mg_vec2_t;

MG_EXTERN mg_vec2_t		mg_vec2(f32 x, f32 y);
MG_EXTERN mg_vec2_t     mg_vec2_add(mg_vec2_t v1, mg_vec2_t v2);
MG_EXTERN mg_vec2_t     mg_vec2_sub(mg_vec2_t v1, mg_vec2_t v2);
MG_EXTERN mg_vec2_t     mg_vec2_scale(mg_vec2_t vec, f32 scalar);
MG_EXTERN f32           mg_vec2_dot(mg_vec2_t v1, mg_vec2_t v2);
MG_EXTERN f32           mg_vec2_length(mg_vec2_t vec);
MG_EXTERN mg_vec2_t		mg_vec2_normalize(mg_vec2_t vec);
MG_EXTERN mg_vec2_t     mg_vec2_rotate(mg_vec2_t vec, f32 angle);
MG_EXTERN mg_vec2_t     mg_vec2_mul(mg_vec2_t v1, mg_vec2_t v2);
MG_EXTERN mg_vec2_t     mg_vec2_div(mg_vec2_t v1, mg_vec2_t v2);

/* ============================ *
 * =====     Vector3D     ===== *
 * ============================ */

typedef struct _TAG_mg_vec3
{
    union
    {
        struct
        {
            f32     x,
                    y,
                    z;
        };
        struct
        {
            f32     u,
                    v,
                    w;
        };
        struct
        {
            f32     r,
                    g,
                    b;
        };
        f32         elements[3];
    };
} mg_vec3_t;

MG_EXTERN mg_vec3_t		mg_vec3(f32 x, f32 y, f32 z);
MG_EXTERN mg_vec3_t     mg_vec3_add(mg_vec3_t v1, mg_vec3_t v2);
MG_EXTERN mg_vec3_t     mg_vec3_sub(mg_vec3_t v1, mg_vec3_t v2);
MG_EXTERN mg_vec3_t     mg_vec3_scale(mg_vec3_t vec, f32 scalar);
MG_EXTERN f32           mg_vec3_dot(mg_vec3_t v1, mg_vec3_t v2);
MG_EXTERN mg_vec3_t     mg_vec3_cross(mg_vec3_t v1, mg_vec3_t v2);
MG_EXTERN f32           mg_vec3_length(mg_vec3_t vec);
MG_EXTERN mg_vec3_t     mg_vec3_normalize(mg_vec3_t vec);
MG_EXTERN mg_vec3_t     mg_vec3_mul(mg_vec3_t v1, mg_vec3_t v2);
MG_EXTERN mg_vec3_t     mg_vec3_div(mg_vec3_t v1, mg_vec3_t v3);

/* ============================ *
 * =====     Vector4D     ===== *
 * ============================ */

typedef struct _TAG_mg_vec4
{
    union
    {
        struct
        {
            f32     x,
                    y,
                    z,
                    w;
        };
        struct
        {
            f32     r,
                    g,
                    b,
					a;
        };
        f32         elements[4];
    };
} mg_vec4_t;

MG_EXTERN mg_vec4_t		mg_vec4(f32 x, f32 y, f32 z, f32 w);
MG_EXTERN mg_vec4_t     mg_vec4_add(mg_vec4_t v1, mg_vec4_t v2);
MG_EXTERN mg_vec4_t     mg_vec4_sub(mg_vec4_t v1, mg_vec4_t v2);
MG_EXTERN mg_vec4_t     mg_vec4_scale(mg_vec4_t vec, f32 scalar);
MG_EXTERN f32           mg_vec4_dot(mg_vec4_t v1, mg_vec4_t v2);
MG_EXTERN f32           mg_vec4_length(mg_vec4_t vec);
MG_EXTERN mg_vec4_t     mg_vec4_normalize(mg_vec4_t vec);
MG_EXTERN mg_vec4_t     mg_vec4_mul(mg_vec4_t v1, mg_vec4_t v2);
MG_EXTERN mg_vec4_t     mg_vec4_div(mg_vec4_t v1, mg_vec4_t v4);

/* ============================ *
 * =====     Matrix4      ===== *
 * ============================ */

typedef struct _TAG_mg_mat4
{
    union
    {
        struct
        {
            mg_vec4_t		col1,
                            col2,
                            col3,
                            col4;
        };
        f32                 elements[16];
    };
} mg_mat4_t;

MG_EXTERN mg_mat4_t		mg_mat4_identity(void);
MG_EXTERN mg_mat4_t     mg_mat4_translate(f32 x, f32 y, f32 z);
MG_EXTERN mg_mat4_t     mg_mat4_translate_v(mg_vec3_t vec);
MG_EXTERN mg_mat4_t     mg_mat4_translate_remove(mg_mat4_t matrix);
MG_EXTERN void          mg_mat4_print(mg_mat4_t matrix);
MG_EXTERN mg_mat4_t     mg_mat4_rotate(f32 angle, f32 x, f32 y, f32 z);
MG_EXTERN mg_mat4_t     mg_mat4_rotate_v(f32 angle, mg_vec3_t vec);
MG_EXTERN mg_mat4_t     mg_mat4_perspective_rh(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane);
MG_EXTERN mg_mat4_t     mg_mat4_perspective_lh(f32 fov, f32 aspect_ratio, f32 near_plane, f32 far_plane);
MG_EXTERN mg_mat4_t		mg_mat4_ortho_lh(f32 view_width, f32 view_height, f32 near_plane, f32 far_plane);
MG_EXTERN mg_mat4_t     mg_mat4_lookat_rh(mg_vec3_t eye, mg_vec3_t center, mg_vec3_t up);
MG_EXTERN mg_mat4_t     mg_mat4_lookat_lh(mg_vec3_t eye, mg_vec3_t center, mg_vec3_t up);
MG_EXTERN mg_mat4_t     mg_mat4_scale(f32 scale_value);
MG_EXTERN mg_mat4_t     mg_mat4_scale_v(mg_vec3_t scale);
MG_EXTERN mg_mat4_t     mg_mat4_mul(mg_mat4_t m1, mg_mat4_t m2);

/* ============================ *
 * =====    Quaternion    ===== *
 * ============================ */

#if 0
// Definition of a quaternion
typedef struct _TAG_mg_quat
{
    union
    {
        struct
        {
            f32     w,
                    i,
                    j,
                    k;
        };
		struct
		{
			vec3_t		axis;
			f32			angle;
		};
        f32         elements[4];
    };
} mg_quat_t;

// Constructs a quaternion given w, i, j, k components
MG_EXTERN mg_quat_t		mg_quat(f32 w, f32 i, f32 j, f32 k);

// Computes the sum of two quaternions
// (q1 + q2)
MG_EXTERN mg_quat_t     mg_quat_add(mg_quat_t q1, mg_quat_t q2);

// Computes the difference of two quaternions
// (q1 - q2)
MG_EXTERN mg_quat_t     mg_quat_sub(mg_quat_t q1, mg_quat_t q2);

// Computes the product of two quaternions
// (q1 * q2)
MG_EXTERN mg_quat_t     mg_quat_mul(mg_quat_t q1, mg_quat_t q2);

// Computes the conjugate of a quaternion
// (w - i - j - k)
MG_EXTERN mg_quat_t     mg_quat_conj(mg_quat_t q);

// Computes the length (norm) of a quaternion
MG_EXTERN f32			mg_quat_length(mg_quat_t q);

// Scales a quaternion by a given factor
MG_EXTERN mg_quat_t		mg_quat_scale(mg_quat_t q, f32 scale);

// Normalizes a quaternion
MG_EXTERN mg_quat_t		mg_quat_normalize(mg_quat_t q);

// Constructs a rotation matrix from a quaternion
MG_EXTERN mg_mat4_t		mg_quat_to_mat4(mg_quat_t q);

#endif


#ifdef MG_IMPL

/////////////////////////////
// VECTOR2D IMPLEMENTATION
/////////////////////////////

mg_vec2_t
mg_vec2(f32 x,
        f32 y)
{
    mg_vec2_t      	vec;


    vec.x = x;
    vec.y = y;

    return vec;
}

mg_vec2_t
mg_vec2_add(mg_vec2_t v1,
            mg_vec2_t v2)
{
    v1.x += v2.x;
    v1.y += v2.y;

    return v1;
}

mg_vec2_t
mg_vec2_sub(mg_vec2_t v1,
            mg_vec2_t v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;

    return v1;
}

mg_vec2_t
mg_vec2_scale(mg_vec2_t vec,
             f32 scalar)
{
    vec.x *= scalar;
    vec.y *= scalar;

    return vec;
}

f32
mg_vec2_dot(mg_vec2_t v1,
            mg_vec2_t v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y));
}

f32
mg_vec2_length(mg_vec2_t vec)
{
    return mg_sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

mg_vec2_t
mg_vec2_normalize(mg_vec2_t vec)
{
    f32     val;


    val = 1 / mg_sqrt((vec.x * vec.x) + (vec.y * vec.y));
    vec.x *= val;
    vec.y *= val;

    return vec;
}

mg_vec2_t
mg_vec2_rotate(mg_vec2_t vec,
               f32 angle)
{
    f32     c,
            s;


    c = mg_cos(mg_rads(angle));
    s = mg_sin(mg_rads(angle));
    vec.x = ((vec.x * c) - (vec.y * s));
    vec.y = ((vec.x * s) + (vec.y * c));

    return vec;
}

mg_vec2_t
mg_vec2_mul(mg_vec2_t v1,
            mg_vec2_t v2)
{
    mg_vec2_t      	res;


    res.x = v1.x * v2.x;
    res.y = v1.y * v2.y;

    return res;
}

mg_vec2_t
mg_vec2_div(mg_vec2_t v1,
            mg_vec2_t v2)
{
    mg_vec2_t      	res;


    res.x = v1.x / v2.x;
    res.y = v1.y / v2.y;

    return res;
}

/////////////////////////////
// VECTOR3D IMPLEMENTATION
/////////////////////////////

mg_vec3_t
mg_vec3(f32 x,
        f32 y,
        f32 z)
{
    mg_vec3_t      	vec;


    vec.x = x;
    vec.y = y;
    vec.z = z;

    return vec;
}

mg_vec3_t
mg_vec3_add(mg_vec3_t v1,
            mg_vec3_t v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;

    return v1;
}

mg_vec3_t
mg_vec3_sub(mg_vec3_t v1,
            mg_vec3_t v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;

    return v1;
}

mg_vec3_t
mg_vec3_scale(mg_vec3_t vec,
              f32 scalar)
{
    vec.x *= scalar;
    vec.y *= scalar;
    vec.z *= scalar;

    return vec;
}

f32
mg_vec3_dot(mg_vec3_t v1,
            mg_vec3_t v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

mg_vec3_t
mg_vec3_cross(mg_vec3_t v1,
              mg_vec3_t v2)
{
    mg_vec3_t      	cross_prod;


    cross_prod.x = (v1.y * v2.z) - (v1.z * v2.y);
    cross_prod.y = (v1.z * v2.x) - (v1.x * v2.z);
    cross_prod.z = (v1.x * v2.y) - (v1.y * v2.x);

    return cross_prod;
}

f32
mg_vec3_length(mg_vec3_t vec)
{
    return mg_sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
}

mg_vec3_t
mg_vec3_normalize(mg_vec3_t vec)
{
    f32     val;


    val = 1 / mg_sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
    vec.x *= val;
    vec.y *= val;
    vec.z *= val;

    return vec;
}

mg_vec3_t
mg_vec3_mul(mg_vec3_t v1,
            mg_vec3_t v2)
{
    mg_vec3_t      	res;


    res.x = v1.x * v2.x;
    res.y = v1.y * v2.y;
    res.z = v1.z * v2.z;

    return res;
}

mg_vec3_t
mg_vec3_div(mg_vec3_t v1,
            mg_vec3_t v2)
{
    mg_vec3_t      	res;


    res.x = v1.x / v2.x;
    res.y = v1.y / v2.y;
    res.z = v1.z / v2.z;

    return res;
}

/////////////////////////////
// VECTOR4D IMPLEMENTATION
/////////////////////////////

mg_vec4_t
mg_vec4(f32 x,
        f32 y,
        f32 z,
        f32 w)
{
    mg_vec4_t		vec;


    vec.x = x;
    vec.y = y;
    vec.z = z;
    vec.w = w;

    return vec;
}

mg_vec4_t
mg_vec4_add(mg_vec4_t v1,
            mg_vec4_t v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    v1.z += v2.z;
    v1.w += v2.w;

    return v1;
}

mg_vec4_t
mg_vec4_sub(mg_vec4_t v1,
            mg_vec4_t v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    v1.z -= v2.z;
    v1.w -= v2.w;

    return v1;
}

mg_vec4_t
mg_vec4_scale(mg_vec4_t vec,
              f32 scalar)
{
    vec.x *= scalar;
    vec.y *= scalar;
    vec.z *= scalar;
    vec.w *= scalar;

    return vec;
}

f32
mg_vec4_dot(mg_vec4_t v1,
            mg_vec4_t v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z) + (v1.w * v2.w));
}

f32
mg_vec4_mag(mg_vec4_t vec)
{
    return mg_sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) + (vec.w * vec.w));
}

mg_vec4_t
mg_vec4_normalize(mg_vec4_t vec)
{
    f32     val;


    val = 1/ mg_sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) + (vec.w * vec.w));
    vec.x *= val;
    vec.y *= val;
    vec.z *= val;
    vec.w *= val;

    return vec;
}

mg_vec4_t
mg_vec4_mul(mg_vec4_t v1,
            mg_vec4_t v2)
{
    mg_vec4_t      	res;


    res.x = v1.x * v2.x;
    res.y = v1.y * v2.y;
    res.z = v1.z * v2.z;
    res.w = v1.w * v2.w;

    return res;
}

mg_vec4_t
mg_vec4_div(mg_vec4_t v1,
            mg_vec4_t v2)
{
    mg_vec4_t		res;


    res.x = v1.x / v2.x;
    res.y = v1.y / v2.y;
    res.z = v1.z / v2.z;
    res.w = v1.w / v2.w;

    return res;
}

//////////////////////////////
// MATRIX4 IMPLEMENTATION
//////////////////////////////

mg_mat4_t
mg_mat4_identity(void)
{
    mg_mat4_t      	matrix = {0};


    matrix.col1.x = 1.0f;
    matrix.col2.y = 1.0f;
    matrix.col3.z = 1.0f;
    matrix.col4.w = 1.0f;

    return matrix;
}

mg_mat4_t
mg_mat4_translate(f32 x,
                  f32 y,
                  f32 z)
{
    mg_mat4_t      	matrix;


    matrix = mg_mat4_identity();
    matrix.col4.x = x;
    matrix.col4.y = y;
    matrix.col4.z = z;

    return matrix;
}

mg_mat4_t
mg_mat4_translate_v(mg_vec3_t vec)
{
    mg_mat4_t		matrix;


    matrix = mg_mat4_identity();
    matrix.col4.x = vec.x;
    matrix.col4.y = vec.y;
    matrix.col4.z = vec.z;

    return matrix;
}

mg_mat4_t
mg_mat4_translate_remove(mg_mat4_t matrix)
{
    matrix.col4.x = 0.0f;
    matrix.col4.y = 0.0f;
    matrix.col4.z = 0.0f;

    return matrix;
}

/* void */
/* mg_mat4_print(mg_mat4_t matrix) */
/* { */
/*     u8      i, */
/*             j; */


/*     for (i = 0; i < 4; i++) */
/*     { */
/*         for (j = 0; j < 4; j++) */
/*         { */
/*             printf("%f ", matrix.elements[i + j * 4]); */
/*         } */
/*         printf("\n"); */
/*     } */
/* } */

mg_mat4_t
mg_mat4_rotate(f32 angle,
               f32 x,
               f32 y,
               f32 z)
{
    f32             c,
                    s,
                    c1;
    mg_vec3_t      	vec = {x, y, z};
    mg_mat4_t      	matrix = {0};


    c = mg_cos(mg_rads(angle));
    s = mg_sin(mg_rads(angle));
    c1 = 1.0f - c;
    vec = mg_vec3_normalize(vec);

    matrix.col1.x = (c1 * vec.x * vec.x) + c;
    matrix.col1.y = (c1 * vec.x * vec.y) + s * vec.z;
    matrix.col1.z = (c1 * vec.x * vec.z) - s * vec.y;

    matrix.col2.x = (c1 * vec.x * vec.y) - s * vec.z;
    matrix.col2.y = (c1 * vec.y * vec.y) + c;
    matrix.col2.z = (c1 * vec.y * vec.z) + s * vec.x;

    matrix.col3.x = (c1 * vec.x * vec.z) + s * vec.y;
    matrix.col3.y = (c1 * vec.y * vec.z) - s * vec.x;
    matrix.col3.z = (c1 * vec.z * vec.z) + c;

    matrix.col4.w = 1.0f;

    return matrix;
}

mg_mat4_t
mg_mat4_rotate_v(f32 angle,
                 mg_vec3_t vec)
{

    f32             c,
                    s,
                    c1;
    mg_mat4_t		matrix = {0};


    c = mg_cos(mg_rads(angle));
    s = mg_sin(mg_rads(angle));
    c1 = 1.0f - c;
    vec = mg_vec3_normalize(vec);

    matrix.col1.x = (c1 * vec.x * vec.x) + c;
    matrix.col1.y = (c1 * vec.x * vec.y) + s * vec.z;
    matrix.col1.z = (c1 * vec.x * vec.z) - s * vec.y;

    matrix.col2.x = (c1 * vec.x * vec.y) - s * vec.z;
    matrix.col2.y = (c1 * vec.y * vec.y) + c;
    matrix.col2.z = (c1 * vec.y * vec.z) + s * vec.x;

    matrix.col3.x = (c1 * vec.x * vec.z) + s * vec.y;
    matrix.col3.y = (c1 * vec.y * vec.z) - s * vec.x;
    matrix.col3.z = (c1 * vec.z * vec.z) + c;

    matrix.col4.w = 1.0f;

    return matrix;
}

mg_mat4_t
mg_mat4_perspective_rh(f32 fov,
                       f32 aspect_ratio,
                       f32 near_plane,
                       f32 far_plane)
{
    f32            t,
                   fdelta;
    mg_mat4_t      matrix = {0};


    t = mg_tan(mg_rads(fov) * 0.5f);
    fdelta = far_plane - near_plane;

    matrix.col1.x = 1 / (aspect_ratio * t);
    matrix.col2.y = 1 / t;
    matrix.col3.z = -1 * ((far_plane + near_plane) / fdelta);
    matrix.col3.w = -1;
    matrix.col4.z = ((-2.0f * far_plane * near_plane) / fdelta);

    return matrix;
}

mg_mat4_t
mg_mat4_perspective_lh(f32 fov,
                       f32 aspect_ratio,
                       f32 near_plane,
                       f32 far_plane)
{
    f32            t,
                   frange;
    mg_mat4_t      matrix = {0};


    t = mg_tan(mg_rads(fov) * 0.5f);
    frange = far_plane / (far_plane - near_plane);

    matrix.col1.x = 1 / (aspect_ratio * t);
    matrix.col2.y = 1 / t;
    matrix.col3.z = frange;
    matrix.col3.w = 1;
    matrix.col4.z = -frange * near_plane;

    return matrix;
}

mg_mat4_t		
mg_mat4_ortho_lh(f32 view_width, 
				 f32 view_height, 
				 f32 near_plane, 
				 f32 far_plane)
{
	f32				frange;
	mg_mat4_t		matrix = {0};


	frange = 1.0f / (far_plane - near_plane);

	matrix.col1.x = 2.0f / view_width;
	matrix.col2.y = 2.0f / view_height;
	matrix.col3.z = frange;
	matrix.col4.z = -frange * near_plane;
	matrix.col4.w = 1.0f;

	return (matrix);
}

mg_mat4_t
mg_mat4_lookat_rh(mg_vec3_t eye,
                   mg_vec3_t center,
                   mg_vec3_t up)
{
    mg_mat4_t      	matrix = {0};
    mg_vec3_t      	f,
                    s,
                    u;


    f = mg_vec3_normalize(mg_vec3_sub(center, eye));
    s = mg_vec3_normalize(mg_vec3_cross(f, up));
    u = mg_vec3_cross(s, f);

    matrix.col1.x = s.x;
    matrix.col1.y = u.x;
    matrix.col1.z = -f.x;

    matrix.col2.x = s.y;
    matrix.col2.y = u.y;
    matrix.col2.z = -f.y;

    matrix.col3.x = s.z;
    matrix.col3.y = u.z;
    matrix.col3.z = -f.z;

    matrix.col4.x = -mg_vec3_dot(s, eye);
    matrix.col4.y = -mg_vec3_dot(u, eye);
    matrix.col4.z = mg_vec3_dot(f, eye);

    matrix.col4.w = 1.0f;

    return matrix;
}

mg_mat4_t
mg_mat4_lookat_lh(mg_vec3_t eye,
                   mg_vec3_t center,
                   mg_vec3_t up)
{
    mg_mat4_t     	matrix = {0};
    mg_vec3_t      	f,
                    s,
                    u;


    f = mg_vec3_normalize(mg_vec3_sub(center, eye));
    s = mg_vec3_normalize(mg_vec3_cross(up, f));
    u = mg_vec3_cross(f, s);

    matrix.col1.x = s.x;
    matrix.col1.y = u.x;
    matrix.col1.z = f.x;

    matrix.col2.x = s.y;
    matrix.col2.y = u.y;
    matrix.col2.z = f.y;

    matrix.col3.x = s.z;
    matrix.col3.y = u.z;
    matrix.col3.z = f.z;

    matrix.col4.x = -mg_vec3_dot(s, eye);
    matrix.col4.y = -mg_vec3_dot(u, eye);
    matrix.col4.z = -mg_vec3_dot(f, eye);

    matrix.col4.w = 1.0f;

    return matrix;
}


mg_mat4_t
mg_mat4_scale(f32 scale_value)
{
    mg_mat4_t      	matrix = {0};


    matrix.col1.x = scale_value;
    matrix.col2.y = scale_value;
    matrix.col3.z = scale_value;
    matrix.col4.w = 1.0f;

    return matrix;
}

mg_mat4_t
mg_mat4_scale_v(mg_vec3_t scale)
{
    mg_mat4_t      	matrix = {0};


    matrix.col1.x = scale.x;
    matrix.col2.y = scale.y;
    matrix.col3.z = scale.z;
    matrix.col4.w = 1.0f;

    return matrix;
}

// eliminate annoying warning about res being uninitialized
#pragma warning(disable: 4701)
mg_mat4_t
mg_mat4_mul(mg_mat4_t m1,
             mg_mat4_t m2)
{
    mg_mat4_t     	res;
    u8              y,
                    x,
                    e;
    f32             sum;


    for (y = 0; y < 4; y++)
    {
        for (x = 0; x < 4; x++)
        {
            sum = 0.0f;
            for (e = 0; e < 4; e++)
            {
                sum += m1.elements[x + e * 4] * m2.elements[e + y * 4];
            }

            res.elements[x + y * 4] = sum;
        }
    }

    return res;
}
#pragma warning(default: 4701)


#endif // MG_IMPL

#endif // _MG_MATH_H_

