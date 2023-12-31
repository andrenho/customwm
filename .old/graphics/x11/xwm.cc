#include "xwm.hh"

#include <X11/Xlib.h>
#include <cstdlib>

#include "x.hh"
#include ".old/common/logger.hh"
#include "xwindow.hh"


XWindowManager::XWindowManager()
    : GWindowManager(std::make_unique<XResources>()),
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
            ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask);
    XSetInputFocus(X->display, X->root, RevertToNone, CurrentTime);
    XGrabKeyboard(X->display, X->root, true, GrabModeAsync, GrabModeAsync, CurrentTime);

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

std::unique_ptr<GWindow> XWindowManager::create_window(Rectangle const &rectangle) const
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
                             {e.xexpose.x, e.xexpose.y, e.xexpose.width, e.xexpose.height});
            break;
        case ButtonPress:
        case ButtonRelease:
        {
            // LOG.debug("event: Button %d", e.xbutton.window);
            ClickEvent click_event = map_to_click_event(e.xbutton);
            if (e.xmotion.window == None || e.xmotion.window == X->root)
                on_desktop_click(click_event);
            else
                on_window_click(e.xmotion.window, click_event);
            XAllowEvents(X->display, ReplayPointer, e.xbutton.time);   // forward event to child
            break;
        }
        case MotionNotify:
            // LOG.debug("event: Motion %d", e.xbutton.window);
            if (e.xmotion.window != None || e.xmotion.window != X->root)
                on_window_move_pointer(e.xmotion.window, {e.xmotion.x, e.xmotion.y});
            on_move_pointer({e.xmotion.x_root, e.xmotion.y_root});
            break;
        case ConfigureNotify:
            // LOG.debug("event: ConfigureNotify %d", e.xconfigure.window);
            on_window_configure(e.xconfigure.window,
                                { e.xconfigure.x, e.xconfigure.y, e.xconfigure.width, e.xconfigure.height });
            break;
        case KeyPress:
        case KeyRelease:
            // LOG.debug("event: KeyPress/Release %d", e.xconfigure.window);
            // TODO - send events to parent class, request authorization to propagate
            propagate_keyevent_to_focused_window(e);
            break;
        case CreateNotify:   // LOG.debug("event: CreateNotify %d", e.xcreatewindow.window); break;
        case DestroyNotify:  // LOG.debug("event: DestroyNotify %d", e.xdestroywindow.window); break;
        case MapNotify:      // LOG.debug("event: MapNotify %d", e.xmap.window); break;
        case ReparentNotify: // LOG.debug("event: ReparentNotify %d", e.xreparent.window); break;
        case GraphicsExpose: // LOG.debug("event: GraphicsExpose %d", e.xreparent.window); break;
        case NoExpose:       // LOG.debug("event: NoExpose %d", e.xreparent.window); break;
            break;
        default:
            LOG.debug("Unmapped event received: %d", e.type);
    }

    XFlush(X->display);
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
    return { requested_x, requested_y, (int) child_w, (int) child_h };
}

void XWindowManager::reparent_window(WHandle parent_id, WHandle child_id, Point const &offset)
{
    XAddToSaveSet(X->display, child_id);
    XReparentWindow(X->display, child_id, parent_id, offset.x, offset.y);
    XMapWindow(X->display, child_id);
}

void XWindowManager::expose(GWindow* window)
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

ClickEvent XWindowManager::map_to_click_event(XButtonEvent e) const
{
    ClickEvent click_event {
            .pressed = (e.type == ButtonPress),
            .pos = Point { e.x, e.y },
            .abs_pos = Point { e.x_root, e.y_root },
            .button = ClickEvent::Left,
    };
    switch (e.button) {
        case Button1: click_event.button = ClickEvent::Left; break;
        case Button2: click_event.button = ClickEvent::Middle; break;
        case Button3: click_event.button = ClickEvent::Right; break;
        default: click_event.button = ClickEvent::Other;
    }
    return click_event;
}

void XWindowManager::propagate_keyevent_to_focused_window(XEvent e) const
{
    auto focused = focus_manager().focused_window();
    if (focused) {
        e.xbutton.window = (*focused)->id();
        XSendEvent(X->display, (*focused)->id(), false, KeyPressMask | KeyReleaseMask, &e);
        auto child_id = (*focused)->child_id();
        if (child_id) {
            e.xbutton.window = *child_id;
            XSendEvent(X->display, *child_id, false, KeyPressMask | KeyReleaseMask, &e);
        }
    }
}

void XWindowManager::on_window_configure(WHandle window, Rectangle rectangle)
{
    GWindowManager::on_window_configure(window, rectangle);
    try {
        LWindow* lwindow = windows_.at(window).get();
        ((XWindow *) lwindow)->update_backbuffer_size(rectangle.size());
    } catch (std::out_of_range&) {}
}

void XWindowManager::on_window_expose(WHandle parent, Rectangle rectangle)
{
    if (grab_manager_.is_moving()) {
        try {
            LWindow* window = windows_.at(parent).get();
            ((XWindow *) window)->expose_from_backbuffer(rectangle);
        } catch (std::out_of_range&) {}

    } else {
        GWindowManager::on_window_expose(parent, rectangle);

        try {
            LWindow* window = windows_.at(parent).get();
            ((XWindow *) window)->update_backbuffer(rectangle);
        } catch (std::out_of_range&) {}
    }
}

Size XWindowManager::screen_size() const
{
    return { DisplayWidth(X->display, X->screen), DisplayHeight(X->display, X->screen) };
}