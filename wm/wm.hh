#ifndef WM_HH_
#define WM_HH_

#include <xcb/xcb.h>

#include "../lib/customwmlib.hh"
#include "windows.hh"

class WM {
public:
    explicit WM(CustomWMLib const& lib) : lib_(lib) {}

    void run();

private:
    CustomWMLib const& lib_;
    xcb_connection_t* dpy = nullptr;
    xcb_screen_t* scr = nullptr;
    Windows windows_;

    void connect_to_X();

    void listen_to_events();

    void main_loop();

    void map_request(xcb_map_request_event_t const &event);

    void unmap_notify(xcb_unmap_notify_event_t const &e);
};

#endif //WM_HH_
