#include <stdlib.h>
#include <stdio.h>
#include "mesh.h"
#include "array.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    {
        .position = {.x = 0, .y = 0, .z = 0},
        .scale = {.x = 1, .y = 1, .z = 1},
        .rotation = {.x = 0, .y = 0, .z = 0},
    }};

void load_mesh_from_obj(mesh_t *mesh, char *file_path)
{
    // clear the mesh
    array_free((*mesh).vertices);
    array_free((*mesh).faces);
    (*mesh).vertices = NULL;
    (*mesh).faces = NULL;
    // open the file
    FILE *obj;
    obj = fopen(file_path, "r");
    if (!obj)
        fprintf(stderr, "Error:open the obj file!\n");
    // move to the end of the file and get the file length
    fseek(obj, 0, SEEK_END);
    long length = ftell(obj);
    fseek(obj, 0, SEEK_SET);
    // create buffer and read the file
    char *buffer = malloc(length + 1);
    if (!buffer)
        fprintf(stderr, "Error:buffer the obj!\n");
    fread(buffer, 1, length, obj);
    // read the string to mesh
    for (int i = 0; i < length + 1; i++)
    {
        switch (buffer[i])
        {
        case 'v':
        {
            if (buffer[i + 1] != ' ')
                break;

            vec3_t vertex;
            sscanf(buffer + i, "v %f %f %f",
                   &vertex.x,
                   &vertex.y,
                   &vertex.z);
            array_push((*mesh).vertices, vertex);
            break;
        }
        case 'f':
        {
            if (buffer[i + 1] != ' ')
                break;

            face_t face;
            sscanf(buffer + i, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
                   &face.a,
                   &face.b,
                   &face.c);
            array_push((*mesh).faces, face);
            break;
        }
        }
    }
    // free resources
    free(buffer);
    fclose(obj);
}
