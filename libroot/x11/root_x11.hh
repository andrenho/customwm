#ifndef ROOT_X11_HH_
#define ROOT_X11_HH_

#include <memory>

#include "../root.hh"
#include "server_x11.hh"

#include <xcb/xcb.h>
#include <xcb/xcb_errors.h>

class RootX11 : public Root {
public:
    RootX11(std::optional<std::string> const& display);
    ~RootX11() override;

    std::string interface_name() const override { return "x11"; }
    Server& server() override { return *server_; }

private:
    xcb_connection_t*     dpy_ = nullptr;
    xcb_screen_t*         scr_ = nullptr;
    xcb_errors_context_t* err_ctx;
    std::unique_ptr<ServerX11> server_;
};

#endif //ROOT_X11_HH_
