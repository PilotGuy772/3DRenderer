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
#define ROTATE_SPEED 0.05f // <- in radians

void read_model(char* filepath, vec3f** vertices, int** indices, int* num_vertices, int* num_indices);

extern int* key_states; // 0 = not pressed, 1 = pressed

/**
 * @brief Interprets the keys that are currently held and applies the corresponding transformations to the camera.asm
 * @param pos The `vec3f` position of the camera.
 * @param rot The `quat` rotation of the camera.
 * @note This function should be called once every frame, after all keypresses have been handled.
 */
void tick_transform(vec3f* pos, quat* rot);

/**
 * @brief Updates the internal hashmap of key states when a key is pressed.
 * @param key The SDL_Keycode representing the key pressed.
 */
void keydown(SDL_Keycode);
/**
 * @brief Updates the internal hashmap of key states when a key is released.
 * @param key The SDL_Keycode representing the key released.
 */
void keyup(SDL_Keycode);

/// @brief Checks all transformations in the per-tick camera transform and clamps them to a maximum speed. Movement is clamped to MOVE_SPEED and rotation to ROTATE_SPEED.
/// @param camera_per_tick_transform The per-tick transformation matrix to be clamped.
/// @note This is useful to prevent the camera from moving too fast.
/// @note This function should be called after all keypresses have been handled for the frame, and should be called once every frame.
void clamp_movement(mat4* camera_per_tick_transform);


#endif // IO_H