#ifndef SERVER_X11_HH_
#define SERVER_X11_HH_

#include "../server.hh"

#include <xcb/xcb.h>
#include <xcb/xcb_errors.h>

class ServerX11 : public Server {
public:
    ServerX11(xcb_connection_t* dpy, xcb_screen_t* scr, xcb_errors_context_t* err_ctx)
        : dpy_(dpy), scr_(scr), err_ctx_(err_ctx) {}

    void setup_event_listeners() override;
    void capture_existing_windows() override;
    void run_event_loop() override;

private:
    xcb_connection_t*     dpy_ = nullptr;
    xcb_screen_t*         scr_ = nullptr;
    xcb_errors_context_t* err_ctx_ = nullptr;

    void on_error(xcb_request_error_t *e);
};

#endif //SERVER_X11_HH_
