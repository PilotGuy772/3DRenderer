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


void culling_cull_triangle(vec4f* vertices, int num_vertices, int* indices, int num_indices, vec4f** out_vertices, int* out_num_vertices, int** out_indices, int* out_num_indices)
{
    int vertex_alloc = num_vertices * 2; // starting guess
    int index_alloc = num_indices * 2;

    *out_num_vertices = 0;
    *out_num_indices = 0;
    *out_vertices = malloc(vertex_alloc * sizeof(vec4f));
    *out_indices = malloc(index_alloc * sizeof(int));

    if (!*out_vertices || !*out_indices) {
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }

    for (int i = 0; i < num_indices; i += 3)
    {
        int v1_index = indices[i];
        int v2_index = indices[i + 1];
        int v3_index = indices[i + 2];

        vec4f v1 = vertices[v1_index];
        vec4f v2 = vertices[v2_index];
        vec4f v3 = vertices[v3_index];

        int v1_in = culling_check_point_in_range(v1);
        int v2_in = culling_check_point_in_range(v2);
        int v3_in = culling_check_point_in_range(v3);

        int num_inside = v1_in + v2_in + v3_in;

        int needed_vertices = 0;
        int needed_indices = 0;

        if (num_inside == 1)
        {
            needed_vertices = 3;
            needed_indices = 3;
        }
        else if (num_inside == 2)
        {
            needed_vertices = 6;
            needed_indices = 6;
        }
        else if (num_inside == 3)
        {
            needed_vertices = 3;
            needed_indices = 3;
        }

        // Check and grow allocations if needed
        if (*out_num_vertices + needed_vertices > vertex_alloc) {
            vertex_alloc *= 2;
            *out_vertices = realloc(*out_vertices, vertex_alloc * sizeof(vec4f));
            if (!*out_vertices) {
                fprintf(stderr, "Realloc vertices failed!\n");
                exit(1);
            }
        }

        if (*out_num_indices + needed_indices > index_alloc) {
            index_alloc *= 2;
            *out_indices = realloc(*out_indices, index_alloc * sizeof(int));
            if (!*out_indices) {
                fprintf(stderr, "Realloc indices failed!\n");
                exit(1);
            }
        }

        if (num_inside == 0)
        {
            continue; // discard
        }
        else if (num_inside == 1)
        {
            vec4f inside, i1, i2;
            if (v1_in) {
                inside = v1;
                culling_find_line_intersection(v2, v1, &i1);
                culling_find_line_intersection(v3, v1, &i2);
            } else if (v2_in) {
                inside = v2;
                culling_find_line_intersection(v1, v2, &i1);
                culling_find_line_intersection(v3, v2, &i2);
            } else {
                inside = v3;
                culling_find_line_intersection(v1, v3, &i1);
                culling_find_line_intersection(v2, v3, &i2);
            }

            int base = *out_num_vertices;
            (*out_vertices)[base] = inside;
            (*out_vertices)[base + 1] = i1;
            (*out_vertices)[base + 2] = i2;

            (*out_indices)[*out_num_indices + 0] = base;
            (*out_indices)[*out_num_indices + 1] = base + 1;
            (*out_indices)[*out_num_indices + 2] = base + 2;

            *out_num_vertices += 3;
            *out_num_indices += 3;
        }
        else if (num_inside == 2)
        {
            vec4f in1, in2, out, i1, i2;

            if (!v1_in) {
                out = v1; in1 = v2; in2 = v3;
                culling_find_line_intersection(v1, v2, &i1);
                culling_find_line_intersection(v1, v3, &i2);
            } else if (!v2_in) {
                out = v2; in1 = v1; in2 = v3;
                culling_find_line_intersection(v2, v1, &i1);
                culling_find_line_intersection(v2, v3, &i2);
            } else {
                out = v3; in1 = v1; in2 = v2;
                culling_find_line_intersection(v3, v1, &i1);
                culling_find_line_intersection(v3, v2, &i2);
            }

            /*
                We have to make one triangle with the two inside vertices and an intersection point,
                and one triangle with the two intersection points and one inside vertex.
                UNFORTUNATELY: what I didn't realize before is that it DOES matter which inside vertex we use!
                We have to use the one that's OPPOSITE the intersection point we used for the triangle
                that uses one intersection point and the two inside vertices.

                How do we determine which inside vertex to use?
                Well, if we have two inside vertices in1 and in2, one outside vertex out, and two intersection points i1 and i2,
                we can tell whether an intersection point shares a face with an inside vertex by checking if the
                intersection point is collinear with the inside vertex and the outside vertex.

                So: Triangle 1 will ALWAYS be made with in1, in2, and i1.

                Triangle 2 will be made with i1 and i2
                We determine whether in1 is collinear with i1 and out. If it is, we use in2 for Triangle 2. Otherwise, we use in1.
            */

            // Triangle 1
            int base1 = *out_num_vertices;
            (*out_vertices)[base1] = in1;
            (*out_vertices)[base1 + 1] = in2;
            (*out_vertices)[base1 + 2] = i1;

            (*out_indices)[*out_num_indices + 0] = base1;
            (*out_indices)[*out_num_indices + 1] = base1 + 1;
            (*out_indices)[*out_num_indices + 2] = base1 + 2;

            *out_num_vertices += 3;
            *out_num_indices += 3;

            // Triangle 2
            int base2 = *out_num_vertices;
            int in1_collinear = vec3_collinear(in1, i1, out, 0.001f);
            (*out_vertices)[base2] = i1;
            (*out_vertices)[base2 + 1] = i2;
            (*out_vertices)[base2 + 2] = (in1_collinear) ? in2 : in1;

            (*out_indices)[*out_num_indices + 0] = base2;
            (*out_indices)[*out_num_indices + 1] = base2 + 1;
            (*out_indices)[*out_num_indices + 2] = base2 + 2;

            *out_num_vertices += 3;
            *out_num_indices += 3;
        }
        else
        {
            // All inside
            int base = *out_num_vertices;
            (*out_vertices)[base] = v1;
            (*out_vertices)[base + 1] = v2;
            (*out_vertices)[base + 2] = v3;

            (*out_indices)[*out_num_indices + 0] = base;
            (*out_indices)[*out_num_indices + 1] = base + 1;
            (*out_indices)[*out_num_indices + 2] = base + 2;

            *out_num_vertices += 3;
            *out_num_indices += 3;
        }
    }

    // Optional: shrink to actual size
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