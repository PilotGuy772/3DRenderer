// quaternions!!

#include "quat.h"

quat quat_multiply(quat a, quat b)
{
    quat result;
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    return result;
}

quat quat_from_axis_angle(vec3f axis, float angle)
{
    quat q;
    float half_angle = angle * 0.5f;
    float s = sinf(half_angle);
    q.w = cosf(half_angle);
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    return q;
}

quat quat_normalize(quat q)
{
    float length = sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    if (length == 0.0f) {
        return (quat){0, 0, 0, 0}; // return zero quaternion if length is zero
    }
    quat normalized;
    normalized.w = q.w / length;
    normalized.x = q.x / length;
    normalized.y = q.y / length;
    normalized.z = q.z / length;
    return normalized;
}

vec3f quat_forward(quat q)
{
    return quat_rotate_vector(q, (vec3f){0, 0, -1});
}

vec3f quat_up(quat q)
{
    return quat_rotate_vector(q, (vec3f){0, 1, 0});
}

vec3f quat_right(quat q)
{
    return quat_rotate_vector(q, (vec3f){1, 0, 0});
}

vec3f quat_rotate_vector(quat q, vec3f v)
{
    // Convert vector to quaternion with zero scalar part
    quat v_quat = {0, v.x, v.y, v.z};

    // Rotate the vector using quaternion multiplication
    quat q_conjugate = {q.w, -q.x, -q.y, -q.z}; // Conjugate of q
    quat rotated_quat = quat_multiply(quat_multiply(q, v_quat), q_conjugate);

    // Return the vector part of the resulting quaternion
    return (vec3f){rotated_quat.x, rotated_quat.y, rotated_quat.z};
}

void quat_to_mat4(quat q, mat4 out) {
    float x2 = q.x + q.x;
    float y2 = q.y + q.y;
    float z2 = q.z + q.z;

    float xx = q.x * x2;
    float yy = q.y * y2;
    float zz = q.z * z2;
    float xy = q.x * y2;
    float xz = q.x * z2;
    float yz = q.y * z2;
    float wx = q.w * x2;
    float wy = q.w * y2;
    float wz = q.w * z2;

    mat4_identity(out);
    // Rotation matrix in column-major order
    out[0]  = 1.0f - (yy + zz); // column 0, row 0
    out[1]  = xy + wz;          // column 0, row 1
    out[2]  = xz - wy;          // column 0, row 2
    out[3]  = 0.0f;

    out[4]  = xy - wz;          // column 1, row 0
    out[5]  = 1.0f - (xx + zz); // column 1, row 1
    out[6]  = yz + wx;          // column 1, row 2
    out[7]  = 0.0f;

    out[8]  = xz + wy;          // column 2, row 0
    out[9]  = yz - wx;          // column 2, row 1
    out[10] = 1.0f - (xx + yy); // column 2, row 2
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
}
 
