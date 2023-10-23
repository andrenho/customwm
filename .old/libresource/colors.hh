#ifndef COLORS_HH_
#define COLORS_HH_

#include <map>

#include <xcb/xcb.h>

#include "../libtheme/types/types.hh"

class Colors {
public:
    Colors(xcb_connection_t* dpy, xcb_screen_t* scr, xcb_window_t window);
    ~Colors();

    uint32_t get_color(Color const& color) const;

private:
    xcb_connection_t* dpy_;
    xcb_screen_t*     scr_;
    xcb_colormap_t    colormap_;

    std::map<Color, uint32_t> colors_;
};

#endif //COLORS_HH_
