#include "wmx11.hh"
#include "theme/logger.hh"
#include "theme/theme.hh"
#include "theme/types/types.hh"
#include "wmwindow.hh"

#include <X11/Xlib.h>
#include <cstdio>
#include <cstdlib>

#include <utility>

WMX11::WMX11(Theme& theme, Display* dpy)
    : theme_(theme), dpy_(dpy)
{
    root_ = DefaultRootWindow(dpy_);
}

void WMX11::run()
{
    setup_event_filter();
    add_existing_windows();
    theme_.call_opt("wm.after_start");
    main_loop();
}

/*
void WMX11::initialize_xcomposite()
{
    int ev_base, err_base;
    if (XCompositeQueryExtension(dpy_, &ev_base, &err_base)) {
        int major = 0, minor = 2;
        XCompositeQueryVersion(dpy_, &major, &minor);
        if (major == 0 && minor < 2)
            throw std::runtime_error("Composite version not supported (expected >= 0.2, found " +
                                     std::to_string(major) + "." + std::to_string(minor) + ")");
    }

    for (int i = 0; i < ScreenCount(dpy_); ++i)
        XCompositeRedirectSubwindows(dpy_, RootWindow(dpy_, i), CompositeRedirectAutomatic);

    LOG.debug("Composite initialized.");
}
 */

void WMX11::setup_event_filter()
{
    XSetErrorHandler(&WMX11::on_error);

    XSelectInput(dpy_, root_, SubstructureRedirectMask | SubstructureNotifyMask);
    XSync(dpy_, false);
}

void WMX11::add_existing_windows()
{
    Window root, parent;
    Window* window_list;
    unsigned int n_windows;

    XQueryTree(dpy_, root_, &root, &parent, &window_list, &n_windows);
    for (size_t i = 0; i < n_windows; ++i)
        on_map_request(window_list[i]);

    free(window_list);
}

[[noreturn]] void WMX11::main_loop()
{
    for (;;) {
        XEvent e;
        XNextEvent(dpy_, &e);

        switch (e.type) {
            case MapRequest:
                on_map_request(e.xmaprequest.window);
                break;
            case UnmapNotify:
                on_unmap_notify(e.xunmap);
                break;
            case Expose:
                on_expose(e.xexpose);
                break;
            default:
                LOG.debug("Unmapped event received: %d", e.type);
        }
    }
}

int WMX11::on_error(Display* dsp, XErrorEvent* e)
{
    if (e->error_code == BadAccess && e->request_code == 2 && e->minor_code == 0)
        throw std::runtime_error("There's already another window manager running!\n");

    char buf[1024];
    XGetErrorText(dsp, e->error_code, buf, sizeof buf);
    LOG.info("XLib error: (request code %d, minor code %d) %s", e->request_code, e->minor_code, buf);

    return 0;
}

void WMX11::on_map_request(Window child_id)
{
    // figure out information about the child window
    Window root;
    int requested_x, requested_y;
    unsigned int child_w, child_h, border, depth;
    XGetGeometry(dpy_, child_id, &root, &requested_x, &requested_y, &child_w, &child_h, &border, &depth);

    // figure out information about the parent window (TODO)
    int snum = DefaultScreen(dpy_);
    Rectangle child_rect = { requested_x, requested_y, child_w, child_h };
    Size screen_size = { (uint32_t) DisplayWidth(dpy_, snum), (uint32_t) DisplayHeight(dpy_, snum) };
    auto [parent_rect, offset] = theme_.get_prop<WindowStartingLocation>("wm.window_starting_location", child_rect, screen_size);

    // create window
    Window parent_id = XCreateWindow(dpy_, root, parent_rect.x, parent_rect.y, parent_rect.w, parent_rect.h, 0,
                                     CopyFromParent, InputOutput, CopyFromParent, 0, nullptr);
    LOG.debug("Created parent window id %d", parent_id);
    XSelectInput(dpy_, parent_id, SubstructureNotifyMask | StructureNotifyMask | ExposureMask);

    // manage child
    XAddToSaveSet(dpy_, child_id);
    XReparentWindow(dpy_, child_id, parent_id, offset.x, offset.y);

    // map windows
    XMapWindow(dpy_, parent_id);
    XMapWindow(dpy_, child_id);
    XFlush(dpy_);

    auto window = std::make_unique<WM_Window>(dpy_, parent_id, child_id, parent_rect);
    theme_.call_opt("wm.after_window_registered", window.get());
    windows_.emplace(parent_id, std::move(window));

    LOG.info("Reparented window %d (parent %d)", child_id, parent_id);
}

void WMX11::on_unmap_notify(XUnmapEvent const &e)
{
    auto it = windows_.find(e.window);
    if (it != windows_.end()) {

        // parent is unmapped
        Window parent_id = it->first;
        windows_.erase(parent_id);
        XDestroyWindow(dpy_, parent_id);
        XFlush(dpy_);

        LOG.debug("Destroyed parent window id %d", parent_id);

    } else {
        for (auto& kv: windows_) {
            WM_Window* window = kv.second.get();

            // child is unmapped
            if (window->child_id == e.window) {
                XReparentWindow(dpy_, window->child_id, root_, 0, 0);
                XRemoveFromSaveSet(dpy_, window->child_id);
                XDestroyWindow(dpy_, window->child_id);
                XUnmapWindow(dpy_, window->parent_id);
                XFlush(dpy_);

                theme_.call_opt("wm.after_window_unregistered", &it->second);
            }
        }
    }
}

void WMX11::on_expose(XExposeEvent const &e)
{
    auto it = windows_.find(e.window);
    if (it != windows_.end())
        theme_.call_opt("wm.on_expose", (WM_Window *) it->second.get(), Rectangle { e.x, e.y, (uint32_t) e.width, (uint32_t) e.height });
}