#include "drawer.h"

#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
int window_width;
int window_height;

void drawer_init(int width, int height)
{
    window = SDL_CreateWindow("3DRenderer",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, 0);

    renderer = SDL_CreateRenderer(window, -1, 0);

    texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        width, height);

    window_width = width;
    window_height = height;
}

void drawer_draw_buffer(uint32_t* image)
{
    SDL_UpdateTexture(texture, NULL, image, window_width * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void drawer_cleanup()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawer_clear_buffer(uint32_t* image)
{
    for (int i = 0; i < window_width * window_height; i++)
    {
        image[i] = 0xFF000000; // ARGB format, fully transparent
    }
}