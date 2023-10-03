#include "brush.hh"

void Brush::set_color(Handle gc, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = 0xFFFFFFFF; // white on my system (TODO)

    uint32_t mask = XCB_GC_FOREGROUND;
    xcb_change_gc(dpy_, gc, mask, &color);
}
