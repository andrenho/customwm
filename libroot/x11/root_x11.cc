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

    xcb_errors_context_new(dpy_, &err_ctx);

    scr_ = xcb_setup_roots_iterator(xcb_get_setup(dpy_)).data;
}

void RootX11::setup_event_listeners(EventListener &event_listener)
{
    Root::setup_event_listeners(event_listener);
}

void RootX11::capture_existing_windows()
{

}

void RootX11::run_event_loop()
{

}

