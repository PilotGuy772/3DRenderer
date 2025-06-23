#ifndef MODEL_H
#define MODEL_H

#include <stdlib.h>
#include "matrix.h"

typedef struct vertex
{
    vec4f position;
    vec3f normal;
    uint32_t color;
    vec3f world_pos;
} vertex;

typedef struct face
{
    int indices[3];
    uint32_t color;
    vec3f normal;
    int material;
} face;


void model_add_w(vec3f* in, int num_vertices, vec4f* out);

#endif