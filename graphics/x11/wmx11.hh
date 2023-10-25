#ifndef WMX11_HH_
#define WMX11_HH_

#include "../wm.hh"
#include "theme/theme.hh"
#include "wmwindow.hh"

#include <unordered_map>
#include <X11/Xlib.h>

class WMX11 : public WM {
public:
    explicit WMX11(Theme& theme, Display* dpy);

    void run() override;
    [[nodiscard]] std::string interface_name() const override { return "X11"; }

private:
    Theme&   theme_;
    Display* dpy_;
    Window   root_;
    std::unordered_map<intptr_t, WM_Window> windows_;

    void setup_event_filter();
    void add_existing_windows();

    [[noreturn]] void main_loop();

    void on_map_request(Window child_id);
    void on_unmap_notify(XUnmapEvent const &e);
    void on_expose(XExposeEvent const &e);
    static int on_error(Display* d, XErrorEvent* e);
};

#endif //WMX11_HH_
