#ifndef CULLING_H
#define CULLING_H

#include <stdint.h>
#include "matrix.h"
#include "model.h"

#define MAX_VERTS_PER_TRI 9

typedef struct vec3f {
    float x, y, z;
} vec3f;

typedef struct vec4f {
    float x, y, z, w;
} vec4f;

typedef struct vertex {
    vec4f position;   // clip space position
    vec3f normal;     // normal vector
    uint32_t color;   // packed RGBA color
    vec3f world_pos;  // world-space position
} vertex;

typedef struct face {
    int indices[3];   // indices into the vertex buffer
    uint32_t color;   // packed RGBA color
    vec3f normal;     // optional flat normal
    int material;     // material ID
} face;

// Cull and clip triangles against the view frustum
void culling_cull_triangle(vertex* vertices, int num_vertices, face* faces, int num_faces,
                           vertex** out_vertices, int* out_num_vertices,
                           face** out_faces, int* out_num_faces);

#endif // CULLING_H
