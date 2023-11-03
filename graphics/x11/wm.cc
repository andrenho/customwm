#include "wm.hh"

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <cstdio>
#include <cstdlib>

#include <utility>

#include "x11.hh"
#include "theme/logger.hh"
#include "xwindow.hh"

void WM::run()
{
    setup_event_filter();
    add_existing_windows();
    theme.call_opt("wm.after_start");
    main_loop();
}

/*
void WMX11::initialize_xcomposite()
{
    int ev_base, err_base;
    if (XCompositeQueryExtension(x11.display, &ev_base, &err_base)) {
        int major = 0, minor = 2;
        XCompositeQueryVersion(x11.display, &major, &minor);
        if (major == 0 && minor < 2)
            throw std::runtime_error("Composite version not supported (expected >= 0.2, found " +
                                     std::to_string(major) + "." + std::to_string(minor) + ")");
    }

    for (int i = 0; i < ScreenCount(x11.display); ++i)
        XCompositeRedirectSubwindows(x11.display, RootWindow(x11.display, i), CompositeRedirectAutomatic);

    LOG.debug("Composite initialized.");
}
 */

void WM::setup_event_filter()
{
    XSetErrorHandler(&WM::on_error);

    XSelectInput(x11.display, x11.root, SubstructureRedirectMask | SubstructureNotifyMask | PointerMotionMask);
    XSync(x11.display, false);
}

void WM::add_existing_windows()
{
    Window root, parent;
    Window* window_list;
    unsigned int n_windows;

    XQueryTree(x11.display, x11.root, &root, &parent, &window_list, &n_windows);
    for (size_t i = 0; i < n_windows; ++i)
        on_map_request(window_list[i]);

    free(window_list);
}

[[noreturn]] void WM::main_loop()
{
    for (;;) {
        XEvent e;
        XNextEvent(x11.display, &e);

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

void WM::move_window_with_mouse(bool move, std::optional<L_Window*> window)
{
    if (move && window) {
        moving_window_with_mouse_ = (XWindow *) window.value();
    } else {
        moving_window_with_mouse_ = {};
    }
}

int WM::on_error(Display* dsp, XErrorEvent* e)
{
    if (e->error_code == BadAccess && e->request_code == 2 && e->minor_code == 0)
        throw std::runtime_error("There's already another window manager running!\n");

    char buf[1024];
    XGetErrorText(dsp, e->error_code, buf, sizeof buf);
    LOG.info("XLib error: (request code %d, minor code %d) %s", e->request_code, e->minor_code, buf);

    return 0;
}

void WM::on_map_request(Window child_id)
{
    // figure out information about the child window
    Window root;
    int requested_x, requested_y;
    unsigned int child_w, child_h, border, depth;
    XGetGeometry(x11.display, child_id, &root, &requested_x, &requested_y, &child_w, &child_h, &border, &depth);

    // figure out information about the parent window (TODO)
    Rectangle child_rect = { requested_x, requested_y, child_w, child_h };
    Size screen_size = { (uint32_t) DisplayWidth(x11.display, x11.screen), (uint32_t) DisplayHeight(x11.display, x11.screen) };
    auto [parent_rect, offset] = theme.get_prop<WindowStartingLocation>("wm.window_starting_location", child_rect, screen_size);

    // create window
    Window parent_id = XCreateWindow(x11.display, root, parent_rect.x, parent_rect.y, parent_rect.w, parent_rect.h, 0,
                                     CopyFromParent, InputOutput, CopyFromParent, 0, nullptr);
    LOG.debug("Created parent window id %d", parent_id);
    XSelectInput(x11.display, parent_id, SubstructureNotifyMask | StructureNotifyMask | ExposureMask |
                                  ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

    // set cursor
    Cursor c = XCreateFontCursor(x11.display, XC_left_ptr);  // TODO - free cursor
    XDefineCursor(x11.display, parent_id, c);

    // manage child
    XAddToSaveSet(x11.display, child_id);
    XReparentWindow(x11.display, child_id, parent_id, offset.x, offset.y);

    // map windows
    XMapWindow(x11.display, parent_id);
    XMapWindow(x11.display, child_id);
    XFlush(x11.display);

    auto window = std::make_unique<XWindow>(resources_, parent_id, child_id, parent_rect);
    theme.call_opt("wm.after_window_registered", window.get());
    windows_.emplace(parent_id, std::move(window));

    LOG.info("Reparented window %d (parent %d)", child_id, parent_id);
}

void WM::on_unmap_notify(XUnmapEvent const &e)
{
    auto it = windows_.find(e.window);
    if (it != windows_.end()) {

        // parent is unmapped
        Window parent_id = it->first;
        windows_.erase(parent_id);
        XDestroyWindow(x11.display, parent_id);
        XFlush(x11.display);

        LOG.debug("Destroyed parent window id %d", parent_id);

    } else {
        for (auto& kv: windows_) {
            XWindow* window = kv.second.get();

            // child is unmapped
            if (window->child_id == e.window) {
                XReparentWindow(x11.display, window->child_id, x11.root, 0, 0);
                XRemoveFromSaveSet(x11.display, window->child_id);
                XDestroyWindow(x11.display, window->child_id);
                XUnmapWindow(x11.display, window->parent_id);
                XFlush(x11.display);

                theme.call_opt("wm.after_window_unregistered", &it->second);
            }
        }
    }
}


void WM::on_expose(XExposeEvent const &e)
{
    auto it = windows_.find(e.window);
    if (it != windows_.end()) {
        theme.call_opt("wm.on_expose", (XWindow *) it->second.get(), Rectangle {e.x, e.y, (uint32_t) e.width, (uint32_t) e.height });
        XFlush(x11.display);
    }
}

void WM::on_click(XButtonEvent const &e)
{
    auto it = windows_.find(e.window);
    if (it != windows_.end()) {

        // create event
        XWindow* window = it->second.get();
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
        theme.call_opt("wm.on_click", window, click_event);

        // on hotspot click
        for (auto [hotspot, rect]: theme.get_prop<std::map<std::string, Rectangle>>("wm.hotspots", window)) {
            if (rect.contains(click_event.pos))
                theme.call_opt("wm.on_hotspot_click", window, hotspot, click_event);
        }
    }
}

void WM::on_move(XMotionEvent const &e)
{
    if (moving_window_with_mouse_.has_value()) {
        XWindowAttributes xwa;
        XGetWindowAttributes(x11.display, (*moving_window_with_mouse_)->parent_id, &xwa);
        int x = xwa.x + e.x_root - last_mouse_position_.x;
        int y = xwa.y + e.y_root - last_mouse_position_.y;
        XMoveWindow(x11.display, (*moving_window_with_mouse_)->parent_id, x, y);
    }
    last_mouse_position_ = { e.x_root, e.y_root };

    std::optional<XWindow*> window {};
    auto it = windows_.find(e.window);
    if (it != windows_.end())
        window = it->second.get();
    theme.call_opt("wm.on_mouse_move", window, Point { e.x, e.y });
}

void WM::on_configure(XConfigureEvent const &e)
{
    auto it = windows_.find(e.window);
    if (it != windows_.end()) {
        XWindow* window = it->second.get();
        window->rectangle = { e.x, e.y, (uint32_t) e.width, (uint32_t) e.height };
        theme.call_opt("wm.on_configure_window", window);
    }
}
