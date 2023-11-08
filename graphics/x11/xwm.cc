#include "xwm.hh"

#include <X11/Xlib.h>
#include <cstdlib>

#include <utility>

#include "xgraphics.hh"
#include "common/logger.hh"
#include "xwindow.hh"


XWindowManager::XWindowManager()
    : WindowManager(std::make_unique<XResources>()),
      xresources_(dynamic_cast<XResources *>(resources_.get()))
{

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

void XWindowManager::setup_event_listener()
{
    XSetErrorHandler(&XWindowManager::on_error);

    XSelectInput(X->display, X->root, SubstructureRedirectMask | SubstructureNotifyMask | PointerMotionMask);
    XSync(X->display, false);
}

void XWindowManager::add_existing_windows()
{
    Window root, parent;
    Window* window_list;
    unsigned int n_windows;

    XQueryTree(X->display, X->root, &root, &parent, &window_list, &n_windows);
    for (size_t i = 0; i < n_windows; ++i)
        on_create_child(window_list[i]);

    free(window_list);
}

std::unique_ptr<LWindow> XWindowManager::create_window(Rectangle const &rectangle) const
{
    return std::make_unique<XWindow>(*xresources_, rectangle);
}

void XWindowManager::parse_next_event()
{
    XEvent e;
    XNextEvent(X->display, &e);

    switch (e.type) {
        case MapRequest:
            LOG.debug("event: MapRequest %d", e.xmaprequest.window);
            on_create_child(e.xmaprequest.window);
            break;
        case UnmapNotify:
            LOG.debug("event: UnmapNotify %d", e.xunmap.window);
            on_destroy_child(e.xunmap.window);
            break;
        case Expose:
            LOG.debug("event: Expose %d", e.xexpose.window);
            on_expose_window(e.xexpose.window,
                 { e.xexpose.x, e.xexpose.y, (uint32_t) e.xexpose.width, (uint32_t) e.xexpose.height });
            break;
        case ButtonPress:
        case ButtonRelease:
        {
            LOG.debug("event: Button %d", e.xbutton.window);
            ClickEvent click_event {
                    .pressed = (e.type == ButtonPress),
                    .pos = Point { e.xbutton.x, e.xbutton.y },
                    .abs_pos = Point { e.xbutton.x_root, e.xbutton.y_root },
            };
            switch (e.xbutton.button) {
                case Button1: click_event.button = ClickEvent::Left; break;
                case Button2: click_event.button = ClickEvent::Middle; break;
                case Button3: click_event.button = ClickEvent::Right; break;
                default: click_event.button = ClickEvent::Other;
            }
            if (e.xmotion.window == None || e.xmotion.window == X->root)
                on_click_desktop(click_event);
            else
                on_click_window(e.xmotion.window, click_event);
            break;
        }
        case MotionNotify:
            if (e.xmotion.window == None || e.xmotion.window == X->root)
                on_move_pointer_desktop({ e.xmotion.x_root, e.xmotion.y_root });
            else
                on_move_pointer_window(e.xmotion.window, { e.xmotion.x, e.xmotion.y });
            break;
        case ConfigureNotify:
            // LOG.debug("event: ConfigureNotify %d", e.xconfigure.window);
            break;
        case CreateNotify:  LOG.debug("event: CreateNotify %d", e.xcreatewindow.window); break;
        case DestroyNotify: LOG.debug("event: DestroyNotify %d", e.xdestroywindow.window); break;
        case MapNotify:     LOG.debug("event: MapNotify %d", e.xmap.window); break;
        case ReparentNotify:LOG.debug("event: ReparentNotify %d", e.xreparent.window); break;
        default:
            LOG.debug("Unmapped event received: %d", e.type);
    }
}

void XWindowManager::move_window_with_mouse(bool move, std::optional<LWindow*> window)
{
    if (move && window) {
        moving_window_with_mouse_ = (XWindow *) window.value();
    } else {
        moving_window_with_mouse_ = {};
    }
}

int XWindowManager::on_error(Display* dsp, XErrorEvent* e)
{
    if (e->error_code == BadAccess && e->request_code == 2 && e->minor_code == 0)
        throw std::runtime_error("There's already another window manager running!\n");

    char buf[1024];
    XGetErrorText(dsp, e->error_code, buf, sizeof buf);
    LOG.info("XLib error: (request code %d, minor code %d) %s", e->request_code, e->minor_code, buf);

    return 0;
}

Rectangle XWindowManager::get_window_rectangle(WHandle window) const
{
    Window root;
    int requested_x, requested_y;
    unsigned int child_w, child_h, border, depth;
    XGetGeometry(X->display, window, &root, &requested_x, &requested_y, &child_w, &child_h, &border, &depth);
    return { requested_x, requested_y, child_w, child_h };
}

Size XWindowManager::get_screen_size() const
{
    return {
        (uint32_t) DisplayWidth(X->display, X->screen),
        (uint32_t) DisplayHeight(X->display, X->screen)
    };
}


/*
void WM_X11::on_map_request(Window child_id)
{
    // figure out information about the child window
    Window root;
    int requested_x, requested_y;
    unsigned int child_w, child_h, border, depth;
    XGetGeometry(X->display, child_id, &root, &requested_x, &requested_y, &child_w, &child_h, &border, &depth);

    // figure out information about the parent window (TODO)
    Rectangle child_rect = { requested_x, requested_y, child_w, child_h };
    Size screen_size =
    auto [parent_rect, offset] = THEME.get_prop<WindowStartingLocation>("wm.window_starting_location", child_rect, screen_size);

    // create window
    auto parent = std::make_unique<XWindow>(resources_, parent_rect);
    LOG.debug("Created parent window id %d", parent->id);

    // manage child
    XAddToSaveSet(X->display, child_id);
    XReparentWindow(X->display, child_id, parent->id, offset.x, offset.y);
    LOG.info("Reparented window %d (parent %d)", child_id, parent->id);

    // map windows
    XMapWindow(X->display, child_id);
    XFlush(X->display);
    THEME.call_opt("wm.after_window_registered", parent.get());
    auto [wmwindow, _] = windows_.emplace(parent->id, std::move(parent));

    // set properties
    XWindow* xparent = wmwindow->second.get();
    resources_.set_property(xparent->id, "child", child_id);
    resources_.set_property(child_id, "parent", xparent->id);
}

void WM_X11::on_unmap_notify(XUnmapEvent const &e)
{
    XWindow* xwindow = find_parent(e.window);
    if (xwindow) {

        // parent is unmapped
        THEME.call_opt("wm.after_window_unregistered", xwindow);
        LOG.debug("Destroyed parent window id %d", xwindow->id);
        windows_.erase(xwindow->id);

    } else {

        // child is unmapped
        for (auto& [parent_id, parent]: windows_) {
            Window child_id = resources_.get_property<Window>(parent_id, "child");
            if (child_id != None) {
                XUnmapWindow(X->display, parent_id);
                parent->deleted = true;
            }
        }
        XFlush(X->display);
    }
}


void WM_X11::on_expose(XExposeEvent const &e)
{
    XWindow* xwindow = find_parent(e.window);
    if (xwindow && !xwindow->deleted) {
        THEME.call_opt("wm.on_expose", xwindow, Rectangle {e.x, e.y, (uint32_t) e.width, (uint32_t) e.height });
        XFlush(X->display);
    }
}

void WM_X11::on_click(XButtonEvent const &e)
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
        THEME.call_opt("wm.on_click", xwindow, click_event);

        // on hotspot click
        for (auto [hotspot, rect]: THEME.get_prop<std::map<std::string, Rectangle>>("wm.hotspots", xwindow)) {
            if (rect.contains(click_event.pos))
                THEME.call_opt("wm.on_hotspot_click", xwindow, hotspot, click_event);
        }
    }
}

void WM_X11::on_move(XMotionEvent const &e)
{
    // check if moving window
    if (moving_window_with_mouse_.has_value()) {
        XWindowAttributes xwa;
        XGetWindowAttributes(X->display, (*moving_window_with_mouse_)->id, &xwa);
        int x = xwa.x + e.x_root - last_mouse_position_.x;
        int y = xwa.y + e.y_root - last_mouse_position_.y;
        XMoveWindow(X->display, (*moving_window_with_mouse_)->id, x, y);
    }

    // check if entering or leaving hotspot
    std::optional<std::string> new_hotspot {};
    XWindow* xwindow = find_parent(e.window);
    if (xwindow) {
        for (auto [hs, rect]: THEME.get_prop<std::map<std::string, Rectangle>>("wm.hotspots", xwindow)) {
            if (rect.contains({ e.x, e.y }))
                new_hotspot = hs;
        }
    }
    if (new_hotspot != current_hotspot_) {
        if (current_hotspot_)
            THEME.call_opt("wm.on_leave_hotspot", xwindow, *current_hotspot_);
        if (new_hotspot)
            THEME.call_opt("wm.on_enter_hotspot", xwindow, *new_hotspot);
        current_hotspot_ = new_hotspot;
    }

    // fire on_mouse_move event on theme
    THEME.call_opt("wm.on_mouse_move", xwindow, Point {e.x, e.y });  // xwindow can be null

    last_mouse_position_ = { e.x_root, e.y_root };
}

void WM_X11::on_configure(XConfigureEvent const &e)
{
    XWindow* xwindow = find_parent(e.window);
    if (xwindow) {
        xwindow->rectangle = { e.x, e.y, (uint32_t) e.width, (uint32_t) e.height };
        THEME.call_opt("wm.on_configure_window", xwindow);
    }
}
 */

XWindow* XWindowManager::find_parent(Window parent_id) const
{
    auto it = windows_.find(parent_id);
    if (it == windows_.end())
        return nullptr;
    else
        return it->second.get();
}

void XWindowManager::set_focus(std::optional<LWindow *> window)
{
    (void) window;  // TODO
}

