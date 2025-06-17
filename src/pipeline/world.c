#include "world.h"
#include "matrix.h"

void world_from_model(vec4f* vertices, int num_vertices, mat4 transform, vec4f* out_vertices)
{
    for (int i = 0; i < num_vertices; i++)
    {
        mat4_transform_vec4f(transform, &vertices[i], &out_vertices[i]);
    }
}