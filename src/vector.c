#include <math.h>
#include "vector.h"

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)};
    return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)};
    return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z};
    return rotated_vector;
}

float vec2_length(vec2_t vec)
{
    return sqrtf(powf(vec.x, 2) + powf(vec.y, 2));
}

vec2_t vec2_add(vec2_t v1, vec2_t v2)
{
    vec2_t v;
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    return v;
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2)
{
    vec2_t v;
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    return v;
}

vec2_t vec2_mul(vec2_t v1, float m)
{
    vec2_t v;
    v.x = v1.x * m;
    v.y = v1.y * m;
    return v;
}

float vec2_dot(vec2_t v1, vec2_t v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

vec2_t vec2_normalize(vec2_t v1)
{
    return vec2_mul(v1, (float)1 / vec2_length(v1));
}

float vec3_length(vec3_t vec)
{
    return sqrtf(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
}

vec3_t vec3_add(vec3_t v1, vec3_t v2)
{
    vec3_t v;
    v.x = v1.x + v2.x;
    v.y = v1.y + v2.y;
    v.z = v1.z + v2.z;
    return v;
}

vec3_t vec3_sub(vec3_t v1, vec3_t v2)
{
    vec3_t v;
    v.x = v1.x - v2.x;
    v.y = v1.y - v2.y;
    v.z = v1.z - v2.z;
    return v;
}

vec3_t vec3_mul(vec3_t v1, float m)
{
    vec3_t v;
    v.x = v1.x * m;
    v.y = v1.y * m;
    v.z = v1.z * m;
    return v;
}

float vec3_dot(vec3_t v1, vec3_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3_t vec3_cross(vec3_t v1, vec3_t v2)
{
    vec3_t v;
    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;
    return v;
}

vec3_t vec3_normalize(vec3_t v1)
{
    return vec3_mul(v1, (float)1 / vec3_length(v1));
}

vec3_t vec3(vec4_t vec)
{
    vec3_t v = {
        .x = vec.x,
        .y = vec.y,
        .z = vec.z,
    };
    return v;
}

vec4_t vec4(vec3_t vec)
{
    vec4_t v = {
        .x = vec.x,
        .y = vec.y,
        .z = vec.z,
        .w = 1,
    };
    return v;
}

