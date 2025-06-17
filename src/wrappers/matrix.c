// code for handling matrices and matrix math
#include "matrix.h"

// matrix math functions!!
void mat4_identity(mat4 m)
{
    for (int i = 0; i < 16; i++)
        m[i] = (i % 5 == 0) ? 1.0f : 0.0f; // set diagonal to 1, others to 0
}

void mat4_multiply(const mat4 a, const mat4 b, mat4 out)
{
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            out[col * 4 + row] =
                a[0 * 4 + row] * b[col * 4 + 0] +
                a[1 * 4 + row] * b[col * 4 + 1] +
                a[2 * 4 + row] * b[col * 4 + 2] +
                a[3 * 4 + row] * b[col * 4 + 3];
        }
    }
}

void mat4_translate(mat4 m, float x, float y, float z)
{
    mat4_identity(m);
    m[12] = x;
    m[13] = y;
    m[14] = z;
}
void mat4_rotate_x(mat4 m, float angle)
{
    mat4_identity(m);
    float c = cosf(angle);
    float s = sinf(angle);
    m[5] = c;
    m[6] = -s;
    m[9] = s;
    m[10] = c;
}
void mat4_rotate_y(mat4 m, float angle)
{
    mat4_identity(m);
    float c = cosf(angle);
    float s = sinf(angle);
    m[0] = c;
    m[2] = s;
    m[8] = -s;
    m[10] = c;
}
void mat4_rotate_z(mat4 m, float angle)
{
    mat4_identity(m);
    float c = cosf(angle);
    float s = sinf(angle);
    m[0] = c;
    m[1] = -s;
    m[4] = s;
    m[5] = c;
}
void mat4_scale(mat4 m, float sx, float sy, float sz)
{
    mat4_identity(m);
    m[0] = sx;
    m[5] = sy;
    m[10] = sz;
}
void mat4_transform_vec3(const mat4 m, const vec3* v, vec3* out)
{
    out->x = (int)(m[0] * v->x + m[4] * v->y + m[8] * v->z + m[12]);
    out->y = (int)(m[1] * v->x + m[5] * v->y + m[9] * v->z + m[13]);
    out->z = (int)(m[2] * v->x + m[6] * v->y + m[10] * v->z + m[14]);
}
void mat4_transform_vec3f(const mat4 m, const vec3f* v, vec3f* out)
{
    out->x = m[0] * v->x + m[4] * v->y + m[8] * v->z + m[12];
    out->y = m[1] * v->x + m[5] * v->y + m[9] * v->z + m[13];
    out->z = m[2] * v->x + m[6] * v->y + m[10] * v->z + m[14];
}
void mat4_transform_vec2(const mat4 m, const vec2* v, vec2* out)
{
    out->x = (int)(m[0] * v->x + m[4] * v->y + m[12]);
    out->y = (int)(m[1] * v->x + m[5] * v->y + m[13]);
}
void mat4_transform_vec2f(const mat4 m, const vec2f* v, vec2f* out)
{
    out->x = m[0] * v->x + m[4] * v->y + m[12];
    out->y = m[1] * v->x + m[5] * v->y + m[13];
}