#include "brush.hh"

Brush::Brush(xcb_connection_t* dpy, xcb_screen_t* scr, xcb_window_t outer_id)
    : dpy_(dpy), scr_(scr), outer_id_(outer_id)
{
    gc_ = xcb_generate_id(dpy);
    xcb_create_gc (dpy, gc_, outer_id, 0, nullptr);
}

Brush::~Brush()
{
    xcb_free_gc(dpy_, gc_);
}

void Brush::set_color(const char* str)
{
    uint32_t color = scr_->white_pixel;

    uint32_t mask = XCB_GC_FOREGROUND;
    xcb_change_gc(dpy_, gc_, mask, &color);
    xcb_flush(dpy_);  // TODO
}

void Brush::draw_rect(int x, int y, int w, int h, bool filled)
{
    xcb_rectangle_t rectangle = { static_cast<int16_t>(x), static_cast<int16_t>(y), static_cast<uint16_t>(w), static_cast<uint16_t>(h) };
    if (filled) {
        xcb_poly_fill_rectangle(dpy_, outer_id_, gc_, 1, &rectangle);
    } else {
        xcb_poly_rectangle(dpy_, outer_id_, gc_, 1, &rectangle);
    }
    xcb_flush(dpy_);
}
