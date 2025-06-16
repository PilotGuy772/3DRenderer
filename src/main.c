#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "drawer.h"


int main(int argc, char *argv[])
{
    printf("Hello, World!");
    
    SDL_Init(SDL_INIT_VIDEO);

    int width = 512;
    int height = 512;

    drawer_init(width, height);

    uint32_t* image = malloc(width * height * sizeof(uint32_t));

    if (!image)
    {
        printf("malloc failure.\n");
        return 1;
    }

    // make a red square in the middle
    for (int y = 32; y < 96; y++)
    {
        for (int x = 32; x < 96; x++)
        {
            image[x * width + y] = 0xFF0000FF;
        }
    }

    int running = 1;
    SDL_Event event;
    while (running)
    {
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = 0;
        }

        drawer_draw_buffer(image);
        SDL_Delay(16);
    }

    free(image);
    drawer_cleanup();
    
    return 0;
}
