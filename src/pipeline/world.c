#include "world.h"
#include "matrix.h"

void world_from_model(vertex* vertices, int num_vertices, mat4 transform, vertex* out_vertices)
{
    for (int i = 0; i < num_vertices; i++)
    {
        mat4_transform_vec4f(transform, vertices[i].position, &out_vertices[i].position);
        mat4_transform_vec3f(transform, &vertices[i].normal, &out_vertices[i].normal);
    }
}