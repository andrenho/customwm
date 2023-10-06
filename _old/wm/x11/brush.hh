#ifndef BRUSH_HH_
#define BRUSH_HH_

#include <xcb/xcb.h>
#include "../../lib/types/ibrush.hh"

class Brush : public IBrush {
public:
    Brush(xcb_connection_t* dpy, xcb_screen_t* scr, xcb_window_t outer_id);
    ~Brush();

    Brush(Brush const&) = delete;
    Brush& operator=(Brush const&) = delete;

    Brush(Brush&&) = default;
    Brush& operator=(Brush&&) = default;

    void set_color(const char* str) override;
    void draw_rect(int x, int y, int w, int h, bool filled) override;

private:
    xcb_connection_t* dpy_;
    xcb_screen_t* scr_;
    xcb_window_t outer_id_;
    xcb_gcontext_t gc_;
};

#endif //BRUSH_HH_
