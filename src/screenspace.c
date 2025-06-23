#include "screenspace.h"

float* depth_buffer;

void screenspace_draw_triangle(uint32_t* image, triangle tri, uint32_t color)
{
    
    screenspace_draw_line(image, tri.a, tri.b, color);
    screenspace_draw_line(image, tri.b, tri.c, color);
    screenspace_draw_line(image, tri.c, tri.a, color);
}

void screenspace_draw_line(uint32_t* image, vec4f p1, vec4f p2, uint32_t color)
{
    /*
        1. Extrapolate the slope of the line
        2. Iterate through every whole-number X-coordinate between p1.x and p2.x
        3. For each X-coord, get the Y-coord on the line rounded to the nearest whole number
        4. Copy it on!
    */

    //float slope = (float)(p2.y - p1.y) / (p2.x - p1.x);
    int dy = p2.y - p1.y;
    int dx = p2.x - p1.x;
    // dz is not used for rendering, but we use it for z-buffering
    int dz = p2.z - p1.z;
    // equation is y = m(x - x1) + y1
    // which equates to y = slope * (x - p1.x) + p1.y
    
    if (abs(dy) <= abs(dx)) // slope <= 1
    {
        if (p1.x > p2.x)
        {
            vec4f tmp = p1;
            p1 = p2;
            p2 = tmp;
        }

        float z = p1.z;
        for (int x = p1.x; x <= p2.x; x++)
        {
            int y = (int) (((float)dy / dx) * (x - p1.x) + p1.y);
            screenspace_add_point_depth((vec4f){x, y, z, 1.0f}, image, color);

            // add to z according to dz
            z += ((float)dz / dx) * (x - p1.x);
        }
    }    
    else
    {
        if (p1.y > p2.y)
        {
            vec4f tmp = p1;
            p1 = p2;
            p2 = tmp;
        }

        float z = p1.z;
        for (int y = p1.y; y <= p2.y; y++)
        {
            int x = (int) (((float)dx / dy) * (y - p1.y) + p1.x);
            screenspace_add_point_depth((vec4f){x, y, z, 1.0f}, image, color);

            // add to z according to dz (this time in terms of dy)
            z += ((float)dz / dy) * (y - p1.y);
        }
    }

    // screenspace_plot_point(image, p1);
    // screenspace_plot_point(image, p2);
}

void screenspace_draw_vertical_line(uint32_t* image, vec4f p1, vec4f p2)
{
    if (p1.x != p2.x) return;
    if (p1.y > p2.y)
    {
        screenspace_draw_vertical_line(image, p2, p1);
        return;
    }

    for (int y = p1.y; y <= p2.y; y++)
    {
        image[(int)round(y) * window_width + (int)round(p1.x)] = 0xFF00FF00;
    }
}

void screenspace_plot_point(uint32_t* image, screen_point p)
{
    for (int y = p.y - 2; y <= p.y + 2; y++)
    {
        for (int x = p.x - 2; x <= p.x + 2; x++)
        {
            image[y * window_width + x] = 0xFFFF0000;
        }
    }
}

void screenspace_from_ndc(vertex *vertices, int num_vertices, float znear, float zfar, vertex *out_vertices)
{
    // apply a basic transformation to convert from NDC to screen space
    for (int i = 0; i < num_vertices; i++)
    {
        // NDC coordinates are in the range [-1, 1]
        // convert to screen space coordinates

        out_vertices[i].position.x = (vertices[i].position.x + 1.0f) * 0.5f * window_width;
        out_vertices[i].position.y = (1.0f - (vertices[i].position.y + 1.0f) * 0.5f) * window_height; // flip Y axis
        out_vertices[i].position.z = vertices[i].position.z; // Z coordinate remains unchanged because we don't do anything with it for now
        out_vertices[i].position.w = vertices[i].position.w; // keep W as is
    }
}

void screenspace_draw_model(vec4f* screen_vertices, int num_indices, int* ibo, uint32_t* colors, uint32_t* image)
{
    int color_index = 0;
    for (int i = 0; i < num_indices; i += 3)
    {
        triangle tri = {
            screen_vertices[ibo[i]],
            screen_vertices[ibo[i + 1]],
            screen_vertices[ibo[i + 2]]
        };
        screenspace_draw_triangle(image, tri, colors[color_index]);
        color_index++;
    }
}

void screenspace_add_point_depth(vec4f point, uint32_t* image, uint32_t color)
{
    if (point.z < depth_buffer[((int)round(point.y) * window_width) + (int)round(point.x)])
    {
        depth_buffer[((int)round(point.y) * window_width) + (int)round(point.x)] = point.z;
        image[((int)round(point.y) * window_width) + (int)round(point.x)] = color;
    }
}

void screenspace_clear_depth_buffer(int width, int height)
{
    depth_buffer = malloc(width * height * sizeof(float));
    for (int i = 0; i < width * height; i++)
    {
        depth_buffer[i] = 1.0f;
    }
}
