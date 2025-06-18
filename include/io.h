#ifndef IO_H
#define IO_H

#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_model(char* filepath, vec3f** vertices, int** indices, int* num_vertices, int* num_indices);

#endif // IO_H