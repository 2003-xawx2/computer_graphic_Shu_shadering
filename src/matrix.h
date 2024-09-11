#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef struct
{
    float m[4][4];
} mat44_t;

mat44_t mat4_identity();
mat44_t mat4_make_scale(float sx, float sy, float sz);
mat44_t mat4_make_translation(float tx, float ty, float tz);
mat44_t mat4_make_rotate(float rx, float ry, float rz);
mat44_t mat4_make_perspective(float fov, float aspect, float z_near, float z_far);
vec4_t mat4_mul_vec4_project(mat44_t proj, vec4_t vec);

vec4_t mat4_mul_vec4(vec4_t vec, mat44_t mat);
mat44_t mat4_mul_mat4(mat44_t a, mat44_t b);

#endif