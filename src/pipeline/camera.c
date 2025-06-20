#include "camera.h"

void camera_from_world(vec3f camera_pos, quat camera_rot, vec4f* vertices, int num_vertices, vec4f* out_vertices)
{
    mat4 inverse_camera_transform;

    // Step 1: Get inverse rotation (conjugate of the quaternion)
    quat inverse_rot = quat_conjugate(camera_rot);

    // Step 2: Convert inverse rotation to a matrix
    mat4 rot_matrix;
    quat_to_mat4(inverse_rot, rot_matrix); // assumes column-major

    // Step 3: Create translation matrix for -camera_pos
    mat4 trans_matrix;
    mat4_identity(trans_matrix);
    mat4_translate(trans_matrix, -camera_pos.x, -camera_pos.y, -camera_pos.z);

    // Step 4: Combine: inverse_transform = rot_matrix * trans_matrix
    mat4_multiply(rot_matrix, trans_matrix, inverse_camera_transform);

    // Step 5: Transform each vertex
    for (int i = 0; i < num_vertices; i++)
    {
        mat4_transform_vec4f(inverse_camera_transform, vertices[i], &out_vertices[i]);
    }
}
