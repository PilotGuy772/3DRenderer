// code for handling matrices and matrix math
#include "matrix.h"

// matrix math functions!!
void mat4_identity(mat4 m)
{
    for (int i = 0; i < 16; i++)
        m[i] = (i % 5 == 0) ? 1.0f : 0.0f; // set diagonal to 1, others to 0
}

void mat4_multiply(mat4 a, mat4 b, mat4 out)
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
// vector4 transformations including homogenous component
void mat4_transform_vec4f(const mat4 m, vec4f v, vec4f* out)
{
    // matrix dot product
    /*
        | m[0] m[4] m[8]  m[12] |   | v->x |
        | m[1] m[5] m[9]  m[13] |   | v->y |
        | m[2] m[6] m[10] m[14] | * | v->z |
        | m[3] m[7] m[11] m[15] |   | v->w |
    
    */
    

    out->x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
    out->y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
    out->z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
    out->w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;

}
void mat4_inverse(const mat4 m, mat4 out)
{
    // Transpose the rotation part (3x3)
    out[0] = m[0];  out[1] = m[4];  out[2] = m[8];   out[3] = 0.0f;
    out[4] = m[1];  out[5] = m[5];  out[6] = m[9];   out[7] = 0.0f;
    out[8] = m[2];  out[9] = m[6];  out[10] = m[10]; out[11] = 0.0f;

    // Compute inverse translation: -Rᵀ * T
    float tx = m[12];
    float ty = m[13];
    float tz = m[14];

    out[12] = -(m[0] * tx + m[1] * ty + m[2] * tz);
    out[13] = -(m[4] * tx + m[5] * ty + m[6] * tz);
    out[14] = -(m[8] * tx + m[9] * ty + m[10] * tz);
    out[15] = 1.0f;
}
// Note: This is a naive implementation and does not handle all edge cases.

void mat4_print(const mat4 m)
{
    printf("| %6.2f %6.2f %6.2f %6.2f |\n",
               m[0], m[4], m[8], m[12]);
    printf("| %6.2f %6.2f %6.2f %6.2f |\n",
               m[1], m[5], m[9], m[13]);
    printf("| %6.2f %6.2f %6.2f %6.2f |\n",
               m[2], m[6], m[10], m[14]);
    printf("| %6.2f %6.2f %6.2f %6.2f |\n",
               m[3], m[7], m[11], m[15]);
}

void vec3_add_scaled(vec3f* a, vec3f* b, float scale)
{
    a->x = a->x + b->x * scale;
    a->y = a->y + b->y * scale;
    a->z = a->z + b->z * scale;
}

int vec3_collinear(vec3f a, vec3f b, vec3f c, float epsilon)
{
    // Check if the points a, b, c are collinear within a given epsilon
    float area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return fabs(area) < epsilon;
}
