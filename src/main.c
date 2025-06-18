#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "drawer.h"
#include "screenspace.h"
#include "model.h"
#include "world.h"
#include "camera.h"
#include "matrix.h"
#include "projection.h"
#include <math.h>

# define M_PI 3.14159265358979323846

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
    // srand(time(NULL));
    // screen_point p1 = {rand() % width, rand() % height};
    // screen_point p2 = {rand() % width, rand() % height};
    // screen_point p3 = {rand() % width, rand() % height};
    // //screen_point p1 = {0, 0};
    // //screen_point p2 = {500, 400};
    // //screen_point p3 = {100, 400};
    // triangle tri1 = { p1, p2, p3 }; 
    // screenspace_draw_triangle(image, tri1);

    //screenspace_draw_line(image, p1, p2);

    // create a model-- for now, just a plane will do
    vec3* vertices = malloc(4 * sizeof(vec3));
    if (!vertices)
    {
        printf("malloc failure.\n");
        return 1;
    }

    // define a simple plane
    vertices[0] = (vec3){-1.0f, 0.0f, -1.0f};
    vertices[1] = (vec3){1.0f, 0.0f, -1.0f};
    vertices[2] = (vec3){1.0f, 0.0f, 1.0f};
    vertices[3] = (vec3){-1.0f, 0.0f, 1.0f};

    // and the IBO
    int* indices = malloc(6 * sizeof(int));
    if (!indices)
    {
        printf("malloc failure.\n");
        free(vertices);
        return 1;
    }
    indices[0] = 0; // triangle 1
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 0; // triangle 2
    indices[4] = 2;
    indices[5] = 3;

    // finally, a mat4 representing its position in world space
    // for now, we want no transformations, so we can use the identity matrix
    mat4 transform;
    mat4_identity(transform);


    // define a matrix for camera position
    // for now, place it directly above the plane, looking down
    mat4 camera_transform;
    mat4_identity(camera_transform);
    mat4_translate(camera_transform, 0.0f, 3.0f, 0.0f);
    //mat4_look_at(camera_transform, (vec3){0.0f, 3.0f, 0.0f}, (vec3){0.0f, 0.0f, 0.0f}, (vec3){0.0f, 0.0f, 1.0f});
    mat4_rotate_x(camera_transform, -M_PI / 2.0f); // rotate the camera to look down

    int running = 1;
    SDL_Event event;
    while (running)
    {
        // temp-- end loop immediately after one iteration
        running = 0;

        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = 0;
        }

        drawer_draw_buffer(image);
        SDL_Delay(1000);

        //free(image);
        // image = malloc(width * height * sizeof(uint32_t));

        // screen_point p1 = {rand() % width, rand() % height};
        // screen_point p2 = {rand() % width, rand() % height};
        // screen_point p3 = {rand() % width, rand() % height};
        // triangle tri1 = { p1, p2, p3 }; 
        // screenspace_draw_triangle(image, tri1);

        // basic render pipeline track, using the model defined above for testing
        
        // 1. translate into world space
        vec4f* world_vertices = malloc(4 * sizeof(vec4f));
        if (!world_vertices)
        {
            printf("malloc failure.\n");
            free(vertices);
            free(indices);
            return 1;
        }
        vec4f* vertices_w = malloc(4 * sizeof(vec4f));
        if (!vertices_w)
        {
            printf("malloc failure.\n");
            free(vertices);
            free(indices);
            free(world_vertices);
            return 1;
        }
        model_add_w((vec3f*)vertices, 4, vertices_w);
        world_from_model(vertices_w, 4, transform, world_vertices);

        // 2. transform into camera space
        vec4f* camera_vertices = malloc(4 * sizeof(vec4f));
        if (!camera_vertices)
        {
            printf("malloc failure.\n");
            free(vertices);
            free(indices);
            free(world_vertices);
            free(vertices_w);
            return 1;
        }
        camera_from_world(camera_transform, world_vertices, 4, camera_vertices);

        // 3. transform into clip space
        // we need to decide on some camera constants too (znear, zfar, fov, aspect)
        float znear = 0.1f;
        float zfar = 100.0f;
        float fov = M_PI / 2.0f; // 90 degrees
        float aspect = (float)width / (float)height;
        vec4f* clip_vertices = malloc(4 * sizeof(vec4f));
        if (!clip_vertices)
        {
            printf("malloc failure.\n");
            free(vertices);
            free(indices);
            free(world_vertices);
            free(vertices_w);
            free(camera_vertices);
            return 1;
        }
        clip_from_camera(camera_vertices, 4, fov, aspect, znear, zfar, clip_vertices);
    }

    //printf("p1: %d %d\np2: %d %d\np3: %d %d\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    free(image);
    drawer_cleanup();
    free(vertices);
    free(indices);
    return 0;
}
