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

    key_states = malloc(SDL_NUM_SCANCODES * sizeof(int));

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
    vertex* vertices;
    face* faces;
    int num_vertices, num_faces;
    read_model("cube.obj", &vertices, &faces, &num_vertices, &num_faces);

    // array for face colors! For now, just randomly generate colors for each face
    // uint32_t* face_colors = malloc((int)ceil(num_indices / 3.0) * sizeof(uint32_t));
    // for (int i = 0; i < (int)ceil(num_indices / 3.0); i++)
    // {
    //     face_colors[i] = (uint32_t)((rand() % 0x00FFFFFF) + 0xFF000000); // random color with full opacity
    // }

    printf("Read vertices:\n");
    for (int i = 0; i < 1; i++)
    {
        printf("Vertex %d: (%f, %f, %f)\n", i, vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
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
    

    vec3f camera_pos = {0.0f, 0.0f, 6.0f};
    quat camera_rot = {1.0f, 0.0f, 0.0f, 0.0f}; // identity quaternion

    int running = 1;
    SDL_Event event;
    while (running)
    {

        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    keydown(event.key.keysym.sym);
                    break;
                case SDL_KEYUP:
                    keyup(event.key.keysym.sym);
                    break;
                default:
                    break;
            }
        }        

        SDL_Delay(16);

        // basic render pipeline track, using the model defined above for testing

        render_model(image, vertices, num_vertices, faces, num_faces, transform, camera_pos, camera_rot, width, height);

        //screenspace_plot_point(image, (screen_point){width / 2, height / 2});
        drawer_draw_buffer(image);
        drawer_clear_buffer(image);

        //mat4_translate(camera_transform, 0, 0, camera_transform[14] - 0.004f);
        mat4_multiply(transform, change, transform);
        // clamp_movement(&camera_per_tick_transform);
        // mat4_multiply(camera_transform, camera_per_tick_transform, camera_transform);

        // camera control
        tick_transform(&camera_pos, &camera_rot);

        // print stats
        printf("Camera position: (%f, %f, %f)\n", camera_pos.x, camera_pos.y, camera_pos.z);
        printf("Camera rotation: (%f, %f, %f, %f)\n", camera_rot.w, camera_rot.x, camera_rot.y, camera_rot.z);
    }

    //printf("p1: %d %d\np2: %d %d\np3: %d %d\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
    free(image);
    drawer_cleanup();
    return 0;
}

void render_model(uint32_t* image, vertex* vertices, int num_vertices, int* faces, int num_faces, mat4 transform, vec3f camera_pos, quat camera_rot, int width, int height)
{
    // 1. translate into world space
    vertex* world_vertices = malloc(num_vertices * sizeof(vertex));
    // (add homogenous component)
    //vec4f* vertices_w = malloc(num_vertices * sizeof(vec4f));
    //model_add_w(vertices, num_vertices, vertices_w);
    world_from_model(vertices, num_vertices, transform, world_vertices);
    

    // 2. transform into camera space
    vertex* camera_vertices = malloc(num_vertices * sizeof(vertex));
    camera_from_world(camera_pos, camera_rot, world_vertices, num_vertices, camera_vertices);

    

    // 3. transform into clip space
    // we need to decide on some camera constants too (znear, zfar, fov, aspect)
    float znear = 0.1f;
    float zfar = 50.0f;
    float fov = M_PI / 2.0f; // 90 degrees
    float aspect = (float)width / (float)height;
    vertex* clip_vertices = malloc(num_vertices * sizeof(vertex));
    clip_from_camera(camera_vertices, num_vertices, fov, aspect, znear, zfar, clip_vertices);

    // culling!!
    // 3.5. cull triangles that are outside the view frustum
    vertex* culled_vertices = NULL;
    face* culled_faces = NULL;
    int tmp_num_vertices = 0;
    int tmp_num_indices = 0;
    culling_cull_triangle(clip_vertices, num_vertices, faces, num_faces, 
                            &culled_vertices, &tmp_num_vertices, 
                            &culled_faces, &tmp_num_indices);
    num_vertices = tmp_num_vertices;
    faces = tmp_num_indices;

    // 4. transform into NDC
    // this is simple enough that we can do it in place
    for (int i = 0; i < num_vertices; i++)
    {
        culled_vertices[i].position.x /= culled_vertices[i].position.w;
        culled_vertices[i].position.y /= culled_vertices[i].position.w;
        culled_vertices[i].position.z /= culled_vertices[i].position.w;
    }

    // free the original clip_vertices
    free(clip_vertices);

    // 5. transform into screen space
    vertex* screen_vertices = malloc(num_vertices * sizeof(vertex));
    screenspace_from_ndc(culled_vertices, num_vertices, znear, zfar, screen_vertices);

    // finally, 6. assemble and draw triangles
    //screenspace_clear_depth_buffer(width, height);
    //screenspace_draw_model(screen_vertices, num_indices, culled_indices, colors, image);
    

    // free everything
    free(world_vertices);
    free(vertices_w);
    free(camera_vertices);
    // ATTN! We will free these as soon as we're done with them later... doing this for simplicity for now

}


