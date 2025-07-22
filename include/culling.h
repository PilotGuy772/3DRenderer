#ifndef CULLING_H
#define CULLING_H
#include "matrix.h"

#define MAX_VERTS_PER_TRI 12

typedef struct {
    vec4f verts[MAX_VERTS_PER_TRI];
    int count;
} polygon4f;

/**
 * @brief Determines whether a vertex is inside the given clipping plane.
 *
 * @param v     The vertex to test.
 * @param axis  The axis to clip against (0=x, 1=y, 2=z).
 * @param sign  +1 for positive plane, -1 for negative.
 * @return 1 if inside, 0 otherwise.
 */
int inside(vec4f v, int axis, float sign);

/**
 * @brief Computes intersection point of edge (a-b) with a clipping plane.
 *
 * @param a     Start vertex of the edge.
 * @param b     End vertex of the edge.
 * @param axis  The axis to clip against (0=x, 1=y, 2=z).
 * @param sign  +1 for positive plane, -1 for negative.
 * @return The intersection point in homogeneous space.
 */
vec4f intersect(vec4f a, vec4f b, int axis, float sign);

/**
 * @brief Clips a polygon against a single plane of the view frustum.
 *
 * @param in     Input polygon.
 * @param out    Output polygon after clipping.
 * @param axis   Axis to clip against (0=x, 1=y, 2=z).
 * @param sign   +1 or -1 indicating the side of the plane.
 */
void clip_polygon_against_plane(polygon4f* in, polygon4f* out, int axis, float sign);

/**
 * @brief Clips a triangle against the 6 planes of the homogeneous view frustum.
 *
 * @param v0             First vertex of the triangle.
 * @param v1             Second vertex of the triangle.
 * @param v2             Third vertex of the triangle.
 * @param out_verts      Output array of clipped vertices.
 * @param out_vert_count Pointer to number of output vertices.
 */
void clip_triangle(vec4f v0, vec4f v1, vec4f v2, vec4f* out_verts, int* out_vert_count);

/**
 * @brief Triangulates a convex polygon into triangles using a triangle fan.
 *
 * @param verts           Array of polygon vertices.
 * @param count           Number of vertices.
 * @param out_indices     Output array to store triangle indices.
 * @param out_index_count Pointer to number of output indices written.
 * @param base_index      Base vertex index offset.
 */
void triangulate_polygon(vec4f* verts, int count, int* out_indices, int* out_index_count, int base_index);

/**
 * @brief Clips and triangulates all input triangles against the view frustum.
 *
 * @param vertices         Input vertex array (in clip space).
 * @param num_vertices     Number of vertices.
 * @param indices          Input triangle indices.
 * @param num_indices      Number of input indices (should be multiple of 3).
 * @param out_vertices     Output array of vertices (malloc'd).
 * @param out_num_vertices Pointer to number of output vertices.
 * @param out_indices      Output triangle indices (malloc'd).
 * @param out_num_indices  Pointer to number of output indices.
 */
void culling_cull_triangle(vec4f* vertices, int num_vertices, int* indices, int num_indices,
                           vec4f** out_vertices, int* out_num_vertices,
                           int** out_indices, int* out_num_indices);

int culling_check_point_in_range(vec4f point);

/// @brief Find the intersection point of a line segment with the view frustum planes.
/// @param v1 The first endpoint of the line segment. This point must be outside the view frustum.
/// @param v2 The second endpoint of the line segment. This point must be inside the view frustum.
/// @param intersection The point of intersection (if any).
void culling_find_line_intersection(vec4f v1, vec4f v2, vec4f* intersection);

#endif