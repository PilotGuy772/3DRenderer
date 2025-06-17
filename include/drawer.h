#ifndef DRAWER_H
#define DRAWER_H

#include <SDL2/SDL.h>
#include <stdlib.h>


extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern int window_width;
extern int window_height;

void drawer_draw_buffer(uint32_t* image);
void drawer_init(int width, int height);
void drawer_cleanup();

#endif