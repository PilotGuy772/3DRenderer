#ifndef CULLING_H
#define CULLING_H
#include "matrix.h"

/// @brief Cull all triangles that are outside the view frustum. This function expects inputs in NDC clip space.
/// @param vertices Array of vertices in NDC clip space.
/// @param num_vertices Number of vertices in the array.
/// @param indices Array of indices defining the triangles.
/// @param num_indices Number of indices in the array.
/// @param out_vertices Output array for the remaining vertices.
/// @param out_num_vertices Pointer to store the number of vertices after culling.
/// @param out_indices Output array for the remaining indices.
/// @param out_num_indices Pointer to store the number of indices after culling.
void culling_cull_triangle(vec4f* vertices, int num_vertices, int* indices, int num_indices, vec4f** out_vertices, int* out_num_vertices, int** out_indices, int* out_num_indices);

int culling_check_point_in_range(vec4f point);

/// @brief Find the intersection point of a line segment with the view frustum planes.
/// @param v1 The first endpoint of the line segment. This point must be outside the view frustum.
/// @param v2 The second endpoint of the line segment. This point must be inside the view frustum.
/// @param intersection The point of intersection (if any).
void culling_find_line_intersection(vec4f v1, vec4f v2, vec4f* intersection);

#endif