// this file will handle culling vertices that are outside the view frustum. Once vertices are in clip space, we can easily determine if they are outside the view frustum by checking their coordinates.
/*
    Here's the basic idea:
    1. We operate in terms of triangles, not vertices. Look at each triangle and determine if it is fully outside the view frustum.
    2. If a triangle is fully outside the view frustum, we can discard it. This is the simple case.
    3. If a triangle has ONE vertex inside the view frustum, we can keep it and cull the other two vertices.
        a. Remove the triangle from the list of triangles.
        b. Add a new triangle with the one vertex inside the view and two vertices at the intersection of the triangle edges and the view frustum planes.
    4. If the triangle has TWO vertices inside the view frustum, we can keep it and cull the third vertex.
        a. Remove the triangle from the list of triangles.
        b. Determine the two points where the edges of the triangle intersect the view frustum planes.
        c. Add one triangle with both original vertices and one intersection point.
        d. Add one triangle with both intersection points and one original vertex.

    Translating this into C, we have some memory stuff to deal with. We copy everything to new arrays as we go; as we process each triangle, valid vertices go into a new
    vertex array, and valid indices go into a new index array.    
*/

#include "matrix.h"
#include "culling.h"


void culling_cull_triangle(vec4f* vertices, int num_vertices, int* indices, int num_indices,
                           vec4f** out_vertices, int* out_num_vertices,
                           int** out_indices, int* out_num_indices)
{
    int vertex_alloc = num_vertices * 3;
    int index_alloc = num_indices * 3;

    *out_vertices = malloc(vertex_alloc * sizeof(vec4f));
    *out_indices = malloc(index_alloc * sizeof(int));
    *out_num_vertices = 0;
    *out_num_indices = 0;

    if (!*out_vertices || !*out_indices) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < num_indices; i += 3)
    {
        vec4f v0 = vertices[indices[i + 0]];
        vec4f v1 = vertices[indices[i + 1]];
        vec4f v2 = vertices[indices[i + 2]];

        vec4f clipped[MAX_VERTS_PER_TRI];
        int clipped_count = 0;
        clip_triangle(v0, v1, v2, clipped, &clipped_count);

        if (clipped_count == 0)
            continue;

        int base = *out_num_vertices;
        for (int j = 0; j < clipped_count; j++) {
            if (*out_num_vertices >= vertex_alloc) {
                vertex_alloc *= 2;
                *out_vertices = realloc(*out_vertices, vertex_alloc * sizeof(vec4f));
                if (!*out_vertices) { fprintf(stderr, "Realloc vertices failed!\n"); exit(1); }
            }
            (*out_vertices)[*out_num_vertices] = clipped[j];
            (*out_num_vertices)++;
        }

        int index_start = *out_num_indices;
        int index_count = 0;
        triangulate_polygon(clipped, clipped_count, *out_indices + index_start, &index_count, base);
        *out_num_indices += index_count;

        if (*out_num_indices >= index_alloc) {
            index_alloc *= 2;
            *out_indices = realloc(*out_indices, index_alloc * sizeof(int));
            if (!*out_indices) { fprintf(stderr, "Realloc indices failed!\n"); exit(1); }
        }
    }

    *out_vertices = realloc(*out_vertices, *out_num_vertices * sizeof(vec4f));
    *out_indices = realloc(*out_indices, *out_num_indices * sizeof(int));
}



int culling_check_point_in_range(vec4f point)
{
    // check if the point is within the view frustum
    return (point.x >= -1.0f && point.x <= 1.0f &&
            point.y >= -1.0f && point.y <= 1.0f &&
            point.z >= -1.0f && point.z <= 1.0f);
}

