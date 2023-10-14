#include "root_x11.hh"

RootX11::RootX11(std::optional<std::string> const &display)
    : Root(display)
{
    std::string display_var = getenv("DISPLAY");
    if (display_var.empty())
        display_var = ":0.0";

    dpy_ = xcb_connect(display.value_or(display_var).c_str(), nullptr);
    if (xcb_connection_has_error(dpy_))
        throw std::runtime_error("Could not connect to display " + display.value_or(display_var));
}
