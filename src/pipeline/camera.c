#include "camera.h"

void camera_from_world(mat4 camera_transform, vec4f* vertices, int num_vertices, vec4f* out_vertices)
{
    mat4 inverse_camera_transform;
    // compute inverse of the camera transform
    mat4_inverse(camera_transform, inverse_camera_transform);

    for (int i = 0; i < num_vertices; i++)
    {
        mat4_transform_vec4f(inverse_camera_transform, &vertices[i], &out_vertices[i]);
    }
}