// read Wavefront object file (.obj) format
// and interpret into arrays of vertices and edges
#include "io.h"

int* key_states; // 0 = not pressed, 1 = pressed

void read_model(char* filepath, vertex** vertices, face** faces, int* num_vertices, int* num_faces)
{
    FILE* file = fopen(filepath, "r");
    if (!file) {
        perror("Failed to open model file");
        return;
    }

    *vertices = malloc(MAX_VERTICES * sizeof(vertex));
    *faces = malloc(MAX_INDICES * sizeof(face));
    vec3f* tmp_normals = malloc(MAX_INDICES * sizeof(vec3f));

    if (!*vertices || !*faces) {
        perror("Failed to allocate memory");
        fclose(file);
        return;
    }

    char line[256];
    int vertex_count = 0;
    int face_count = 0;
    int normal_count = 0;
    while (fgets(line, sizeof(line), file)) 
    {
        if (strncmp(line, "v ", 2) == 0) 
        {
            // Vertex line
            float x, y, z;
            sscanf(line + 2, "%f %f %f", x, y, z);
            vec3f pos = {x, y, z};
            (*vertices)[vertex_count].position = (vec4f){pos.x, pos.y, pos.z, 1.0f};
            (*vertices)[vertex_count].world_pos = pos;
            (*vertices)[vertex_count].color = 0xFFFFFFFF;
            vertex_count++;
        }
        else if(strncmp(line, "vn ", 3) == 0)
        {
            float x, y, z;
            sscanf(line + 3, "%f %f %f", x, y, z);
            vec3f norm = {x, y, z};
            tmp_normals[normal_count] = norm;
            normal_count++;
        }
        else if (strncmp(line, "f ", 2) == 0) 
        {
            // first, map vertices & normals
            int v[4];
            int n[4];
            int count = sscanf(line + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
                                &v[0], &v[1], &v[2], &v[3]);
            sscanf(line + 2, "%*d/%*d/%d %*d/%*d/%d %*d/%*d/%d %*d/%*d/%d",
                                &n[0], &n[1], &n[2], &n[3]);

            // map normals to vertices
            for (int i = 0; i < count; i++)
            {
                (*vertices)[i].normal = (vec3f){
                    tmp_normals[n[i]].x, 
                    tmp_normals[n[i]].y, 
                    tmp_normals[n[i]].z
                };
            }

            if (count >= 3)
            {
                (*faces)[face_count] = (face){
                    {v[0] - 1, v[1] - 1, v[2] - 1}, // indices
                    0xFFFFFFFF, // placeholder color
                    (vec3f){tmp_normals[n[0]].x, tmp_normals[n[0]].y, tmp_normals[n[0]].z}, // use the first normal in the list for now
                    0 // placeholder material index
                };
            }
            if (count == 4)
            {
                (*faces)[face_count] = (face){
                    {v[0] - 1, v[2] - 1, v[3] - 1}, // indices
                    0xFFFFFFFF, // placeholder color
                    (vec3f){tmp_normals[n[0]].x, tmp_normals[n[0]].y, tmp_normals[n[0]].z}, // use the first normal in the list for now
                    0 // placeholder material index
                };
            }
        }
    }

    *num_vertices = vertex_count;
    *num_faces = face_count; 

    *vertices = realloc(*vertices, vertex_count * sizeof(vec3f));
    *faces = realloc(*faces, face_count * sizeof(face));

    fclose(file);
}


void tick_transform(vec3f* pos, quat* rot)
{
    mat4 delta;
    mat4_identity(delta);
    // (translation only for now, rotation is... a challenge)
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        if (key_states[i] == 0) {
            continue; // key not pressed
        }
        SDL_Keycode key = (SDL_Keycode)i;
        vec3f dir;
        switch (key) {
            // translation
            case SDLK_w: // Move forward
                dir = quat_forward(*rot);
                vec3_add_scaled(pos, &dir, MOVE_SPEED);
                break;
            case SDLK_s: // Move backward
                dir = quat_forward(*rot);
                vec3_add_scaled(pos, &dir, -MOVE_SPEED);
                break;
            case SDLK_a: // Move left
                dir = quat_right(*rot);
                vec3_add_scaled(pos, &dir, -MOVE_SPEED);
                break;
            case SDLK_d: // Move right
                dir = quat_right(*rot);
                vec3_add_scaled(pos, &dir, MOVE_SPEED);
                break;
            case SDLK_q: // move up
                dir = quat_up(*rot);
                vec3_add_scaled(pos, &dir, MOVE_SPEED);
                break;
            case SDLK_e: // move down
                dir = quat_up(*rot);
                vec3_add_scaled(pos, &dir, -MOVE_SPEED);
                break;

            // rotation
            // y-axis rotation uses the global axis
            case SDLK_j: // Rotate left
                *rot = quat_multiply(quat_from_axis_angle((vec3f){0.0f, 1.0f, 0.0f}, ROTATE_SPEED), *rot);
                break;
            case SDLK_l: // Rotate right
                *rot = quat_multiply(quat_from_axis_angle((vec3f){0.0f, 1.0f, 0.0f}, -ROTATE_SPEED), *rot);
                break;

            // x-axis rotation uses the local axis
            case SDLK_i: // Rotate up
                *rot = quat_multiply(quat_from_axis_angle(quat_right(*rot), -ROTATE_SPEED), *rot);
                break;
            case SDLK_k: // Rotate down
                *rot = quat_multiply(quat_from_axis_angle(quat_right(*rot), ROTATE_SPEED), *rot);
                break;

            // special inputs
            case SDLK_SPACE: // Reset position and rotation
                *pos = (vec3f){0.0f, 0.0f, 6.0f};
                *rot = (quat){1.0f, 0.0f, 0.0f, 0.0f};
                break;
            case SDLK_ESCAPE: // Exit the application
                exit(0);
                break;
            default:
                break;
        }
    }
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

void keydown(SDL_Keycode key)
{
    if (key_states[key] == 0) {
        key_states[key] = 1;
    }
}

void keyup(SDL_Keycode key)
{
    if (key_states[key] == 1) {
        key_states[key] = 0;
    }
}
