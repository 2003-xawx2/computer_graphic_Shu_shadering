#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vector.h"
#include "matrix.h"

typedef struct{
    vec3_t position;
    vec3_t scale;
    vec3_t rotation; // Elur angles
} transform_t;

mat44_t get_transform_matrix(transform_t tran);

#endif