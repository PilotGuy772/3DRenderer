#ifndef IO_H
#define IO_H

#include "matrix.h"
#include "quat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#define MAX_VERTICES 10000
#define MAX_INDICES 30000
#define MOVE_SPEED 0.1f
#define ROTATE_SPEED 0.01f // <- in radians

void read_model(char* filepath, vec3f** vertices, int** indices, int* num_vertices, int* num_indices);

/// @brief Takes a keypress and updates the per-tick camera transform. The per-tick transform should be applied to the camera transform once per frame.
/// @param key The SDL_Keycode representing the key pressed.
/// @param camera_transform The current per-tick camera transform to be updated. 
void handle_keypress(SDL_Keycode key, mat4* camera_per_tick_transform);
void handle_keyup(SDL_Keycode key, mat4* camera_per_tick_transform);

/// @brief Checks all transformations in the per-tick camera transform and clamps them to a maximum speed. Movement is clamped to MOVE_SPEED and rotation to ROTATE_SPEED.
/// @param camera_per_tick_transform The per-tick transformation matrix to be clamped.
/// @note This is useful to prevent the camera from moving too fast.
/// @note This function should be called after all keypresses have been handled for the frame, and should be called once every frame.
void clamp_movement(mat4* camera_per_tick_transform);


#endif // IO_H