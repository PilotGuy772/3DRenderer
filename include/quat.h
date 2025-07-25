#ifndef QUAT_H
#define QUAT_H

#include "matrix.h"

typedef struct quat {
    float w, x, y, z;
} quat;

/**
 * @brief Creates a new quaternion representing rotation about a given axis.
 * @param axis A vector representing the 3D axis to rotate around.
 * @param angle The angle to rotate around that axis.
 */
quat quat_from_axis_angle(vec3f axis, float angle);

/**
 * @brief Multiplies two quaternions.
 */
quat quat_multiply(quat a, quat b);

/**
 * @brief Returns the unit vector of the forward direction of an observer pointing in the direction of the quaternion q.
 */
vec3f quat_forward(quat q);

/**
 * @brief Returns the unit vector of the up direction of an observer pointing in the direction of the quaternion q.
 */
vec3f quat_up(quat q);

/**
 * @brief Returns the unit vector of the right direction of an observer pointing in the direction of the quaternion q.
 */
vec3f quat_right(quat q);

/**
 * @brief Rotates the vector v by the quaternion q.
 * @param q The quaternion representing the rotation.
 * @param v The vector to be rotated.
 * @return The rotated vector.
 * @note This function assumes that the quaternion is normalized.
 */
vec3f quat_rotate_vector(quat q, vec3f v);

/**
 * @brief Transforms the quaternion q into a rotation matrix.
 * @param q The quaternion to be transformed.
 * @param out The output matrix where the result will be stored. The result will be an identity matrix except for the first 3x3 submatrix, which will represent the rotation.
 */
void quat_to_mat4(quat q, mat4 out);

/**
 * @brief Returns the conjugate of the quaternion q, which is the opposite of the rotation represented by q.
 * @param q The quaternion to be conjugated.
 */
quat quat_conjugate(quat q);

#endif // QUAT_H