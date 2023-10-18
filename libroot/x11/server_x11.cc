#include "server_x11.hh"

#include <cstdio>
#include <cstdlib>

#include "../../libengine/engine.hh"
#include "../../libengine/types/types.hh"

void ServerX11::run()
{
    setup_event_listeners();
    capture_existing_windows();
    engine_.call_opt("wm.after_start");

    run_event_loop();
}

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
            on_map_request(xcb_query_tree_children(r)[i]);
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
                    /*
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

void ServerX11::on_map_request(xcb_window_t child_id)
{
    // figure out information about the window
    auto geo = xcb_get_geometry_reply(dpy_, xcb_get_geometry(dpy_, child_id), nullptr);
    Rectangle child_geometry { geo->x, geo->y, geo->width, geo->height };
    auto [outer_pos, inner_pos] = engine_.read<std::pair<Rectangle, Point>>("wm.starting_location", child_geometry);

    // create window
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_EXPOSURE;
    uint32_t outer_id = xcb_generate_id(dpy_);
    xcb_create_window(dpy_, XCB_COPY_FROM_PARENT, outer_id, scr_->root, outer_pos.x, outer_pos.y, outer_pos.h, outer_pos.w,
                      0, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, XCB_CW_EVENT_MASK, &values);

    xcb_change_save_set(dpy_, XCB_SET_MODE_INSERT, child_id);

    xcb_reparent_window(dpy_, child_id, outer_id, inner_pos.x, inner_pos.y);
    xcb_map_window(dpy_, outer_id);
    xcb_map_window(dpy_, child_id);
    xcb_flush(dpy_);

    // register window
    engine_.call("wm.__register_window", outer_id, child_id);

    // cleanup
    free(geo);
}

void ServerX11::on_unmap_notify(xcb_unmap_notify_event_t *e)
{
    auto owindow = engine_.read<std::optional<Window>>("wm.__find_window_by_child_id", e->window);

    if (owindow) {
        xcb_reparent_window_checked(dpy_, e->window, scr_->root, 0, 0);      // checked to ignore errors if the window holder was killed
        xcb_change_save_set_checked(dpy_, XCB_SET_MODE_DELETE, e->window);
        xcb_unmap_window(dpy_, owindow->outer_id);
        xcb_destroy_window(dpy_, owindow->outer_id);
        xcb_flush(dpy_);

        engine_.call("wm.__unregister_window", owindow->outer_id);
    }
}
