#ifndef XGRAPHICS_HH_
#define XGRAPHICS_HH_

#include <unordered_map>

#include <X11/Xlib.h>

#include "graphics/graphics.hh"
#include "xresources.hh"

class XGraphics : public Graphics {
public:
    explicit XGraphics(class Options *options, class Theme* theme)
            : Graphics(options), resources_(this, theme) {}
    ~XGraphics() override;

    void init() override;

    // information
    std::string interface_name() const override { return "X11"; }
    Size        screen_size() const override;

    // window information
    Rectangle get_window_rectangle(WindowHandle window) const override;

    // window actions
    WindowHandle create_window(Rectangle const& rectangle) override;
    void         destroy_window(WindowHandle window) override;
    void         reparent_window(WindowHandle parent, WindowHandle child, Point const& offset) override;
    void         unparent_window(WindowHandle child) override;
    void         window_fill(WindowHandle window, Color const& color, Rectangle const& rect) override;
    void         window_swap_buffers(WindowHandle window, Rectangle const& rectangle) override;
    std::vector<WindowHandle> toplevel_windows() const override;

    // events
    void                 subscribe_to_wm_events() override;
    std::optional<Event> next_event() override;

    friend class XResources;

private:
    Display*      display  = nullptr;
    int           screen   = 0;
    Window        root     = None;
    Visual*       visual   = nullptr;
    GC            gc       = None;
    Colormap      colormap = None;
    int           depth    = 0;
    unsigned long black    = 0;
    unsigned long white    = 0;

    XResources    resources_;

    struct WindowInfo {
        Pixmap backbuffer;
        GC     gc;
        // TODO - add Xft
    };
    std::unordered_map<WindowHandle, WindowInfo> window_info_;
};

#endif //XGRAPHICS_HH_
