#ifndef WMX11_HH_
#define WMX11_HH_

#include "../wm.hh"

#include <X11/Xlib.h>

class WMX11 : public WM {
public:
    explicit WMX11(Display* dpy) : dpy_(dpy) {}

    void run() override;

private:
    Display* dpy_;
};

#endif //WMX11_HH_
