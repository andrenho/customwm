#include "wmx11.hh"
#include "theme/logger.hh"
#include "theme/theme.hh"
#include "theme/types/types.hh"
#include "windowx11.hh"

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <cstdio>
#include <cstdlib>

#include <utility>

WMX11::WMX11(Theme& theme, Display* dpy, ResourcesX11& resources)
    : theme_(theme), dpy_(dpy), resources_(resources)
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

    XSelectInput(dpy_, root_, SubstructureRedirectMask | SubstructureNotifyMask | PointerMotionMask);
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
            case ButtonPress:
            case ButtonRelease:
                on_click(e.xbutton);
                break;
            case MotionNotify:
                on_move(e.xmotion);
                break;
            case ConfigureNotify:
                on_configure(e.xconfigure);
                break;
            case CreateNotify:
            case DestroyNotify:
            case MapNotify:
            case ReparentNotify:
                break;
            default:
                LOG.debug("Unmapped event received: %d", e.type);
        }
    }
}

void WMX11::move_window_with_mouse(bool move, std::optional<L_Window*> window)
{
    if (move && window) {
        moving_window_with_mouse_ = (WindowX11 *) window.value();
    } else {
        moving_window_with_mouse_ = {};
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
    XSelectInput(dpy_, parent_id, SubstructureNotifyMask | StructureNotifyMask | ExposureMask |
                                  ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

    // set cursor
    Cursor c = XCreateFontCursor(dpy_, XC_left_ptr);  // TODO - free cursor
    XDefineCursor(dpy_, parent_id, c);

    // manage child
    XAddToSaveSet(dpy_, child_id);
    XReparentWindow(dpy_, child_id, parent_id, offset.x, offset.y);

    // map windows
    XMapWindow(dpy_, parent_id);
    XMapWindow(dpy_, child_id);
    XFlush(dpy_);

    auto window = std::make_unique<WindowX11>(dpy_, resources_, parent_id, child_id, parent_rect);
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
            WindowX11* window = kv.second.get();

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
    if (it != windows_.end()) {
        theme_.call_opt("wm.on_expose", (WindowX11 *) it->second.get(), Rectangle {e.x, e.y, (uint32_t) e.width, (uint32_t) e.height });
        XFlush(dpy_);
    }
}

void WMX11::on_click(XButtonEvent const &e)
{
    auto it = windows_.find(e.window);
    if (it != windows_.end()) {

        // create event
        WindowX11* window = it->second.get();
        ClickEvent click_event {
            .pressed = (e.type == ButtonPress),
            .pos = Point { e.x, e.y },
            .abs_pos = Point { e.x_root, e.y_root },
        };
        switch (e.button) {
            case Button1: click_event.button = ClickEvent::Left; break;
            case Button2: click_event.button = ClickEvent::Middle; break;
            case Button3: click_event.button = ClickEvent::Right; break;
            default: click_event.button = ClickEvent::Other;
        }

        // on click
        theme_.call_opt("wm.on_click", window, click_event);

        // on hotspot click
        for (auto [hotspot, rect]: theme_.get_prop<std::map<std::string, Rectangle>>("wm.hotspots", window)) {
            if (rect.contains(click_event.pos))
                theme_.call_opt("wm.on_hotspot_click", window, hotspot, click_event);
        }
    }
}

void WMX11::on_move(XMotionEvent const &e)
{
    if (moving_window_with_mouse_.has_value()) {
        XWindowAttributes xwa;
        XGetWindowAttributes(dpy_, (*moving_window_with_mouse_)->parent_id, &xwa);
        int x = xwa.x + e.x_root - last_mouse_position_.x;
        int y = xwa.y + e.y_root - last_mouse_position_.y;
        XMoveWindow(dpy_, (*moving_window_with_mouse_)->parent_id, x, y);
    }
    last_mouse_position_ = { e.x_root, e.y_root };

    std::optional<WindowX11*> window {};
    auto it = windows_.find(e.window);
    if (it != windows_.end())
        window = it->second.get();
    theme_.call_opt("wm.on_mouse_move", window, Point { e.x, e.y });
}

void WMX11::on_configure(XConfigureEvent const &e)
{
    auto it = windows_.find(e.window);
    if (it != windows_.end()) {
        WindowX11* window = it->second.get();
        window->rectangle = { e.x, e.y, (uint32_t) e.width, (uint32_t) e.height };
        theme_.call_opt("wm.on_configure_window", window);
    }
}
