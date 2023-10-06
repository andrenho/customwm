#include "wm.hh"
#include "../libtheme/types/types.hh"
#include "../libtheme/types/TWindow.hh"

WM::WM(std::string const& display, Theme& theme)
    : theme_(theme)
{
    // setup connection
    dpy = xcb_connect(display.c_str(), nullptr);
    if (xcb_connection_has_error(dpy))
        throw std::runtime_error("Could not connect to display " + display);

    // get screen
    scr = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;

    // select event filter
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    xcb_change_window_attributes_checked(dpy, scr->root, XCB_CW_EVENT_MASK, &values);
    xcb_flush(dpy);
}

WM::~WM()
{
    xcb_disconnect(dpy);
}

void WM::run()
{
    while (!xcb_connection_has_error(dpy)) {
        xcb_generic_event_t *ev = xcb_wait_for_event(dpy);
        if (ev != nullptr) {
            switch (ev->response_type & ~0x80) {
                case XCB_MAP_REQUEST: {
                    auto* e = reinterpret_cast<xcb_map_request_event_t *>(ev);
                    on_map_request(e);
                    break;
                }
                case XCB_UNMAP_NOTIFY: {
                    auto* e = reinterpret_cast<xcb_unmap_notify_event_t *>(ev);
                    on_unmap_notify(e);
                    break;
                }
                case XCB_EXPOSE: {
                    auto* e = reinterpret_cast<xcb_expose_event_t *>(ev);
                    on_expose(e);
                    break;
                }
            }
            free(ev);
        }
        xcb_flush(dpy);
    }
}

void WM::on_map_request(xcb_map_request_event_t *e)
{
    // figure out information about the window
    uint32_t id = xcb_generate_id(dpy);
    auto geo = xcb_get_geometry_reply(dpy, xcb_get_geometry(dpy, e->window), nullptr);

    // get configuration from theme
    TWindow twin {};
    Padding padding = theme_.read<Padding>("window.padding", twin);
    WindowStartingPos wps = theme_.read<WindowStartingPos>("window.starting_pos", twin);
    auto [x, y] = calculate_starting_position(wps, geo);

    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_EXPOSURE;
    xcb_create_window(dpy, XCB_COPY_FROM_PARENT, id, scr->root, x, y,
                      geo->width + padding.left + padding.right + 1,
                      geo->height + padding.top + padding.bottom + 1,
                      0, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, XCB_CW_EVENT_MASK, &values);
    xcb_change_save_set(dpy, XCB_SET_MODE_INSERT, e->window);
    xcb_reparent_window(dpy, e->window, id, padding.left, padding.top);
    xcb_map_window(dpy, id);
    xcb_map_window(dpy, e->window);
    xcb_flush(dpy);

    /*
    // fill out window fields
    w.outer_id = outer_id;
    w.x = pos.x;
    w.y = pos.y;
    w.w = window_size.w;
    w.h = window_size.h;

    // create GC
    brushes_[outer_id] = x11_.create_brush(w);
     */

    windows_[id] = Window { .id = id, .child_id = e->window };

    free(geo);
}

void WM::on_unmap_notify(xcb_unmap_notify_event_t *e)
{
    /*
    auto ow = find_window(window_id);
    if (ow) {
        x11_.destroy_window(**ow);
        brushes_.erase((*ow)->outer_id);
        windows_.erase((*ow)->inner_id);
    }

    xcb_unmap_window(dpy, w.outer_id);
    xcb_reparent_window(dpy, w.inner_id, scr->root, 0, 0);
    xcb_change_save_set(dpy, XCB_SET_MODE_DELETE, w.inner_id);
    xcb_destroy_window(dpy, w.outer_id);
    xcb_flush(dpy);
     */
}

void WM::on_expose(xcb_expose_event_t *e)
{

}

std::pair<int16_t, int16_t> WM::calculate_starting_position(WindowStartingPos const &wsp, xcb_get_geometry_reply_t *geo)
{
    switch (wsp.starting_pos) {
        case WindowStartingPos::Cascade: {
            int16_t x = cascade_ * 24;
            int16_t y = cascade_ * 24;
            ++cascade_;
            if (cascade_ == 5)
                cascade_ = 0;
            return { x, y };
        }
        case WindowStartingPos::Center:
            return { (scr->width_in_pixels / 2) - (geo->width / 2), (scr->height_in_pixels / 2) - (geo->height / 2) };
        case WindowStartingPos::Random:
            return { rand() % (scr->width_in_pixels / 3), rand() % (scr->height_in_pixels / 3) };
        case WindowStartingPos::Maximized:
            throw std::runtime_error("Not implemented");  // TODO
        case WindowStartingPos::Requested:
            return { geo->x, geo->y };
        case WindowStartingPos::Custom:
            return { wsp.point.x, wsp.point.y };
    }
}
