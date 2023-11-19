#include "xgraphics.hh"

#include <cstdlib>

#include "options/options.hh"
#include "util/log.hh"

void XGraphics::init()
{
    const char* display_str = nullptr;
    if (options_->display)
        display_str = options_->display->c_str();
    display = XOpenDisplay(display_str);
    if (!display) {
        error("Could not connect to display %s", display_str);
        exit(EXIT_FAILURE);
    }

    XSync(display, false);

    screen = DefaultScreen(display);
    root = DefaultRootWindow(display);
    visual = DefaultVisual(display, screen);
    gc = DefaultGC(display, screen);
    colormap = DefaultColormap(display, screen);
    depth = DefaultDepth(display, screen);
    black = BlackPixel(display, screen);
    white = WhitePixel(display, screen);

    debug("X11 screen initialized");

    if (options_->debug_mode)
        XSynchronize(display, True);

    resources.init();
}

XGraphics::~XGraphics()
{
    if (display) {
        XCloseDisplay(display);
    }
    debug("X11 screen destroyed");
}

void XGraphics::subscribe_to_wm_events()
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
}

std::optional<Event> XGraphics::next_event()
{
    XEvent e;
    XNextEvent(display, &e);

    switch (e.type) {
        case MapRequest:
            return WindowAdded { e.xmaprequest.window };
        case UnmapNotify:
            return WindowRemoved { e.xunmap.window };
    }

    return {};
}

Size XGraphics::screen_size() const
{
    return { DisplayWidth(display, screen), DisplayHeight(display, screen) };
}

Rectangle XGraphics::get_window_rectangle(WindowHandle window) const
{
    Window root;
    int requested_x, requested_y;
    unsigned int child_w, child_h, border, depth;
    XGetGeometry(display, window, &root, &requested_x, &requested_y, &child_w, &child_h, &border, &depth);
    return { requested_x, requested_y, (int) child_w, (int) child_h };
}

WindowHandle XGraphics::create_window(Rectangle const &rectangle)
{
    WindowHandle handle = XCreateWindow(display, root, rectangle.x, rectangle.y, rectangle.w, rectangle.h, 0,
                                        CopyFromParent, InputOutput, CopyFromParent, 0, nullptr);

    XSelectInput(display, handle, SubstructureNotifyMask | StructureNotifyMask | ExposureMask |
                                  ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

    window_info_.emplace(handle, WindowInfo {
        .backbuffer = XCreatePixmap(display, handle, rectangle.w, rectangle.h, depth),
        .gc = XCreateGC(display, handle, 0, nullptr),
        // .xft_draw = XftDrawCreate(display, id_, visual, colormap);
    });

    XMapWindow(display, handle);

    // TODO - define arrow cursor

    debug("Window %d created", handle);
    return handle;
}

void XGraphics::destroy_window(WindowHandle window)
{
    XUnmapWindow(display, window);

    WindowInfo const& info = window_info_.at(window);
    // XftDrawDestroy(xft_draw_);
    XFreeGC(display, info.gc);
    XFreePixmap(display, info.backbuffer);

    XDestroyWindow(display, window);
    window_info_.erase(window);

    debug("Window %d destroyed", window);
}

void XGraphics::reparent_window(WindowHandle parent, WindowHandle child, Point const &offset)
{
    XAddToSaveSet(display, child);
    XReparentWindow(display, child, parent, offset.x, offset.y);
    XMapWindow(display, child);

    debug("Window %d reparented into %d", child, parent);
}

void XGraphics::unparent_window(WindowHandle child)
{
    XUnmapWindow(display, child);
    XRemoveFromSaveSet(display, child);
    XReparentWindow(display, child, root, 0, 0);

    debug("Window %d unparented", child);
}