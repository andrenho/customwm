#ifndef GRAPHICS_HH_
#define GRAPHICS_HH_

#include <memory>
#include <string>
#include <vector>

#include "event.hh"
#include "options/options.hh"
#include "theme/types/types.hh"
#include "graphics/pencil/pencil.hh"

class Graphics {
public:
    virtual ~Graphics() = default;

    virtual void init() = 0;

    // information
    virtual std::string               interface_name() const = 0;
    virtual Size                      screen_size() const = 0;
    virtual std::vector<WindowHandle> toplevel_windows() const = 0;

    // window actions
    virtual WindowHandle create_window(Rectangle const& rectangle) = 0;
    virtual void         destroy_window(WindowHandle window) = 0;
    virtual void         reparent_window(WindowHandle parent, WindowHandle child, Point const& offset) = 0;
    virtual void         unparent_window(WindowHandle child) = 0;

    // window information
    virtual Rectangle   get_window_rectangle(WindowHandle window) const = 0;

    // events
    virtual void subscribe_to_wm_events() = 0;
    virtual std::optional<Event> next_event() = 0;

    // pencil
    virtual std::unique_ptr<Pencil> create_pencil(Window_* window) const = 0;

    // factory
    static std::unique_ptr<Graphics> create_unique_ptr(Options* options, class Theme* theme);

protected:
    explicit Graphics(class Options* options) : options_(options) {}

    class Options* options_;
};

#endif //GRAPHICS_HH_
