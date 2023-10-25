#ifndef WMX11_HH_
#define WMX11_HH_

#include "../wm.hh"

#include <X11/Xlib.h>

class WMX11 : public WM {
public:
    explicit WMX11(Display* dpy);

    void run() override;

private:
    Display* dpy_;
    Window   root_;

    void setup_event_filter();

    void add_existing_windows();
    [[noreturn]] void main_loop();

    void on_map_request(Window child_id);
    void on_unmap_notify(XUnmapEvent const &e);
    void on_expose(XExposeEvent const &e);

    static int on_error(Display* d, XErrorEvent* e);

    void initialize_xcomposite();
};

#endif //WMX11_HH_
