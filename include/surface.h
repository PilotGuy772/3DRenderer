#ifndef SURFACE_H
#define SURFACE_H

typedef struct world_point
{
    int x;
    int y;
    int z;
} world_point;

typedef struct world_line
{
    world_point a;
    world_point b;
} world_line;

typedef struct world_triangle
{
    world_point a;
    world_point b;
    world_point c;
} world_triangle;

typedef struct world_rect
{
    world_point a;
    world_point b;
} world_rect;


void surface_get_triangles(world_rect rect, world_triangle* a, world_triangle* b);

#endif