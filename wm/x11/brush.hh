#ifndef BRUSH_HH_
#define BRUSH_HH_

#include <xcb/xcb.h>
#include "../../lib/types/ibrush.hh"

class Brush : public IBrush {
public:
    explicit Brush(xcb_connection_t* dpy) : dpy_(dpy) {}

    void set_color(Handle gc, uint8_t r, uint8_t g, uint8_t b) override;

private:
    xcb_connection_t* dpy_;
};

#endif //BRUSH_HH_
