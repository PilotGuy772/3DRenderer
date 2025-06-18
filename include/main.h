#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "drawer.h"
#include "matrix.h"

int main(int argc, char *argv[]);
void print_vertices(vec4f *screen_vertices, int num_vertices);

#endif