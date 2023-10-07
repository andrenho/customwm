#ifndef WINDOW_HH_
#define WINDOW_HH_

#include <cstdint>
#include <xcb/xcb.h>

#include "../libtheme/types/interfaces.hh"

struct Window : public IWindow {
    Window(xcb_connection_t *dpy, xcb_window_t root, Rectangle area, xcb_window_t child_id, Point child_pos);
    ~Window();

    Window(Window const&) = delete;
    Window& operator=(Window const&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    const uint32_t id;
    const uint32_t child_id;

    Rectangle area() const override { return area_; }
    void draw_rectangles(const std::vector<Rectangle> &rectangles, const std::string &color, bool filled) override;

private:
    xcb_connection_t* dpy_;
    xcb_window_t      root_;
    Rectangle         area_;
    xcb_gcontext_t    gc_;
};

#endif //WINDOW_HH_
