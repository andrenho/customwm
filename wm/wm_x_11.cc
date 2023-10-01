#include "wm_x_11.hh"

void WM_X11::setup()
{
    connect_to_X();
    listen_to_events();
}

void WM_X11::connect_to_X()
{
    dpy = xcb_connect(lib_.display().c_str(), nullptr);
    if (xcb_connection_has_error(dpy))
        throw std::runtime_error("Could not connect to display " + lib_.display());

    scr = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;
}

void WM_X11::listen_to_events()
{
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    xcb_change_window_attributes_checked(dpy, scr->root, XCB_CW_EVENT_MASK, &values);
    xcb_flush(dpy);
}


void WM_X11::loop()
{
    for (;;) {
        if (xcb_connection_has_error(dpy) != 0)
            break;  // TODO - print error?

        xcb_generic_event_t *ev = xcb_wait_for_event(dpy);
        if (ev != nullptr) {
            switch (ev->response_type & ~0x80) {
                case XCB_MAP_REQUEST:       map_request(*(xcb_map_request_event_t *) ev); break;
                case XCB_UNMAP_NOTIFY:      unmap_notify(*(xcb_unmap_notify_event_t *) ev); break;
            }
            free(ev);
        }
        xcb_flush(dpy);
    }
}

void WM_X11::map_request(xcb_map_request_event_t const& e)
{
    Window& w = windows_.add({ .inner_id = e.window });

    Padding padding = lib_.theme().window_border_width(w, 1);
    Point pos = window_starting_pos(w);

    auto geo = xcb_get_geometry_reply(dpy, xcb_get_geometry(dpy, e.window), nullptr);
    geo->width += padding.left + padding.right;
    geo->height += padding.top + padding.bottom;

    uint32_t outer_w = xcb_generate_id(dpy);
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    xcb_create_window(dpy, XCB_COPY_FROM_PARENT, outer_w, scr->root, pos.x, pos.y, geo->width, geo->height,
                      geo->border_width, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, XCB_CW_EVENT_MASK, &values);
    xcb_change_save_set(dpy, XCB_SET_MODE_INSERT, e.window);
    xcb_reparent_window(dpy, e.window, outer_w, padding.left, padding.top);
    xcb_map_window(dpy, outer_w);
    xcb_map_window(dpy, e.window);

    w.outer_id = outer_w;

    xcb_flush(dpy);

    free(geo);
}

void WM_X11::unmap_notify(xcb_unmap_notify_event_t const& e)
{
    auto ow = windows_.find(e.window);
    if (ow) {
        xcb_unmap_window(dpy, ow->outer_id);
        xcb_reparent_window(dpy, e.window, scr->root, 0, 0);
        xcb_change_save_set(dpy, XCB_SET_MODE_DELETE, e.window);
        xcb_destroy_window(dpy, ow->outer_id);
        windows_.remove(e.window);
        xcb_flush(dpy);
    }
}
