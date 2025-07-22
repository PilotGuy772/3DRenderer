#ifndef CAMERA_H
#define CAMERA_H
#include "matrix.h"
#include "quat.h"
#include "model.h"

/**
 * Transforms vertices from world space to camera space.
 *  @param camera_transform The transformation matrix representing the camera's position and orientation in world space.
 *  @param vertices Array of vertices in world space, including homogeneous coordinates.
 *  @param num_vertices Number of vertices in the input array.
 *  @param out_vertices Output array for the transformed vertices in camera space, also with homogeneous coordinates.
 */
void camera_from_world(vec3f camera_pos, quat camera_rot, veretx* vertices, int num_vertices, vertex* out_vertices);

#endif // CAMERA_H