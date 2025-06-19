// read Wavefront object file (.obj) format
// and interpret into arrays of vertices and edges
#include "io.h"


void read_model(char* filepath, vec3f** vertices, int** indices, int* num_vertices, int* num_indices)
{
    FILE* file = fopen(filepath, "r");
    if (!file) {
        perror("Failed to open model file");
        return;
    }

    *vertices = malloc(MAX_VERTICES * sizeof(vec3f));
    *indices = malloc(MAX_INDICES * sizeof(int));

    if (!*vertices || !*indices) {
        perror("Failed to allocate memory");
        fclose(file);
        return;
    }

    char line[256];
    int vertex_count = 0;
    int face_count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            // Vertex line
            sscanf(line + 2, "%f %f %f", &(*vertices)[vertex_count].x, &(*vertices)[vertex_count].y, &(*vertices)[vertex_count].z);
            vertex_count++;
        } else if (strncmp(line, "f ", 2) == 0) {
            int v[4];
            int count = sscanf(line + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
                            &v[0], &v[1], &v[2], &v[3]);

            if (count >= 3) {
                (*indices)[face_count++] = v[0] - 1;
                (*indices)[face_count++] = v[1] - 1;
                (*indices)[face_count++] = v[2] - 1;
            }
            if (count == 4) {
                (*indices)[face_count++] = v[0] - 1;
                (*indices)[face_count++] = v[2] - 1;
                (*indices)[face_count++] = v[3] - 1;
            }
        }
    }

    *num_vertices = vertex_count;
    *num_indices = face_count; // Each face consists of 3 indices

    *vertices = realloc(*vertices, vertex_count * sizeof(vec3f));
    *indices = realloc(*indices, face_count * sizeof(int));

    fclose(file);
}


void handle_keypress(SDL_Keycode key, mat4* camera_transform)
{
    mat4 delta;
    mat4_identity(delta);
    // (translation only for now, rotation is... a challenge)
    switch (key) {
        // translation
        case SDLK_w: // Move forward
            mat4_translate(delta, 0.0f, 0.0f, -MOVE_SPEED);
            break;
        case SDLK_s: // Move backward
            mat4_translate(delta, 0.0f, 0.0f, MOVE_SPEED);
            break;
        case SDLK_a: // Move left
            mat4_translate(delta, -MOVE_SPEED, 0.0f, 0.0f);
            break;
        case SDLK_d: // Move right
            mat4_translate(delta, MOVE_SPEED, 0.0f, 0.0f);
            break;
        case SDLK_q: // move up
            mat4_translate(delta, 0.0f, MOVE_SPEED, 0.0f);
            break;
        case SDLK_e: // move down
            mat4_translate(delta, 0.0f, -MOVE_SPEED, 0.0f);
            break;

        // rotation
        case SDLK_LEFT: // Rotate left
            mat4_rotate_y(delta, -ROTATE_SPEED);
            break;
        case SDLK_RIGHT: // Rotate right
            mat4_rotate_y(delta, ROTATE_SPEED);
            break;
        case SDLK_UP: // Rotate up
            mat4_rotate_x(delta, -ROTATE_SPEED);
            break;
        case SDLK_DOWN: // Rotate down
            mat4_rotate_x(delta, ROTATE_SPEED);
        default:
            break;
    }

    // apply the delta to the camera transform
    mat4_multiply(*camera_transform, delta, *camera_transform);
}

// this is the above but in reverse; it undoes the per-tick transform
void handle_keyup(SDL_Keycode key, mat4* camera_transform)
{
    mat4 delta;
    mat4_identity(delta);
    switch (key) {
        // translation
        case SDLK_w: // Move forward
            mat4_translate(delta, 0.0f, 0.0f, MOVE_SPEED);
            break;
        case SDLK_s: // Move backward
            mat4_translate(delta, 0.0f, 0.0f, -MOVE_SPEED);
            break;
        case SDLK_a: // Move left
            mat4_translate(delta, MOVE_SPEED, 0.0f, 0.0f);
            break;
        case SDLK_d: // Move right
            mat4_translate(delta, -MOVE_SPEED, 0.0f, 0.0f);
            break;
        case SDLK_q: // move up
            mat4_translate(delta, 0.0f, -MOVE_SPEED, 0.0f);
            break;
        case SDLK_e: // move down
            mat4_translate(delta, 0.0f, MOVE_SPEED, 0.0f);

        // rotation
        case SDLK_LEFT: // Rotate left
            mat4_identity(*camera_transform);
            break;
        case SDLK_RIGHT: // Rotate right
            mat4_identity(*camera_transform);
            break;
        case SDLK_UP: // Rotate up
            mat4_identity(*camera_transform);
            break;
        case SDLK_DOWN: // Rotate down
            mat4_identity(*camera_transform);
            break;
        default:
            break;
    }

    // apply the delta to the camera transform
    mat4_multiply(*camera_transform, delta, *camera_transform);
}

void clamp_movement(mat4 *camera_per_tick_transform)
{
    // NOTE: for now, only clamp translation.
    // translation is at 13 14 15
    for (int i = 12; i < 15; i++) {
        if ((*camera_per_tick_transform)[i] > MOVE_SPEED) {
            (*camera_per_tick_transform)[i] = MOVE_SPEED;
        } else if ((*camera_per_tick_transform)[i] < -MOVE_SPEED) {
            (*camera_per_tick_transform)[i] = -MOVE_SPEED;
        }
    }

    // rotation is at 0 1 2 4 5 6
    // for (int i = 0; i < 12; i++) {
    //     if ((*camera_per_tick_transform)[i] > ROTATE_SPEED) {
    //         (*camera_per_tick_transform)[i] = ROTATE_SPEED;
    //     } else if ((*camera_per_tick_transform)[i] < -ROTATE_SPEED) {       
    //         (*camera_per_tick_transform)[i] = -ROTATE_SPEED;
    //     }
    // }
}

void negate_rotation(mat4* camera_per_tick_transform)
{
    float* x = camera_per_tick_transform[12];
    float* y = camera_per_tick_transform[13];
    float* z = camera_per_tick_transform[14];

    mat4_identity(*camera_per_tick_transform);
    mat4_translate(*camera_per_tick_transform, *x, *y, *z);
}