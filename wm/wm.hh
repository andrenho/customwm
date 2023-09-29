#ifndef WM_HH_
#define WM_HH_

#include "config.hh"

#include <xcb/xcb.h>

class WM {
public:
    explicit WM(Config const& config) : config(config) {}

    WM& start();
    void run();

private:
    Config const& config;

    xcb_connection_t* dpy = nullptr;
    xcb_screen_t* scr = nullptr;

    int handle_event();

    void log(std::string const& event_type, uint32_t window_id, const char* fmt, ...) const;
    void log(std::string const& event_type, uint32_t window_id) const { log(event_type, window_id, ""); }

    void on_create_notify(xcb_create_notify_event_t const& e);
    void on_configure_request(xcb_configure_request_event_t const& e);
    void on_map_request(xcb_map_request_event_t const& e);
    void on_button_press(xcb_button_press_event_t const& e);
};

#endif //WM_HH_
