#ifndef WMX11_HH_
#define WMX11_HH_

#include "../wm.hh"
#include "theme/theme.hh"
#include "windowx11.hh"
#include "resourcesx11.hh"

#include <unordered_map>
#include <X11/Xlib.h>

class WMX11 : public WM {
public:
    explicit WMX11(Theme& theme, Display* dpy, ResourcesX11& resources);

    void run() override;
    [[nodiscard]] std::string interface_name() const override { return "X11"; }

    void move_window_with_mouse(bool move, std::optional<L_Window*> window) override;

private:
    Theme&   theme_;
    Display* dpy_;
    ResourcesX11& resources_;
    Window   root_;
    std::unordered_map<Window, std::unique_ptr<WindowX11>> windows_;

    std::optional<WindowX11*> moving_window_with_mouse_ {};
    Point last_mouse_position_ { 0, 0 };

    void setup_event_filter();
    void add_existing_windows();

    [[noreturn]] void main_loop();

    void on_map_request(Window child_id);
    void on_unmap_notify(XUnmapEvent const &e);
    void on_expose(XExposeEvent const &e);
    void on_click(XButtonEvent const &e);
    static int on_error(Display* d, XErrorEvent* e);

    void on_move(XMotionEvent const &xmotion);

    void on_configure(XConfigureEvent const &xconfigure);
};

#endif //WMX11_HH_