void culling_find_line_intersection(vec4f v1, vec4f v2, vec4f* intersection)
{
    // This function will find the intersection of the line segment defined by v1 and v2 with the view frustum planes.
    // For simplicity, we will assume the view frustum is defined by the planes x = -1, x = 1, y = -1, y = 1, z = -1, z = 1.
    

    // there are three key steps:
    // 1. Determine which plane the line segment intersects-- which plane is sandwiched between the two points?
    // 2. Calculate the intersection point using linear interpolation.
    // 3. Return the intersection point.
    
    if (v1.x < -1.0f && v2.x > -1.0f)
    {
        // intersects the left plane
        intersection->x = -1.0f;
        intersection->y = v1.y + (v2.y - v1.y) * (-1.0f - v1.x) / (v2.x - v1.x);
        intersection->z = v1.z + (v2.z - v1.z) * (-1.0f - v1.x) / (v2.x - v1.x);
    }
    else if (v1.x > 1.0f && v2.x < 1.0f)
    {
        // intersects the right plane
        intersection->x = 1.0f;
        intersection->y = v1.y + (v2.y - v1.y) * (1.0f - v1.x) / (v2.x - v1.x);
        intersection->z = v1.z + (v2.z - v1.z) * (1.0f - v1.x) / (v2.x - v1.x);
    }
    else if (v1.y < -1.0f && v2.y > -1.0f)
    {
        // intersects the bottom plane
        intersection->y = -1.0f;
        intersection->x = v1.x + (v2.x - v1.x) * (-1.0f - v1.y) / (v2.y - v1.y);
        intersection->z = v1.z + (v2.z - v1.z) * (-1.0f - v1.y) / (v2.y - v1.y);
    }
    else if (v1.y > 1.0f && v2.y < 1.0f)
    {
        // intersects the top plane
        intersection->y = 1.0f;
        intersection->x = v1.x + (v2.x - v1.x) * (1.0f - v1.y) / (v2.y - v1.y);
        intersection->z = v1.z + (v2.z - v1.z) * (1.0f - v1.y) / (v2.y - v1.y);
    }
    else if (v1.z < -1.0f && v2.z > -1.0f)
    {
        // intersects the near plane
        intersection->z = -1.0f;
        intersection->x = v1.x + (v2.x - v1.x) * (-1.0f - v1.z) / (v2.z - v1.z);
        intersection->y = v1.y + (v2.y - v1.y) * (-1.0f - v1.z) / (v2.z - v1.z);
    }
    else if (v1.z > 1.0f && v2.z < 1.0f)
    {
        // intersects the far plane
        intersection->z = 1.0f;
        intersection->x = v1.x + (v2.x - v1.x) * (1.0f - v1.z) / (v2.z - v1.z);
        intersection->y = v1.y + (v2.y - v1.y) * (1.0f - v1.z) / (v2.z - v1.z);
    }
}


int inside(vec4f v, int axis, float sign) {
    float c = (axis == 0 ? v.x : axis == 1 ? v.y : v.z) * sign;
    return c <= v.w;
}

vec4f intersect(vec4f a, vec4f b, int axis, float sign) {
    float a_c = (axis == 0 ? a.x : axis == 1 ? a.y : a.z) * sign;
    float b_c = (axis == 0 ? b.x : axis == 1 ? b.y : b.z) * sign;

    float t = (a.w - a_c) / ((a.w - a_c) - (b.w - b_c));
    vec4f r;
    r.x = a.x + t * (b.x - a.x);
    r.y = a.y + t * (b.y - a.y);
    r.z = a.z + t * (b.z - a.z);
    r.w = a.w + t * (b.w - a.w);
    return r;
}

void clip_polygon_against_plane(polygon4f* in, polygon4f* out, int axis, float sign) {
    out->count = 0;
    for (int i = 0; i < in->count; i++) {
        vec4f curr = in->verts[i];
        vec4f prev = in->verts[(i - 1 + in->count) % in->count];

        int curr_in = inside(curr, axis, sign);
        int prev_in = inside(prev, axis, sign);

        if (curr_in && prev_in)
            out->verts[out->count++] = curr;
        else if (!prev_in && curr_in) {
            out->verts[out->count++] = intersect(prev, curr, axis, sign);
            out->verts[out->count++] = curr;
        } else if (prev_in && !curr_in) {
            out->verts[out->count++] = intersect(prev, curr, axis, sign);
        }
    }
}

void clip_triangle(vec4f v0, vec4f v1, vec4f v2, vec4f* out_verts, int* out_vert_count) {
    polygon4f bufferA = { .verts = { v0, v1, v2 }, .count = 3 };
    polygon4f bufferB;

    polygon4f* in = &bufferA;
    polygon4f* out = &bufferB;

    for (int axis = 0; axis < 3; axis++) {
        clip_polygon_against_plane(in, out, axis, +1.0f); // axis <= w
        if (out->count == 0) { *out_vert_count = 0; return; }
        polygon4f* tmp = in; in = out; out = tmp;

        clip_polygon_against_plane(in, out, axis, -1.0f); // -axis <= w
        if (out->count == 0) { *out_vert_count = 0; return; }
        tmp = in; in = out; out = tmp;
    }

    *out_vert_count = in->count;
    for (int i = 0; i < in->count; i++) {
        out_verts[i] = in->verts[i];
    }
}

void triangulate_polygon(vec4f* verts, int count, int* out_indices, int* out_index_count, int base_index) {
    *out_index_count = 0;
    for (int i = 1; i < count - 1; i++) {
        out_indices[(*out_index_count)++] = base_index;
        out_indices[(*out_index_count)++] = base_index + i;
        out_indices[(*out_index_count)++] = base_index + i + 1;
    }
}
