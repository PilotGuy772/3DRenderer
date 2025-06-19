#include "projection.h"

void clip_from_camera(vec4f* vertices, int num_vertices, float fov, float aspect, float znear, float zfar, vec4f* out_vertices)
{
    // Calculate the projection matrix
    // mat4_identity(projection_matrix);
    
    // float f = 1.0f / tanf(fov * 0.5f);
    // projection_matrix[0] = f / aspect; // m00
    // projection_matrix[5] = f;          // m11
    // projection_matrix[10] = (float)(zfar + znear) / (znear - zfar); // m22
    // projection_matrix[11] = -1.0f;     // m23
    // projection_matrix[14] = (2.0f * zfar * znear) / (znear - zfar); // m32

    float f = 1.0f / tanf(fov * 0.5f);
    float zRange = znear - zfar;

    // column major! This looks kinda wrong but it's correct
    mat4 projection_matrix = {
        f / aspect, 0,              0,                                0,
        0,          f,              0,                                0,
        0,          0,    (zfar + znear) / zRange,                   -1,
        0,          0,  (2 * zfar * znear) / zRange,                  0
    };

    for (int i = 0; i < num_vertices; i++)
    {
        mat4_transform_vec4f(projection_matrix, vertices[i], &out_vertices[i]);
    }
}