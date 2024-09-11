#include "matrix.h"
#include <math.h>

#define MAT4_I (mat4_identity())

mat44_t mat4_identity()
{
    mat44_t m = {.m = {
                     {1, 0, 0, 0},
                     {0, 1, 0, 0},
                     {0, 0, 1, 0},
                     {0, 0, 0, 1},
                 }};
    return m;
}

mat44_t mat4_make_scale(float sx, float sy, float sz)
{
    mat44_t m = {.m = {
                     {sx, 0, 0, 0},
                     {0, sy, 0, 0},
                     {0, 0, sz, 0},
                     {0, 0, 0, 1},
                 }};
    return m;
}

mat44_t mat4_make_translation(float tx, float ty, float tz)
{
    mat44_t m = {.m = {
                     {1, 0, 0, tx},
                     {0, 1, 0, ty},
                     {0, 0, 1, tz},
                     {0, 0, 0, 1},
                 }};
    return m;
}

mat44_t mat4_make_rotate(float rx, float ry, float rz)
{
    mat44_t mx = {.m = {
                      {cos(rx), -sin(rx), 0, 0},
                      {sin(rx), cos(rx), 0, 0},
                      {0, 0, 1, 0},
                      {0, 0, 0, 1},
                  }};
    mat44_t my = {.m = {
                      {1, 0, 0, 0},
                      {0, cos(ry), -sin(ry), 0},
                      {0, sin(ry), cos(ry), 0},
                      {0, 0, 0, 1},
                  }};
    mat44_t mz = {.m = {
                      {cos(rz), 0, sin(rz), 0},
                      {0, 1, 0, 0},
                      {-sin(rz), 0, cos(rz), 0},
                      {0, 0, 0, 1},
                  }};

    mat44_t result = mat4_mul_mat4(mat4_mul_mat4(mx, my), mz);

    return result;
}

mat44_t mat4_make_perspective(float fov, float aspect, float z_near, float z_far)
{
    float fov_s = tan(fov / 2.0);
    float z_nor_s = z_far / (z_far - z_near);

    mat44_t m = {.m = {
                     {aspect * fov_s, 0, 0, 0},
                     {0, fov_s, 0, 0},
                     {0, 0, z_nor_s, -z_nor_s * z_near},
                     {0, 0, 1, 0},
                 }};

    return m;
}

vec4_t mat4_mul_vec4_project(mat44_t proj, vec4_t vec)
{
    vec4_t result = mat4_mul_vec4(vec, proj);

    // persform the perspective by dividing the original z
    if (result.w != 0)
    {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }
    return result;
}

vec4_t mat4_mul_vec4(vec4_t vec, mat44_t mat)
{
    vec4_t v = {
        .x = vec.x * mat.m[0][0] + vec.y * mat.m[0][1] + vec.z * mat.m[0][2] + vec.w * mat.m[0][3],
        .y = vec.x * mat.m[1][0] + vec.y * mat.m[1][1] + vec.z * mat.m[1][2] + vec.w * mat.m[1][3],
        .z = vec.x * mat.m[2][0] + vec.y * mat.m[2][1] + vec.z * mat.m[2][2] + vec.w * mat.m[2][3],
        .w = vec.x * mat.m[3][0] + vec.y * mat.m[3][1] + vec.z * mat.m[3][2] + vec.w * mat.m[3][3],
    };

    return v;
}

mat44_t mat4_mul_mat4(mat44_t a, mat44_t b)
{
    mat44_t result = {0}; // 初始化结果矩阵为0
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return result;
}
