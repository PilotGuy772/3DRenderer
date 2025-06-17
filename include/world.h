#ifndef WORLD_H
#define WORLD_H

#include "matrix.h"

void world_from_model(vec3* vertices, int* ibo, int num_vertices, int num_indices, vec3* out_vertices, int* out_indices);