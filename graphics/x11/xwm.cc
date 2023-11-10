#include "xwm.hh"

#include <X11/Xlib.h>
#include <cstdlib>

#include <utility>

#include "x.hh"
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

    XSelectInput(X->display, X->root, SubstructureRedirectMask | SubstructureNotifyMask | PointerMotionMask |
            ButtonPressMask | ButtonReleaseMask);
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
    return std::make_unique<XWindow>(*this, *xresources_, rectangle);
}

void XWindowManager::parse_next_event()
{
    XEvent e;
    XNextEvent(X->display, &e);

    switch (e.type) {
        case MapRequest:
            // LOG.debug("event: MapRequest %d", e.xmaprequest.window);
            on_create_child(e.xmaprequest.window);
            break;
        case UnmapNotify:
            // LOG.debug("event: UnmapNotify %d", e.xunmap.window);
            on_destroy_child(e.xunmap.window);
            break;
        case Expose:
            // LOG.debug("event: Expose %d", e.xexpose.window);
            on_window_expose(e.xexpose.window,
                             {e.xexpose.x, e.xexpose.y, (uint32_t) e.xexpose.width, (uint32_t) e.xexpose.height});
            break;
        case ButtonPress:
        case ButtonRelease:
        {
            // LOG.debug("event: Button %d", e.xbutton.window);
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
                on_desktop_click(click_event);
            else
                on_window_click(e.xmotion.window, click_event);
            break;
        }
        case MotionNotify:
            if (e.xmotion.window == None || e.xmotion.window == X->root)
                on_desktop_move_pointer({e.xmotion.x_root, e.xmotion.y_root});
            else
                on_window_move_pointer(e.xmotion.window, {e.xmotion.x, e.xmotion.y});
            break;
        case ConfigureNotify:
            // LOG.debug("event: ConfigureNotify %d", e.xconfigure.window);
            break;
        case CreateNotify:   // LOG.debug("event: CreateNotify %d", e.xcreatewindow.window); break;
        case DestroyNotify:  // LOG.debug("event: DestroyNotify %d", e.xdestroywindow.window); break;
        case MapNotify:      // LOG.debug("event: MapNotify %d", e.xmap.window); break;
        case ReparentNotify: // LOG.debug("event: ReparentNotify %d", e.xreparent.window); break;
            break;
        default:
            LOG.debug("Unmapped event received: %d", e.type);
    }

    XFlush(X->display);
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

void XWindowManager::reparent_window(WHandle parent_id, WHandle child_id, Point const &offset)
{
    XAddToSaveSet(X->display, child_id);
    XReparentWindow(X->display, child_id, parent_id, offset.x, offset.y);
    XMapWindow(X->display, child_id);
}


/*

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

 */

void XWindowManager::expose(LWindow* window)
{
    XEvent ev {
        .xexpose {
                .type = Expose,
                .serial = 0,
                .send_event = true,
                .display = X->display,
                .window = window->id(),
                .x = window->rect().x,
                .y = window->rect().y,
                .width = static_cast<int>(window->rect().w),
                .height = static_cast<int>(window->rect().h),
                .count = 0,
        }
    };
    XSendEvent(X->display, window->id(), false, ExposureMask, &ev);
}
