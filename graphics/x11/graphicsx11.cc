#include "graphicsx11.hh"

#include <cstdlib>

#include "options/options.hh"
#include "util/log.hh"

void GraphicsX11::init()
{
    const char* display_str = nullptr;
    if (options_->display)
        display_str = options_->display->c_str();
    display = XOpenDisplay(display_str);
    if (!display) {
        error("Could not connect to display %s", display_str);
        exit(EXIT_FAILURE);
    }

    screen = DefaultScreen(display);
    root = DefaultRootWindow(display);
    visual = DefaultVisual(display, screen);
    gc = DefaultGC(display, screen);
    colormap = DefaultColormap(display, screen);
    depth = DefaultDepth(display, screen);
    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);
}

GraphicsX11::~GraphicsX11()
{
    if (display) {
        XCloseDisplay(display);
    }
}
