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

void GraphicsX11::listen_to_wm_events()
{
    XSetErrorHandler([](Display* dsp, XErrorEvent* e) -> int {
        if (e->error_code == BadAccess && e->request_code == 2 && e->minor_code == 0) {
            error("There's already another window manager running!");
            exit(EXIT_FAILURE);
        }

        char buf[1024];
        XGetErrorText(dsp, e->error_code, buf, sizeof buf);
        info("XLib error: (resource %d, request code %d, minor code %d) %s", e->resourceid, e->request_code, e->minor_code, buf);

        return 0;
    });

    XSelectInput(display, root, SubstructureRedirectMask | SubstructureNotifyMask | PointerMotionMask |
                                      ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask);
    XSetInputFocus(display, root, RevertToNone, CurrentTime);
    XGrabKeyboard(display, root, true, GrabModeAsync, GrabModeAsync, CurrentTime);

    XSync(display, false);
}
