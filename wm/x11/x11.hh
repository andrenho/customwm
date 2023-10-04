#ifndef X11_HH_
#define X11_HH_

#include <xcb/xcb.h>
#include "../wm/ievents.hh"
#include "../../lib/types/iwindow.hh"
#include "../../lib/types/ibrush.hh"
#include "brush.hh"
#include "../wm/window.hh"

class X11 {
public:
    void setup(std::string const& display_name);
    bool running() const;
    void do_events(IEvents *events);

    Area inner_window_size(const Window &w) const;
    Area screen_size() const;

    Handle reparent_window(const Window &w, const Point &pos, const Area &window_sz, const Padding &padding);
    void   destroy_window(const Window &w);

    std::unique_ptr<Brush> create_brush(Window const& w);

private:
    xcb_connection_t* dpy = nullptr;
    xcb_screen_t*     scr = nullptr;
};

#endif //X11_HH_
