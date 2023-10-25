#include "graphicsx11.hh"
#include "wmx11.hh"

#include "theme/logger.hh"

GraphicsX11::GraphicsX11(Theme& theme, std::optional<std::string> const& display)
    : Graphics(theme)
{
    std::string sdisplay = display.value_or(getenv("DISPLAY"));
    const char* cdisplay = sdisplay.empty() ? nullptr : sdisplay.c_str();
    dpy_ = XOpenDisplay(cdisplay);
    if (!dpy_)
        throw std::runtime_error("Failed to open display.");
    LOG.debug("Connected to display %p.", dpy_);
}

GraphicsX11::~GraphicsX11()
{
    XCloseDisplay(dpy_);
}

std::unique_ptr<WM> GraphicsX11::create_wm_()
{
    return std::make_unique<WMX11>(theme_, dpy_);
}

