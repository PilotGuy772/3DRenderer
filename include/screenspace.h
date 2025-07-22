#ifndef SCREENSPACE_H
#define SCREENSPACE_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "drawer.h"
#include "matrix.h"
#include <stdlib.h>


// this set of code is meant for drawing triangles-- basically, translating an array of triangles into an array of points in the image.
typedef struct screen_point
{
    int x;
    int y;
} screen_point;

typedef struct triangle
{
    vec4f a;
    vec4f b;
    vec4f c;
} triangle;

extern float* depth_buffer;

void screenspace_draw_triangle(uint32_t* image, triangle tri);
void screenspace_draw_line(uint32_t* image, vec4f p1, vec4f p2);
void screenspace_draw_vertical_line(uint32_t* image, vec4f p1, vec4f p2);
void screenspace_plot_point(uint32_t* image, screen_point p);
void screenspace_from_ndc(vec4f* vertices, int num_vertices, float znear, float zfar, vec4f* out_vertices);
void screenspace_draw_model(vec4f* screen_vertices, int num_indices, int* ibo, uint32_t* image);
void screenspace_add_point_depth(vec4f point, uint32_t* image);
void screenspace_clear_depth_buffer(int width, int height);


#endif