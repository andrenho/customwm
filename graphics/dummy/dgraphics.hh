#ifndef DGRAPHICS_HH_
#define DGRAPHICS_HH_

#include "graphics/graphics.hh"

#define MU [[maybe_unused]]

class DGraphics : public Graphics {
public:
    explicit DGraphics(class Options* options) : Graphics(options) {}

    void init() override {}

    // information
    std::string               interface_name() const override { return "dummy"; }
    Size                      screen_size() const override { return { 0, 0 }; }
    std::vector<WindowHandle> toplevel_windows() const override { return {}; }

    // window information
    Rectangle get_window_rectangle(MU WindowHandle window) const override { return { 0, 0, 0, 0 }; }

    // window actions
    WindowHandle create_window(MU Rectangle const &rectangle) override { return 0; }
    void         destroy_window(MU WindowHandle window) override {}
    void         reparent_window(MU WindowHandle parent, MU WindowHandle child, MU Point const &offset) override {}
    void         unparent_window(MU WindowHandle child) override {}

    // events
    void                 subscribe_to_wm_events() override {}
    std::optional<Event> next_event() override { return Quit(); }

    // pencil
    std::unique_ptr<Pencil> create_pencil(MU Window_* window) const override { return nullptr; }
};

#undef MU

#endif //DGRAPHICS_HH_
