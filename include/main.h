#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "drawer.h"
#include "matrix.h"

int main(int argc, char *argv[]);
void print_vertices(vec4f *screen_vertices, int num_vertices);
void render_model(uint32_t* image, vec3f* vertices, int num_vertices, int* indices, int num_indices, mat4 transform, mat4 camera_transform, int width, int height);

#endif