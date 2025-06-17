#include "screenspace.h"

void screenspace_draw_triangle(uint32_t* image, triangle tri)
{
    screenspace_draw_line(image, tri.a, tri.b);
    screenspace_draw_line(image, tri.b, tri.c);
    screenspace_draw_line(image, tri.c, tri.a);
}

void screenspace_draw_line(uint32_t* image, screen_point p1, screen_point p2)
{
    /*
        1. Extrapolate the slope of the line
        2. Iterate through every whole-number X-coordinate between p1.x and p2.x
        3. For each X-coord, get the Y-coord on the line rounded to the nearest whole number
        4. Copy it on!
    */

    //p1 MUST be further to the left! If not, call this function again with parameters swapped, then return.
    if (p1.x == p2.x)
    {
        screenspace_draw_vertical_line(image, p1, p2);
        return;
    }

    //float slope = (float)(p2.y - p1.y) / (p2.x - p1.x);
    int dy = p2.y - p1.y;
    int dx = p2.x - p1.x;
    // equation is y = m(x - x1) + y1
    // which equates to y = slope * (x - p1.x) + p1.y
    
    if (abs(dy) <= abs(dx)) // slope <= 1
    {
        if (p1.x > p2.x)
        {
            screen_point tmp = p2;
            p2 = p1;
            p2 = tmp;
        }

        for (int x = p1.x; x <= p2.x; x++)
        {
            int y = (int) (((float)dy / dx) * (x - p1.x) + p1.y);
            image[y * window_width + x] = 0xFF00FF00;
        }
    }    
    else
    {
        if (p1.y > p2.y)
        {
            screen_point tmp = p2;
            p2 = p1;
            p2 = tmp;
        }

        for (int y = p1.y; y <= p2.y; y++)
        {
            int x = (int) (((float)dx / dy) * (y - p1.y) + p1.x);
            image[y * window_width + x] = 0xFF00FF00;
        }
    }

    // TEMP!! DEBUG!! plot the points at the edges
    screenspace_plot_point(image, p1);
    screenspace_plot_point(image, p2);
}

void screenspace_draw_vertical_line(uint32_t* image, screen_point p1, screen_point p2)
{
    if (p1.x != p2.x) return;
    if (p1.y > p2.y)
    {
        screenspace_draw_vertical_line(image, p2, p1);
        return;
    }

    for (int y = p1.y; y <= p2.y; y++)
    {
        image[y * window_width + p1.x] = 0xFF00FF00;
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