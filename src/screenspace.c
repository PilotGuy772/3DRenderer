#include "screenspace.h"

float* depth_buffer;

void screenspace_draw_triangle(uint32_t* image, triangle tri)
{
    
    screenspace_draw_line(image, tri.a, tri.b);
    screenspace_draw_line(image, tri.b, tri.c);
    screenspace_draw_line(image, tri.c, tri.a);
}

void screenspace_draw_line(uint32_t* image, vec4f p1, vec4f p2)
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
            screenspace_add_point_depth((vec4f){x, y, z, 1.0f}, image);

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
            screenspace_add_point_depth((vec4f){x, y, z, 1.0f}, image);

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

void screenspace_from_ndc(vec4f *vertices, int num_vertices, float znear, float zfar, vec4f *out_vertices)
{
    // apply a basic transformation to convert from NDC to screen space
    for (int i = 0; i < num_vertices; i++)
    {
        // NDC coordinates are in the range [-1, 1]
        // convert to screen space coordinates
        /*
        x_screen = x_vp + ((x_ndc + 1) * w_vp / 2)
        y_screen = y_vp + ((y_ndc + 1) * h_vp / 2)
        z_screen = z_min + ((z_ndc + 1) * (z_max - z_min) / 2)
        */
        // out_vertices[i].x = /* 0 + */ ((vertices[i].x + 1.0f) * window_width / 2.0f);
        // out_vertices[i].y = window_height + ((vertices[i].y + 1.0f) * window_height / 2.0f);
        // out_vertices[i].z = znear + ((vertices[i].z + 1.0f) * (zfar - znear) / 2.0f);
        // out_vertices[i].w = vertices[i].w; // keep W as is

        out_vertices[i].x = (vertices[i].x + 1.0f) * 0.5f * window_width;
        out_vertices[i].y = (1.0f - (vertices[i].y + 1.0f) * 0.5f) * window_height; // flip Y axis
        out_vertices[i].z = vertices[i].z; // Z coordinate remains unchanged because we don't do anything with it for now
        out_vertices[i].w = vertices[i].w; // keep W as is
    }
}

void screenspace_draw_model(vec4f* screen_vertices, int num_indices, int* ibo, uint32_t* image)
{
    for (int i = 0; i < num_indices; i += 3)
    {
        triangle tri = {
            screen_vertices[ibo[i]],
            screen_vertices[ibo[i + 1]],
            screen_vertices[ibo[i + 2]]
        };
        screenspace_draw_triangle(image, tri);
    }
}

void screenspace_add_point_depth(vec4f point, uint32_t* image)
{
    if (point.z < depth_buffer[((int)round(point.y) * window_width) + (int)round(point.x)])
    {
        depth_buffer[((int)round(point.y) * window_width) + (int)round(point.x)] = point.z;
        image[((int)round(point.y) * window_width) + (int)round(point.x)] = 0xFF00FF00;
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
