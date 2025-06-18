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
    screen_point a;
    screen_point b;
    screen_point c;
} triangle;

void screenspace_draw_triangle(uint32_t* image, triangle tri);
void screenspace_draw_line(uint32_t* image, screen_point p1, screen_point p2);
void screenspace_draw_vertical_line(uint32_t* image, screen_point p1, screen_point p2);
void screenspace_plot_point(uint32_t* image, screen_point p);
void screenspace_from_ndc(vec4f* vertices, int num_vertices, float znear, float zfar, vec4f* out_vertices);

#endif