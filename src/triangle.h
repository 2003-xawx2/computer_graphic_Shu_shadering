#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include "texture.h"

typedef struct
{
    int a;
    int b;
    int c;
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
} face_t;

typedef struct
{
    vec2_t points[3];
    tex2_t tex_coords[3];
    vec3_t normal;
    float avg_depth;
} triangle_t;

#endif