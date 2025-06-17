#include "world.h"
#include "matrix.h"

void world_from_model(vec3* vertices, int num_vertices, mat4 transform, vec3* out_vertices)
{
    for (int i = 0; i < num_vertices; i++)
    {
        mat4_transform_vec3(transform, &vertices[i], &out_vertices[i]);
    }
}