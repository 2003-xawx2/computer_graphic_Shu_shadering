#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "transform.h"

typedef struct
{
    vec3_t *vertices;
    face_t *faces;
    transform_t tran;
} mesh_t;

extern mesh_t mesh;

void load_mesh_from_obj(mesh_t *mesh, char *file_path);

#endif