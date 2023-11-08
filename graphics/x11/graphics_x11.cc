#include "graphics_x11.hh"
#include "common/logger.hh"
#include "wm_x11.hh"

Graphics_X11* X = nullptr;

Graphics_X11::Graphics_X11(std::optional<std::string> const &display_name)
{
    std::string sdisplay = display_name.value_or(getenv("DISPLAY"));
    const char* cdisplay = sdisplay.empty() ? nullptr : sdisplay.c_str();
    display = XOpenDisplay(cdisplay);
    if (!display)
        throw std::runtime_error("Failed to open display.");
    LOG.debug("Connected to display %p.", display);

    screen = DefaultScreen(display);
    root = DefaultRootWindow(display);
    visual = DefaultVisual(display, screen);
    gc = DefaultGC(display, screen);
    colormap = DefaultColormap(display, screen);
    depth = DefaultDepth(display, screen);
    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);

}

Graphics_X11::~Graphics_X11()
{
    XCloseDisplay(display);
}

std::unique_ptr<WM> Graphics_X11::create_wm()
{
    return std::make_unique<WM_X11>();
}

