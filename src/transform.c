#include "transform.h"

mat44_t get_transform_matrix(transform_t tran)
{
    mat44_t scale_matrix = mat4_make_scale(tran.scale.x, tran.scale.y, tran.scale.z);
    mat44_t rotation_matrix = mat4_make_rotate(tran.rotation.x, tran.rotation.y, tran.rotation.z);
    mat44_t translation_matrix = mat4_make_translation(tran.position.x, tran.position.y, tran.position.z);

    mat44_t result = mat4_mul_mat4(mat4_mul_mat4(translation_matrix, rotation_matrix), scale_matrix);
    return result;
}