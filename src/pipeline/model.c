// code for handling objects in model space
#include "model.h"

void model_add_w(vec3f* in, int num_vertices, vec4f* out)
{
    out = malloc(num_vertices * sizeof(vec4f));
    for (int i = 0; i < num_vertices; i++)
    {
        out[i].x = in[i].x;
        out[i].y = in[i].y;
        out[i].z = in[i].z;
        out[i].w = 1.0f; // set the homogenous component to 1
    }
}