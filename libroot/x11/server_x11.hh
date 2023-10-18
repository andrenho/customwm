#ifndef SERVER_X11_HH_
#define SERVER_X11_HH_

#include "../server.hh"

#include <xcb/xcb.h>
#include <xcb/xcb_errors.h>

class ServerX11 : public Server {
public:
    ServerX11(Engine& engine, xcb_connection_t* dpy, xcb_screen_t* scr, xcb_errors_context_t* err_ctx)
        : Server(engine), dpy_(dpy), scr_(scr), err_ctx_(err_ctx) {}

    void run() override;

private:
    xcb_connection_t*     dpy_ = nullptr;
    xcb_screen_t*         scr_ = nullptr;
    xcb_errors_context_t* err_ctx_ = nullptr;

    void setup_event_listeners();
    void capture_existing_windows();
    void run_event_loop();

    void on_error(xcb_request_error_t *e);

    void on_map_request(xcb_window_t window);

    void on_unmap_notify(xcb_unmap_notify_event_t *p_event);
};

#endif //SERVER_X11_HH_
