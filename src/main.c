#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "drawer.h"
#include "screenspace.h"

int main(int argc, char *argv[])
{    
    SDL_Init(SDL_INIT_VIDEO);

    int width = 1920;
    int height = 1080;

    drawer_init(width, height);

    uint32_t* image = malloc(width * height * sizeof(uint32_t));

    if (!image)
    {
        printf("malloc failure.\n");
        return 1;
    }

    // draw a random triangle!
    srand(time(NULL));
    screen_point p1 = {rand() % width, rand() % height};
    screen_point p2 = {rand() % width, rand() % height};
    screen_point p3 = {rand() % width, rand() % height};
    //screen_point p1 = {0, 0};
    //screen_point p2 = {500, 400};
    //screen_point p3 = {100, 400};
    triangle tri1 = { p1, p2, p3 }; 
    screenspace_draw_triangle(image, tri1);

    //screenspace_draw_line(image, p1, p2);

    int running = 1;
    SDL_Event event;
    while (running)
    {
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = 0;
        }

        drawer_draw_buffer(image);
        SDL_Delay(1000);

        //free(image);
        // image = malloc(width * height * sizeof(uint32_t));

        screen_point p1 = {rand() % width, rand() % height};
        screen_point p2 = {rand() % width, rand() % height};
        screen_point p3 = {rand() % width, rand() % height};
        triangle tri1 = { p1, p2, p3 }; 
        screenspace_draw_triangle(image, tri1);
    }

    printf("p1: %d %d\np2: %d %d\np3: %d %d\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    free(image);
    drawer_cleanup();
    
    return 0;
}
