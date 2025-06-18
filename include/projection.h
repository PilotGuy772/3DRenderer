#ifndef PROJECTION_H
#define PROJECTION_H

#include "matrix.h"

void clip_from_camera(vec4f* vertices, int num_vertices, float fov, float aspect, float znear, float zfar, vec4f* out_vertices);

#endif // PROJECTION_H