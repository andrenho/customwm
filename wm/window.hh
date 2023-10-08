#ifndef WINDOW_HH_
#define WINDOW_HH_

#include <cstdint>
#include <map>

#include <xcb/xcb.h>

#include "../libtheme/types/interfaces.hh"
#include "resources.hh"

struct Window : public IWindow {
    Window(xcb_connection_t *dpy, xcb_screen_t* scr, Rectangle area, xcb_window_t child_id, Point child_pos, Resources* res);
    ~Window();

    Window(Window const&) = delete;
    Window& operator=(Window const&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    const uint32_t id;
    const uint32_t child_id;

    Rectangle area() const override { return area_; }
    void draw_rectangles(std::vector<Rectangle> const& rectangles, Color const& color, bool filled) override;
    void draw_polygon(std::vector<Point> const& points, const Color &color, bool filled) override;

private:
    xcb_connection_t* dpy_;
    xcb_screen_t*     scr_;
    Rectangle         area_;
    xcb_gcontext_t    gc_;
    xcb_colormap_t    colormap_;
    Resources*        res_;

    std::map<Color, uint32_t> colors_;

    uint32_t get_color(Color const& color);
};

#endif //WINDOW_HH_
