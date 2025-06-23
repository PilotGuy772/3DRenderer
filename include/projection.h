#ifndef PROJECTION_H
#define PROJECTION_H

#include "matrix.h"
#include "model.h"

void clip_from_camera(vertex* vertices, int num_vertices, float fov, float aspect, float znear, float zfar, vertex* out_vertices);

#endif // PROJECTION_H