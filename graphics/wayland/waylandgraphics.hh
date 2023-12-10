#ifndef CUSTOMWM_WAYLANDGRAPHICS_HH
#define CUSTOMWM_WAYLANDGRAPHICS_HH

extern "C" {
#include <wayland-server-core.h>
#include <wlr/backend.h>
}

#include "graphics/graphics.hh"
#include "window/window.hh"

class WaylandGraphics : public Graphics {
public:
    WaylandGraphics(class Options *options, [[maybe_unused]] class Theme* theme) : Graphics(options) {}
    ~WaylandGraphics() override;

    void init() override;

    // information
    std::string               interface_name() const override { return "wayland"; }
    Size                      screen_size() const override;
    std::vector<WindowHandle> toplevel_windows() const override;

    // window actions
    WindowHandle create_window(const Rectangle &rectangle) override;
    void         destroy_window(WindowHandle window) override;
    void         reparent_window(WindowHandle parent, WindowHandle child, const Point &offset) override;
    void         unparent_window(WindowHandle child) override;
    void         paint(Window_ *window, std::function<void()> paint_function) override;

    // window information
    Rectangle   get_window_rectangle(WindowHandle window) const override;

    // events
    void                 subscribe_to_wm_events() override;
    std::optional<Event> next_event() override;

    // pencil
    std::unique_ptr<Pencil> create_pencil(Window_ *window) override;

private:
    wl_display*    display = nullptr;
    wl_event_loop* event_loop = nullptr;
    wlr_backend*   backend = nullptr;
};

#endif //CUSTOMWM_WAYLANDGRAPHICS_HH
