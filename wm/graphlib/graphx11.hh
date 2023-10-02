#ifndef GRAPHX11_HH_
#define GRAPHX11_HH_

#include <xcb/xcb.h>

#include "graphiclibrary.hh"

class GraphX11 : public GraphicLibrary {
public:
    void setup(void* data) override;
    bool running() const override;
    void do_events(WM_Events *events) override;

    Area inner_window_size(const Window &w) const override;
    Area screen_size() const override;

    Handle reparent_window(const Window &w, const Point &pos, const Area &window_sz, const Padding &padding) override;
    void   destroy_window(const Window &w) override;

private:
    xcb_connection_t* dpy = nullptr;
    xcb_screen_t* scr = nullptr;
};

#endif //GRAPHX11_HH_
