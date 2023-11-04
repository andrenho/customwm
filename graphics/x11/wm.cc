#include "wm.hh"

#include <X11/Xlib.h>
#include <cstdlib>

#include <utility>

#include "x11.hh"
#include "theme/logger.hh"
#include "xwindow.hh"

WM::WM()
    : child_id_atom(XInternAtom(x11.display, "child_id", false)),
      parent_id_atom(XInternAtom(x11.display, "parent_id", false))
{
}

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
    for (size_t i = 0; i < n_windows; ++i) {
        XEvent ev {
            .xmaprequest = {
                    .type = MapRequest,
                    .serial = 0,
                    .send_event = 0,
                    .display = x11.display,
                    .parent = root,
                    .window = window_list[i],
            }
        };
        XPutBackEvent(x11.display, &ev);
    }

    free(window_list);
}

[[noreturn]] void WM::main_loop()
{
    for (;;) {
        XEvent e;
        XNextEvent(x11.display, &e);

        switch (e.type) {
            case MapRequest:
                LOG.debug("event: MapRequest %d", e.xmaprequest.window);
                on_map_request(e.xmaprequest.window);
                break;
            case UnmapNotify:
                LOG.debug("event: UnmapNotify %d", e.xunmap.window);
                on_unmap_notify(e.xunmap);
                break;
            case Expose:
                LOG.debug("event: Expose %d", e.xexpose.window);
                on_expose(e.xexpose);
                break;
            case ButtonPress:
            case ButtonRelease:
                LOG.debug("event: Button %d", e.xbutton.window);
                on_click(e.xbutton);
                break;
            case MotionNotify:
                on_move(e.xmotion);
                break;
            case ConfigureNotify:
                LOG.debug("event: ConfigureNotify %d", e.xconfigure.window);
                on_configure(e.xconfigure);
                break;
            case CreateNotify:  LOG.debug("event: CreateNotify %d", e.xcreatewindow.window); break;
            case DestroyNotify: LOG.debug("event: DestroyNotify %d", e.xdestroywindow.window); break;
            case MapNotify:     LOG.debug("event: MapNotify %d", e.xmap.window); break;
            case ReparentNotify:LOG.debug("event: ReparentNotify %d", e.xreparent.window); break;
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
    auto parent = std::make_unique<XWindow>(resources_, parent_rect);
    LOG.debug("Created parent window id %d", parent->id);

    // manage child
    XAddToSaveSet(x11.display, child_id);
    XReparentWindow(x11.display, child_id, parent->id, offset.x, offset.y);
    LOG.info("Reparented window %d (parent %d)", child_id, parent->id);

    // map windows
    XMapWindow(x11.display, child_id);
    XFlush(x11.display);
    theme.call_opt("wm.after_window_registered", parent.get());
    auto [wmwindow, _] = windows_.emplace(parent->id, std::move(parent));

    // set properties
    XWindow* xparent = wmwindow->second.get();
    resources_.set_property(xparent->id, "child", child_id);
    resources_.set_property(child_id, "parent", xparent->id);
}

void WM::on_unmap_notify(XUnmapEvent const &e)
{
    XWindow* xwindow = find_parent(e.window);
    if (xwindow) {

        // parent is unmapped
        theme.call_opt("wm.after_window_unregistered", xwindow);
        LOG.debug("Destroyed parent window id %d", xwindow->id);
        windows_.erase(xwindow->id);

    } else {

        // child is unmapped
        for (auto& [parent_id, parent]: windows_) {
            Window child_id = resources_.get_property<Window>(parent_id, "child");
            if (child_id != None) {
                XUnmapWindow(x11.display, parent_id);
                parent->deleted = true;
            }
        }
        XFlush(x11.display);
    }
}


void WM::on_expose(XExposeEvent const &e)
{
    XWindow* xwindow = find_parent(e.window);
    if (xwindow && !xwindow->deleted) {
        theme.call_opt("wm.on_expose", xwindow, Rectangle {e.x, e.y, (uint32_t) e.width, (uint32_t) e.height });
        XFlush(x11.display);
    }
}

void WM::on_click(XButtonEvent const &e)
{
    XWindow* xwindow = find_parent(e.window);
    if (xwindow) {

        // create event
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
        theme.call_opt("wm.on_click", xwindow, click_event);

        // on hotspot click
        for (auto [hotspot, rect]: theme.get_prop<std::map<std::string, Rectangle>>("wm.hotspots", xwindow)) {
            if (rect.contains(click_event.pos))
                theme.call_opt("wm.on_hotspot_click", xwindow, hotspot, click_event);
        }
    }
}

void WM::on_move(XMotionEvent const &e)
{
    // check if moving window
    if (moving_window_with_mouse_.has_value()) {
        XWindowAttributes xwa;
        XGetWindowAttributes(x11.display, (*moving_window_with_mouse_)->id, &xwa);
        int x = xwa.x + e.x_root - last_mouse_position_.x;
        int y = xwa.y + e.y_root - last_mouse_position_.y;
        XMoveWindow(x11.display, (*moving_window_with_mouse_)->id, x, y);
    }

    // check if entering or leaving hotspot
    std::optional<std::string> new_hotspot {};
    XWindow* xwindow = find_parent(e.window);
    if (xwindow) {
        for (auto [hs, rect]: theme.get_prop<std::map<std::string, Rectangle>>("wm.hotspots", xwindow)) {
            if (rect.contains({ e.x, e.y }))
                new_hotspot = hs;
        }
    }
    if (new_hotspot != current_hotspot_) {
        if (current_hotspot_)
            theme.call_opt("wm.on_leave_hotspot", xwindow, *current_hotspot_);
        if (new_hotspot)
            theme.call_opt("wm.on_enter_hotspot", xwindow, *new_hotspot);
        current_hotspot_ = new_hotspot;
    }

    // fire on_mouse_move event on theme
    theme.call_opt("wm.on_mouse_move", xwindow, Point { e.x, e.y });  // xwindow can be null

    last_mouse_position_ = { e.x_root, e.y_root };
}

void WM::on_configure(XConfigureEvent const &e)
{
    XWindow* xwindow = find_parent(e.window);
    if (xwindow) {
        xwindow->rectangle = { e.x, e.y, (uint32_t) e.width, (uint32_t) e.height };
        theme.call_opt("wm.on_configure_window", xwindow);
    }
}

XWindow* WM::find_parent(Window parent_id) const
{
    auto it = windows_.find(parent_id);
    if (it == windows_.end())
        return nullptr;
    else
        return it->second.get();
}