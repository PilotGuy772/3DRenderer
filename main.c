#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>



int main(int argc, char *argv[])
{
    printf("Hello, World!");
    
    SDL_Init(SDL_INIT_VIDEO);

    int width = 128;
    int height = 128;

    SDL_Window *window = SDL_CreateWindow("3DRenderer",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
        width, height);

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

    bool running = true;
    while (running)
    {
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_UpdateTexture(texture, NULL, image, width * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    free(image);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
