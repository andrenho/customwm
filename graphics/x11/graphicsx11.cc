#include "graphicsx11.hh"
#include "wmx11.hh"

GraphicsX11::GraphicsX11(std::optional<std::string> display)
{
    std::string sdisplay = display.value_or(getenv("DISPLAY"));
    const char* cdisplay = sdisplay.empty() ? nullptr : sdisplay.c_str();
    dsp_ = XOpenDisplay(cdisplay);
}

GraphicsX11::~GraphicsX11()
{
    XCloseDisplay(dsp_);
}

std::unique_ptr<WM> GraphicsX11::create_wm_()
{
    return std::make_unique<WMX11>();
}

