#include "brush.hh"

void Brush::set_color(uint8_t r, uint8_t g, uint8_t b)
{
    printf("Color: %d %d %d\n", r, g, b);
}

void Brush::fill_rectangle(Point p1, Point p2)
{
    printf("Fill rectangle: %d %d %d %d\n", p1.x, p1.y, p2.x, p2.y);
}
