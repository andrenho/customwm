#ifndef GRAPHICS_HH_
#define GRAPHICS_HH_

#include <string>
#include <vector>

#include "event.hh"
#include "theme/types/types.hh"

class Graphics {
public:
    explicit Graphics(class Options* options) : options_(options) {}
    virtual ~Graphics() = default;

    virtual void init() = 0;

    // information
    virtual std::string interface_name() const = 0;
    virtual Size        screen_size() const = 0;

    // window actions
    virtual WindowHandle create_window(Rectangle const& rectangle) = 0;
    virtual void         destroy_window(WindowHandle window) = 0;
    virtual void         reparent_window(WindowHandle parent, WindowHandle child, Point const& offset) = 0;
    virtual void         unparent_window(WindowHandle child) = 0;
    virtual void         window_fill(WindowHandle window, Color const& color, Rectangle const& rect) = 0;
    virtual void         window_swap_buffers(WindowHandle window, Rectangle const& rectangle) = 0;
    virtual std::vector<WindowHandle> toplevel_windows() const = 0;

    // window information
    virtual Rectangle   get_window_rectangle(WindowHandle window) const = 0;

    // events
    virtual void subscribe_to_wm_events() = 0;
    virtual std::optional<Event> next_event() = 0;

protected:
    class Options* options_;
};

#endif //GRAPHICS_HH_
