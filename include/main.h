#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "drawer.h"
#include "matrix.h"
#include "quat.h"

int main(int argc, char *argv[]);
void print_vertices(vec4f *screen_vertices, int num_vertices);
void render_model(uint32_t* image, vec3f* vertices, int num_vertices, int* faces, int num_faces, mat4 transform, vec3f camera_pos, quat camera_rot, int width, int height);

#endif