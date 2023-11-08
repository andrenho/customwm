#include "x.hh"
#include "common/logger.hh"
#include "xwm.hh"

XGraphics* X = nullptr;

XGraphics::XGraphics(std::optional<std::string> const &display_name)
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

XGraphics::~XGraphics()
{
    XCloseDisplay(display);
}

std::unique_ptr<WindowManager> XGraphics::create_wm()
{
    return std::make_unique<XWindowManager>();
}

