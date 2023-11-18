#ifndef GRAPHICSX11_HH_
#define GRAPHICSX11_HH_

#include <unordered_map>

#include <X11/Xlib.h>

#include "graphics/graphics.hh"

class GraphicsX11 : public Graphics {
public:
    explicit GraphicsX11(class Options *options) : Graphics(options) {}
    ~GraphicsX11() override;

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

    // events
    void                 subscribe_to_wm_events() override;
    std::optional<Event> next_event() override;

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

    struct WindowInfo {
        Pixmap backbuffer;
        GC     gc;
        // TODO - add Xft
    };
    std::unordered_map<WindowHandle, WindowInfo> window_info_;
};

#endif //GRAPHICSX11_HH_
