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

    xcb_connection_t* dpy;
    xcb_screen_t* scr;
};

#endif //WM_HH_
