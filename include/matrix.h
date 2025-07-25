// data structures for points, lines, tris, matrices
// as well as math for matrices

#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct vec2
{
    int x;
    int y;
} vec2;

typedef struct vec3
{
    int x;
    int y;
    int z;
} vec3;

typedef struct vec2f
{
    float x;
    float y;
} vec2f;

typedef struct vec3f
{
    float x;
    float y;
    float z;
} vec3f;

typedef struct vec4f
{
    float x;
    float y;
    float z;
    float w; // homogenous component
} vec4f;

// 4x4 matrix for 3D transformations
// coordinates are stored in column-major order
typedef float mat4[16];

// matrix math!
void mat4_identity(mat4 m);
void mat4_multiply(mat4 a, mat4 b, mat4 out);
void mat4_translate(mat4 m, float x, float y, float z);
void mat4_rotate_x(mat4 m, float angle);
void mat4_rotate_y(mat4 m, float angle);
void mat4_rotate_z(mat4 m, float angle);
void mat4_scale(mat4 m, float sx, float sy, float sz);
void mat4_transform_vec3(const mat4 m, const vec3* v, vec3* out);
void mat4_transform_vec3f(const mat4 m, const vec3f* v, vec3f* out);
void mat4_transform_vec2(const mat4 m, const vec2* v, vec2* out);
void mat4_transform_vec2f(const mat4 m, const vec2f* v, vec2f* out);

// vector4 transformations including homogenous component
void mat4_transform_vec4f(const mat4 m, vec4f v, vec4f* out);
void mat4_inverse(const mat4 m, mat4 out);

void mat4_print(const mat4 m);

void vec3_add_scaled(vec3f* a, vec3f* b, float scale);
int vec3_collinear(vec3f a, vec3f b, vec3f c, float epsilon);
int vec4_collinear(vec4f a, vec4f b, vec4f c, float epsilon);

#endif