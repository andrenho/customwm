#include "wm.hh"

#include <stdexcept>

WM& WM::start()
{
    dpy = xcb_connect(config.display_name.c_str(), nullptr);
    if (xcb_connection_has_error(dpy))
        throw std::runtime_error("Could not connect to display " + config.display_name);

    scr = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;

    return *this;
}

void WM::run()
{

}

