#ifndef DGRAPHICS_HH_
#define DGRAPHICS_HH_

#include "graphics/graphics.hh"

#define X [[maybe_unused]]

class DGraphics : public Graphics {
public:
    explicit DGraphics(class Options* options) : Graphics(options) {}

    void init() override {}

    // information
    std::string               interface_name() const override { return "dummy"; }
    Size                      screen_size() const override { return { 0, 0 }; }
    std::vector<WindowHandle> toplevel_windows() const override { return {}; }

    // window information
    Rectangle get_window_rectangle(X WindowHandle window) const override { return { 0, 0, 0, 0 }; }

    // window actions
    WindowHandle create_window(X Rectangle const &rectangle) override { return 0; }
    void         destroy_window(X WindowHandle window) override {}
    void         reparent_window(X WindowHandle parent, X WindowHandle child, X Point const &offset) override {}
    void         unparent_window(X WindowHandle child) override {}

    // draw
    void window_fill(X WindowHandle window, X Color const &color, X Rectangle const &rect) override {}
    void window_swap_buffers(X WindowHandle window, X Rectangle const &rectangle) override {}

    // events
    void                 subscribe_to_wm_events() override {}
    std::optional<Event> next_event() override { return Quit(); }
};

#undef X

#endif //DGRAPHICS_HH_
