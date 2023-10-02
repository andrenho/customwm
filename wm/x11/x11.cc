#include "x11.hh"

#include <stdexcept>
#include <string>

void X11::setup(void* data)
{
    // setup connection
    dpy = xcb_connect((const char *) data, nullptr);
    if (xcb_connection_has_error(dpy))
        throw std::runtime_error(std::string("Could not connect to display ") + (const char *) data);

    // get screen
    scr = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;

    // select event filter
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    xcb_change_window_attributes_checked(dpy, scr->root, XCB_CW_EVENT_MASK, &values);
    xcb_flush(dpy);
}

bool X11::running() const
{
    return !xcb_connection_has_error(dpy);
}

void X11::do_events(IEvents *events)
{
    xcb_generic_event_t *ev = xcb_wait_for_event(dpy);
    if (ev != nullptr) {
        switch (ev->response_type & ~0x80) {
            case XCB_MAP_REQUEST: {
                auto* e = reinterpret_cast<xcb_map_request_event_t *>(ev);
                events->on_create_window(e->window);
                break;
            }
            case XCB_UNMAP_NOTIFY: {
                auto* e = reinterpret_cast<xcb_unmap_notify_event_t *>(ev);
                events->on_destroy_window(e->window);
                break;
            }
            case XCB_EXPOSE: {
                auto* e = reinterpret_cast<xcb_expose_event_t *>(ev);
                events->on_expose_window(e->window, { e->x, e->y, e->width, e->height });
                break;
            }
        }
        free(ev);
    }
    xcb_flush(dpy);
}

Area X11::inner_window_size(const Window &w) const
{
    auto geo = xcb_get_geometry_reply(dpy, xcb_get_geometry(dpy, w.inner_id), nullptr);
    Area area = { geo->x, geo->y, geo->width, geo->height };
    free(geo);
    return area;
}

Area X11::screen_size() const
{
    return { 0, 0, scr->width_in_pixels, scr->height_in_pixels };
}

Handle X11::reparent_window(const Window &w, const Point &pos, const Area &window_sz, const Padding &padding)
{
    uint32_t outer_w = xcb_generate_id(dpy);

    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_EXPOSURE;
    xcb_create_window(dpy, XCB_COPY_FROM_PARENT, outer_w, scr->root, (int16_t) pos.x, (int16_t) pos.y,
                      window_sz.w + padding.left + padding.right + 1,
                      window_sz.h + padding.top + padding.bottom + 1,
                      1, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, XCB_CW_EVENT_MASK, &values);
    xcb_change_save_set(dpy, XCB_SET_MODE_INSERT, w.inner_id);
    xcb_reparent_window(dpy, w.inner_id, outer_w, padding.left, padding.top);
    xcb_map_window(dpy, outer_w);
    xcb_map_window(dpy, w.inner_id);
    xcb_flush(dpy);

    return outer_w;
}

void X11::destroy_window(const Window &w)
{
    xcb_unmap_window(dpy, w.outer_id);
    xcb_reparent_window(dpy, w.inner_id, scr->root, 0, 0);
    xcb_change_save_set(dpy, XCB_SET_MODE_DELETE, w.inner_id);
    xcb_destroy_window(dpy, w.outer_id);
    xcb_flush(dpy);
}
