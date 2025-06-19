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
#include "io.h"
#include "culling.h"

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

    int width = 800;
    int height = 600;

    drawer_init(width, height);

    uint32_t* image = malloc(width * height * sizeof(uint32_t));

    if (!image)
    {
        printf("malloc failure.\n");
        return 1;
    }

    // read a model from file
    vec3f* vertices;
    int* indices;
    int num_vertices, num_indices;
    read_model("cube.obj", &vertices, &indices, &num_vertices, &num_indices);

    printf("Read vertices:\n");
    for (int i = 0; i < 1; i++)
    {
        printf("Vertex %d: (%f, %f, %f)\n", i, vertices[i].x, vertices[i].y, vertices[i].z);
    }
    // printf("Read faces:\n");
    // for (int i = 0; i < num_indices; i += 3)
    // {
    //     printf("Face %d: (%d, %d, %d)\n", i / 3, indices[i], indices[i + 1], indices[i + 2]);
    // }

    // finally, a mat4 representing its position in world space
    // for now, we want no transformations, so we can use the identity matrix
    mat4 transform;
    mat4_identity(transform);
    //mat4_translate(transform, 0.0f, 0.0f, 6.0f);
    printf("Model transform:\n");
    mat4_print(transform);

    // this is the delta every frame; this matrix is applied to the transform every frame
    mat4 change;
    mat4_rotate_y(change, 0.01f);
    
    

    // define a matrix for camera position
    // for now, back it up a bit and look at the origin
    mat4 camera_transform;
    mat4_identity(camera_transform);
    mat4_translate(camera_transform, 0.0f, 0.0f, 6.0f);
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

        SDL_Delay(16);

        // basic render pipeline track, using the model defined above for testing

        render_model(image, vertices, num_vertices, indices, num_indices, transform, camera_transform, width, height);

        //screenspace_plot_point(image, (screen_point){width / 2, height / 2});
        drawer_draw_buffer(image);
        drawer_clear_buffer(image);

        //mat4_translate(camera_transform, 0, 0, camera_transform[14] - 0.004f);
        // spin the model
        mat4_multiply(transform, change, transform);
    }

    //printf("p1: %d %d\np2: %d %d\np3: %d %d\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    free(image);
    drawer_cleanup();
    return 0;
}

void render_model(uint32_t* image, vec3f* vertices, int num_vertices, int* indices, int num_indices, mat4 transform, mat4 camera_transform, int width, int height)
{
    // 1. translate into world space
    vec4f* world_vertices = malloc(num_vertices * sizeof(vec4f));
    // (add homogenous component)
    vec4f* vertices_w = malloc(num_vertices * sizeof(vec4f));
    model_add_w(vertices, num_vertices, vertices_w);
    world_from_model(vertices_w, num_vertices, transform, world_vertices);
    printf("Vertices in world space:\n");
    print_vertices(world_vertices, num_vertices);

    // 2. transform into camera space
    vec4f* camera_vertices = malloc(num_vertices * sizeof(vec4f));
    camera_from_world(camera_transform, world_vertices, num_vertices, camera_vertices);

    printf("Vertices in camera space:\n");
    print_vertices(camera_vertices, num_vertices);

    // 3. transform into clip space
    // we need to decide on some camera constants too (znear, zfar, fov, aspect)
    float znear = 0.1f;
    float zfar = 50.0f;
    float fov = M_PI / 2.0f; // 90 degrees
    float aspect = (float)width / (float)height;
    vec4f* clip_vertices = malloc(num_vertices * sizeof(vec4f));
    clip_from_camera(camera_vertices, num_vertices, fov, aspect, znear, zfar, clip_vertices);

    printf("Vertices in clip space:\n");
    print_vertices(clip_vertices, num_vertices);

    // 4. transform into NDC
    // this is simple enough that we can do it in place
    for (int i = 0; i < num_vertices; i++)
    {
        clip_vertices[i].x /= clip_vertices[i].w;
        clip_vertices[i].y /= clip_vertices[i].w;
        clip_vertices[i].z /= clip_vertices[i].w;
    }

    printf("Vertices in NDC:\n");
    print_vertices(clip_vertices, num_vertices);

    // culling!!
    // 4.5. cull triangles that are outside the view frustum
    vec4f* culled_vertices;
    int* culled_indices;
    int tmp_num_vertices;
    int tmp_num_indices;
    culling_cull_triangle(clip_vertices, num_vertices, indices, num_indices, culled_vertices, &tmp_num_vertices, culled_indices, &tmp_num_indices);
    num_vertices = tmp_num_vertices;
    num_indices = tmp_num_indices;
    printf("Culled!\n");

    // 5. transform into screen space
    vec4f* screen_vertices = malloc(num_vertices * sizeof(vec4f));
    screenspace_from_ndc(culled_vertices, num_vertices, znear, zfar, screen_vertices);
    printf("Vertices in screen space:\n");
    print_vertices(screen_vertices, num_vertices);

    // finally, 6. assemble and draw triangles
    screenspace_draw_model(screen_vertices, num_indices, culled_indices, image);

    // free everything
    free(world_vertices);
    free(vertices_w);
    free(camera_vertices);
    free(clip_vertices);
    // ATTN! We will free these as soon as we're done with them later... doing this for simplicity for now

}


