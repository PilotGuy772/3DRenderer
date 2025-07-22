#include "fragment.h"

fragment* fragments;
int fragments_size;
int fragment_index;

void add_fragment(fragment frag)
{
    if (fragment_index >= fragments_size - 1)
    {
        fragments_size *= 2;
        fragments = realloc(fragments, fragments_size * sizeof(fragment));
    }

    fragment_index++;
    fragments[fragment_index] = frag;
}

void draw_fragments(vertex *vertices, int num_vertices, face *faces, int num_faces, fragment *out_fragments, int num_fragments)
{
    out_fragments = malloc(window_width * window_height * sizeof(fragment));
    for (int i = 0; i < num_faces; i++)
    {
        fragment_line(vertices[faces[i].indices[0]], vertices[faces[i].indices[1]]);
        fragment_line(vertices[faces[i].indices[0]], vertices[faces[i].indices[2]]);
        fragment_line(vertices[faces[i].indices[1]], vertices[faces[i].indices[2]]);
    }

    fragments = realloc(fragments, (fragment_index + 1) * sizeof(fragment));
    out_fragments = fragments;
    num_fragments = fragment_index + 1;
}

void fragment_line(vertex a, vertex b)
{
    int dy = (int)round(b.position.y) - (int)round(a.position.y);
    int dx = (int)round(b.position.x) - (int)round(a.position.x);
    int dz = (int)round(b.position.z) - (int)round(a.position.z);
    vec2 point_a = (vec2){a.position.x, a.position.y};
    vec2 point_b = (vec2){b.position.x, b.position.y};
    float dist = vec2_distance(point_a, point_b);


    if (abs(dy) <= abs(dx))
    {
        if (a.position.x > b.position.x)
        {
            vertex tmp = a;
            a = b;
            b = tmp;
        }

        float z = a.position.z;
        for (int x = a.position.x; x <= b.position.x; x++)
        {
            int y = (int) (((float)dy / dx) * (x - a.position.x) + a.position.y);
            vec2 point = (vec2){x, y};
            add_fragment((fragment){
                point,
                z,
                (vec2_distance(point, point_a) * a.color / dist) 
                + (vec2_distance(point, point_b) * b.color / dist), // we will use proper interpolation later
                a.normal,
            });

            z += ((float)dz / dx) * (x - a.position.x);
        }
    }
    else
    {
        if (a.position.y > b.position.y)
        {
            vertex tmp = a;
            a = b;
            b = tmp;
        }

        float z = a.position.z;
        for (int y = a.position.y; y <= b.position.y; y++)
        {
            int x = (int) (((float)dx / dy) * (y - a.position.y) + a.position.x);
            vec2 point = (vec2){x, y};
            add_fragment((fragment){
                point,
                z,
                (vec2_distance(point, point_a) * a.color / dist) 
                + (vec2_distance(point, point_b) * b.color / dist), // we will use proper interpolation later
                a.normal,
            });

            z += ((float)dz / dy) * (y - a.position.y);
        }
    }
}