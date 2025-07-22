#ifndef FRAGMENT_H
#define FRAGMENT_H

#include "matrix.h"
#include "model.h"
#include "drawer.h"

typedef struct fragment
{
    vec2 screen_coord;
    float depth;
    uint32_t color;
    vec3f normal;
} fragment;

void draw_fragments(vertex* vertices, int num_vertices, face* faces, int num_faces, fragment* out_fragments, int* num_fragments);
void fragment_line(vertex a, vertex b);


#endif