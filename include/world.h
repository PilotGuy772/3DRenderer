#ifndef WORLD_H
#define WORLD_H

#include "matrix.h"

/** 
 * @brief Converts a model's vertices to world coordinates using a transformation matrix.
 * 
 * @param vertices Array of model vertices in model space.
 * @param num_vertices Number of vertices in the model.
 * @param transform Transformation matrix to apply to the vertices representing the model's position, rotation, and scale in the world.
 * @param out_vertices Output array for the transformed vertices in world space.
 */
void world_from_model(vec4f* vertices, int num_vertices, mat4 transform, vec4f* out_vertices);

#endif