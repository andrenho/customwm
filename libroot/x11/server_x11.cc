#include "server_x11.hh"

#include <cstdio>
#include <cstdlib>

void ServerX11::setup_event_listeners()
{
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    xcb_change_window_attributes(dpy_, scr_->root, XCB_CW_EVENT_MASK, &values);
    xcb_flush(dpy_);
}

void ServerX11::capture_existing_windows()
{
    auto r = xcb_query_tree_reply(dpy_, xcb_query_tree(dpy_, scr_->root), nullptr);
    if (r) {
        for (int i = 0; i < xcb_query_tree_children_length(r); ++i) {
            // TODO - on_map_request(xcb_query_tree_children(r)[i]);
        }
    }
}

void ServerX11::run_event_loop()
{
    while (!xcb_connection_has_error(dpy_)) {
        xcb_generic_event_t *ev = xcb_wait_for_event(dpy_);
        if (ev != nullptr) {
            switch (ev->response_type & ~0x80) {
                case 0: {
                    auto* e = reinterpret_cast<xcb_request_error_t *>(ev);
                    on_error(e);
                    break;
                }
                /*
                case XCB_MAP_REQUEST: {
                    auto* e = reinterpret_cast<xcb_map_request_event_t *>(ev);
                    on_map_request(e->window);
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
                 */
            }
            free(ev);
        }
        xcb_flush(dpy_);
    }
}

void ServerX11::on_error(xcb_request_error_t *e)
{
    if (e->error_code == XCB_ACCESS && e->major_opcode == 2 && e->minor_opcode == 0) {
        fprintf(stderr, "Another window manager seems to be already running.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "X11 error: %s %s %s\n",
            xcb_errors_get_name_for_error(err_ctx_, e->error_code, nullptr),
            xcb_errors_get_name_for_major_code(err_ctx_, e->major_opcode),
            xcb_errors_get_name_for_minor_code(err_ctx_, e->major_opcode, e->minor_opcode));
}
