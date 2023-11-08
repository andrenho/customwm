#include "x11.hh"
#include "util/logger.hh"

X11 x11 {};

void X11::init(std::optional<std::string> const& display)
{
    std::string sdisplay = display.value_or(getenv("DISPLAY"));
    const char* cdisplay = sdisplay.empty() ? nullptr : sdisplay.c_str();
    x11.display = XOpenDisplay(cdisplay);
    if (!x11.display)
        throw std::runtime_error("Failed to open display.");
    LOG.debug("Connected to display %p.", x11.display);

    x11.screen = DefaultScreen(x11.display);
    x11.root = DefaultRootWindow(x11.display);
    x11.visual = DefaultVisual(x11.display, x11.screen);
    x11.gc = DefaultGC(x11.display, x11.screen);
    x11.colormap = DefaultColormap(x11.display, x11.screen);
    x11.depth = DefaultDepth(x11.display, x11.screen);
    x11.black = BlackPixel(x11.display, x11.screen);
    x11.white = WhitePixel(x11.display, x11.screen);
}

X11::~X11()
{
    XCloseDisplay(x11.display);
}
