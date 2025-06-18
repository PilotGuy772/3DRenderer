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

void print_vertices(vec4f* screen_vertices, int num_vertices)
{
    for (int i = 0; i < num_vertices; i++)
    {
        printf("Vertex %d: (%f, %f, %f, %f)\n", i, screen_vertices[i].x, screen_vertices[i].y, screen_vertices[i].z, screen_vertices[i].w);
    }
}

int main(int argc, char *argv[])
{    
    SDL_Init(SDL_INIT_VIDEO);

    int width = 640;
    int height = 480;

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
    vec3f* vertices = malloc(4 * sizeof(vec3));
    if (!vertices)
    {
        printf("malloc failure.\n");
        return 1;
    }

    // define a simple plane
    vertices[0] = (vec3f){-0.5f, -0.5f, 0.0f};
    vertices[1] = (vec3f){0.5f, -0.5f, 0.0f};
    vertices[2] = (vec3f){0.5f, 0.5f, 0.0f};
    vertices[3] = (vec3f){-0.5f, 0.5f, 0.0f};

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
    // for now, back it up a bit and look at the origin
    mat4 camera_transform;
    mat4_identity(camera_transform);
    mat4_translate(camera_transform, 0.0f, 0.0f, -3.0f);
    printf("Camera transform:\n");
    mat4_print(camera_transform);

    int running = 1;
    SDL_Event event;
    while (running)
    {
        // temp-- end loop immediately after one iteration
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = 0;
        }        

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
        // (add homogenous component)
        vec4f* vertices_w = malloc(4 * sizeof(vec4f));
        model_add_w(vertices, 4, vertices_w);
        world_from_model(vertices_w, 4, transform, world_vertices);

        // 2. transform into camera space
        vec4f* camera_vertices = malloc(4 * sizeof(vec4f));
        camera_from_world(camera_transform, world_vertices, 4, camera_vertices);

        // 3. transform into clip space
        // we need to decide on some camera constants too (znear, zfar, fov, aspect)
        float znear = 0.1f;
        float zfar = 100.0f;
        float fov = M_PI / 2.0f; // 90 degrees
        float aspect = (float)width / (float)height;
        vec4f* clip_vertices = malloc(4 * sizeof(vec4f));
        clip_from_camera(camera_vertices, 4, fov, aspect, znear, zfar, clip_vertices);

        printf("Vertices in clip space:\n");
        print_vertices(clip_vertices, 4);

        // 4. transform into NDC
        // this is simple enough that we can do it in place
        for (int i = 0; i < 4; i++)
        {
            clip_vertices[i].x /= clip_vertices[i].w;
            clip_vertices[i].y /= clip_vertices[i].w;
            clip_vertices[i].z /= clip_vertices[i].w;
        }

        printf("Vertices in NDC:\n");
        print_vertices(clip_vertices, 4);

        // 5. transform into screen space
        vec4f* screen_vertices = malloc(4 * sizeof(vec4f));
        screenspace_from_ndc(clip_vertices, 4, znear, zfar, screen_vertices);

        // finally, 6. assemble and draw triangles
        // now we can go all the way back to our IBO and make triangles
        // this will be automated later, manual for now
        triangle tri1 = { 
            {screen_vertices[0].x, screen_vertices[0].y}, 
            {screen_vertices[1].x, screen_vertices[1].y}, 
            {screen_vertices[2].x, screen_vertices[2].y} 
        };

        triangle tri2 = { 
            {screen_vertices[0].x, screen_vertices[0].y}, 
            {screen_vertices[2].x, screen_vertices[2].y}, 
            {screen_vertices[3].x, screen_vertices[3].y} 
        };
        screenspace_draw_triangle(image, tri1);
        screenspace_draw_triangle(image, tri2);

        // free everything
        free(world_vertices);
        free(vertices_w);
        free(camera_vertices);
        free(clip_vertices);
        // ATTN! We will free these as soon as we're done with them later... doing this for simplicity for now


        // for testing, move the camera backwards 
        mat4_translate(camera_transform, 0, 0, camera_transform[14] - 1.0f);
        mat4_print(camera_transform);

        //screenspace_plot_point(image, (screen_point){width / 2, height / 2});
        drawer_draw_buffer(image);
        drawer_clear_buffer(image);
    }

    //printf("p1: %d %d\np2: %d %d\np3: %d %d\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    free(image);
    drawer_cleanup();
    return 0;
}


