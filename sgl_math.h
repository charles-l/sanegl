#ifndef SGL_MATH_H
#define SGL_MATH_H
// mostly stolen from linmath.h (https://github.com/datenwolf/linmath.h)

#define __USE_XOPEN
#include <math.h>

typedef float v2 __attribute__ ((vector_size (8)));
typedef float v3 __attribute__ ((vector_size (16))); // must be a power of 2 - last byte is unused
typedef float v4 __attribute__ ((vector_size (16)));
typedef v4 mat4x4[4];

#define GEN_VECN_FUNCTIONS(n) \
static float v##n##_mul_in(v##n a, v##n b) { \
    float p = 0.; \
    for(int i = 0; i < n; ++i) \
	p += b[i] * a[i]; \
    return p; \
} \
static float v##n##_len(v##n v) { \
    return sqrtf(v##n##_mul_in(v, v)); \
} \
static v##n v##n##_norm(v##n v) { \
    float k = 1.0 / v##n##_len(v); \
    return v * k; \
}

GEN_VECN_FUNCTIONS(2);
GEN_VECN_FUNCTIONS(3);
GEN_VECN_FUNCTIONS(4);

static v3 v3_cross(v3 a, v3 b) {
    return (v3) {
	a[1] * b[2] - a[2] * b[1],
	a[2] * b[0] - a[0] * b[2],
	a[0] * b[1] - a[1] * b[0],
    };
}

static v4 v4_cross(v4 a, v4 b) {
    return (v4) {
	a[1] * b[2] - a[2] * b[1],
	a[2] * b[0] - a[0] * b[2],
	a[0] * b[1] - a[1] * b[0],
	1.f
    };
}

static inline v4 mat4x4_row(mat4x4 M, int i)
{
    v4 r;
    int k;
    for(k=0; k<4; ++k)
	r[k] = M[k][i];
    return r;
}
static inline void mat4x4_translate_in_place(mat4x4 M, float x, float y, float z)
{
    v4 t = {x, y, z, 0};
    int i;
    for (i = 0; i < 4; ++i) {
	v4 r = mat4x4_row(M, i);
	M[3][i] += v4_mul_in(r, t);
    }
}

static inline void mat4x4_perspective(mat4x4 m, float y_fov, float aspect, float n, float f)
{
    /* NOTE: Degrees are an unhandy unit to work with. everything is radians */
    float const a = 1.f / tan(y_fov / 2.f);

    m[0][0] = a / aspect;
    m[0][1] = 0.f;
    m[0][2] = 0.f;
    m[0][3] = 0.f;

    m[1][0] = 0.f;
    m[1][1] = a;
    m[1][2] = 0.f;
    m[1][3] = 0.f;

    m[2][0] = 0.f;
    m[2][1] = 0.f;
    m[2][2] = -((f + n) / (f - n));
    m[2][3] = -1.f;

    m[3][0] = 0.f;
    m[3][1] = 0.f;
    m[3][2] = -((2.f * f * n) / (f - n));
    m[3][3] = 0.f;
}

static inline void mat4x4_look_at(mat4x4 m, v3 eye, v3 center, v3 up)
{
    /* Adapted from Android's OpenGL Matrix.java.                        */
    /* See the OpenGL GLUT documentation for gluLookAt for a description */
    /* of the algorithm. We implement it in a straightforward way:       */

    /* TODO: The negation of of can be spared by swapping the order of
     *       operands in the following cross products in the right way. */
    v3 f = center - eye;
    f = v3_norm(f);

    v3 s = v3_cross(f, up);
    s = v3_norm(s);

    v3 t = v3_cross(s, f);

    m[0][0] =  s[0];
    m[0][1] =  t[0];
    m[0][2] = -f[0];
    m[0][3] =   0.f;

    m[1][0] =  s[1];
    m[1][1] =  t[1];
    m[1][2] = -f[1];
    m[1][3] =   0.f;

    m[2][0] =  s[2];
    m[2][1] =  t[2];
    m[2][2] = -f[2];
    m[2][3] =   0.f;

    m[3][0] =  0.f;
    m[3][1] =  0.f;
    m[3][2] =  0.f;
    m[3][3] =  1.f;

    mat4x4_translate_in_place(m, -eye[0], -eye[1], -eye[2]);
}

static inline void mat4x4_dup(mat4x4 M, mat4x4 N)
{
    int i, j;
    for(i=0; i<4; ++i)
	for(j=0; j<4; ++j)
	    M[i][j] = N[i][j];
}

static inline void mat4x4_mul(mat4x4 M, mat4x4 a, mat4x4 b)
{
    mat4x4 temp;
    int k, r, c;
    for(c=0; c<4; ++c) for(r=0; r<4; ++r) {
	temp[c][r] = 0.f;
	for(k=0; k<4; ++k)
	    temp[c][r] += a[k][r] * b[c][k];
    }
    mat4x4_dup(M, temp);
}

static inline void mat4x4_diag(mat4x4 M, float v)
{
    int i, j;
    for(i=0; i<4; ++i)
	for(j=0; j<4; ++j)
	    M[i][j] = 0.0f;
    for(i=0; i<4; ++i)
	M[i][i] = v;
}

static inline double radians(double degrees) {
    return (degrees * M_PI) / 180.0;
}
#endif // SGL_MATH_H
