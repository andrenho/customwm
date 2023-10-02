#ifndef WM_X11_HH_
#define WM_X11_HH_

#include <xcb/xcb.h>

#include "wm/wm.hh"

class WM_X11 : public WM {
public:
    explicit WM_X11(CustomWMLib const& lib) : WM(lib) {}

protected:
    void setup() override;
    void loop() override;

private:
    void connect_to_X();
    void listen_to_events();

    void map_request(xcb_map_request_event_t const& e);
    void unmap_notify(xcb_unmap_notify_event_t const& e);
    void expose(xcb_expose_event_t const& e);

    xcb_connection_t* dpy = nullptr;
    xcb_screen_t* scr = nullptr;

};

#endif //WM_X11_HH_
