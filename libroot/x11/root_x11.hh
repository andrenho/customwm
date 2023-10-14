#ifndef ROOT_X11_HH_
#define ROOT_X11_HH_

#include <memory>

#include "../root.hh"

#include <xcb/xcb.h>
#include <xcb/xcb_errors.h>

class RootX11 : public Root {
public:
    RootX11(std::optional<std::string> const& display);

    std::string interface_name() const override { return "x11"; }

    void setup_event_listeners(EventListener &event_listener) override;
    void capture_existing_windows() override;
    void run_event_loop() override;

private:
    xcb_connection_t*     dpy_ = nullptr;
    xcb_screen_t*         scr_ = nullptr;
    xcb_errors_context_t* err_ctx;
};

#endif //ROOT_X11_HH_
