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

void culling_cull_triangle(vec4f* vertices, int num_vertices, int* indices, int num_indices, vec4f* out_vertices, int* out_num_vertices, int* out_indices, int* out_num_indices)
{
    // allocate output arrays
    *out_num_vertices = 0;
    *out_num_indices = 0;
    out_vertices = malloc(num_vertices * sizeof(vec4f));
    out_indices = malloc(num_indices * sizeof(int));

    for (int i = 0; i < num_indices; i += 3)
    {
        int v1_index = indices[i];
        int v2_index = indices[i + 1];
        int v3_index = indices[i + 2];

        vec4f v1 = vertices[v1_index];
        vec4f v2 = vertices[v2_index];
        vec4f v3 = vertices[v3_index];

        int v1_in_range = culling_check_point_in_range(v1);
        int v2_in_range = culling_check_point_in_range(v2);
        int v3_in_range = culling_check_point_in_range(v3);

        // if all three vertices are outside
        if (!v1_in_range && !v2_in_range && !v2_in_range)
        {
            // discard this triangle
            continue;
        }
        // if one vertex is inside
        else if (v1_in_range + v2_in_range + v2_in_range == 1)
        {
            // implement later
        }
        // if two vertices are inside
        else if (v1_in_range + v2_in_range + v3_in_range == 2)
        {
            // implement later
        }
        else
        {
            // all three vertices are inside, so we can keep this triangle
            out_vertices[*out_num_vertices] = v1;
            out_vertices[*out_num_vertices + 1] = v2;
            out_vertices[*out_num_vertices + 2] = v3;

            out_indices[*out_num_indices++] = *out_num_vertices;
            out_indices[*out_num_indices++] = *out_num_vertices + 1;
            out_indices[*out_num_indices++] = *out_num_vertices + 2;

            out_num_vertices += 3;
        }
    }
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
    

    // first, determine which plane the line segment intersects
    //     

}