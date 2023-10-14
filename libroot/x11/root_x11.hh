#ifndef ROOT_X11_HH_
#define ROOT_X11_HH_

#include <memory>

#include "../root.hh"
#include "wm_x11.hh"

#include <xcb/xcb.h>

class RootX11 : public Root {
public:
    RootX11(std::optional<std::string> const& display);

    std::string interface_name() const override { return "x11"; }
    std::unique_ptr<WM> create_wm() override { return std::make_unique<WM_X11>(); }

private:
    xcb_connection_t*     dpy_ = nullptr;
};

#endif //ROOT_X11_HH_
