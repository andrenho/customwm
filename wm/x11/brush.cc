#include "brush.hh"

void Brush::set_color(Handle gc, uint8_t r, uint8_t g, uint8_t b)
{
    uint32_t color = 0xFFFFFFFF; // white on my system (TODO)

    uint32_t mask = XCB_GC_FOREGROUND;
    xcb_change_gc(dpy_, gc, mask, &color);
    xcb_flush(dpy_);
}

void Brush::draw_rect(Handle window, Handle gc, int x, int y, int w, int h, bool filled)
{
    xcb_rectangle_t rectangle = { static_cast<int16_t>(x), static_cast<int16_t>(y), static_cast<uint16_t>(w), static_cast<uint16_t>(h) };
    if (filled) {
        xcb_poly_fill_rectangle(dpy_, window, gc, 1, &rectangle);
    } else {
        xcb_poly_rectangle(dpy_, window, gc, 1, &rectangle);
    }
    xcb_flush(dpy_);
}
