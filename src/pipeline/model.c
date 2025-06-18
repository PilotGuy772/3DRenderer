// code for handling objects in model space
#include "model.h"

void model_add_w(vec3f* in, int num_vertices, vec4f* out)
{
    printf("Original vertices:\n");
    for (int i = 0; i < num_vertices; i++)
    {
        printf("Vertex %d: (%f, %f, %f)\n", i, in[i].x, in[i].y, in[i].z);
    }
    //out = malloc(num_vertices * sizeof(vec4f));
    for (int i = 0; i < num_vertices; i++)
    {
        out[i].x = in[i].x;
        out[i].y = in[i].y;
        out[i].z = in[i].z;
        out[i].w = 1.0f; // set the homogenous component to 1
    }

    printf("Vertices with homogenous component:\n");
    for (int i = 0; i < num_vertices; i++)
    {
        printf("Vertex %d: (%f, %f, %f, %f)\n", i, out[i].x, out[i].y, out[i].z, out[i].w);
    }
}