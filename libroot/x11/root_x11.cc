#include "root_x11.hh"

RootX11::RootX11(std::optional<std::string> const &display)
    : Root(display)
{
    std::string display_var = getenv("DISPLAY");
    if (display_var.empty())
        display_var = ":0.0";

    dpy_ = xcb_connect(display.value_or(display_var).c_str(), nullptr);
    if (xcb_connection_has_error(dpy_))
        throw std::runtime_error("Could not connect to display " + display.value_or(display_var));

    xcb_errors_context_new(dpy_, &err_ctx);

    scr_ = xcb_setup_roots_iterator(xcb_get_setup(dpy_)).data;
}

RootX11::~RootX11()
{
    xcb_errors_context_free(err_ctx);
    xcb_disconnect(dpy_);
}

void RootX11::setup_event_listeners(EventListener &event_listener)
{
    Root::setup_event_listeners(event_listener);

    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    xcb_change_window_attributes(dpy_, scr_->root, XCB_CW_EVENT_MASK, &values);
    xcb_flush(dpy_);
}

void RootX11::capture_existing_windows()
{
    auto r = xcb_query_tree_reply(dpy_, xcb_query_tree(dpy_, scr_->root), nullptr);
    if (r) {
        for (int i = 0; i < xcb_query_tree_children_length(r); ++i) {
            // TODO - on_map_request(xcb_query_tree_children(r)[i]);
        }
    }
}

void RootX11::run_event_loop()
{
    while (!xcb_connection_has_error(dpy_)) {
        xcb_generic_event_t *ev = xcb_wait_for_event(dpy_);
        if (ev != nullptr) {
            switch (ev->response_type & ~0x80) {
                case 0: {
                    auto* e = reinterpret_cast<xcb_request_error_t *>(ev);
                    // on_error(e);
                    break;
                }
                case XCB_MAP_REQUEST: {
                    auto* e = reinterpret_cast<xcb_map_request_event_t *>(ev);
                    // on_map_request(e->window);
                    break;
                }
                case XCB_UNMAP_NOTIFY: {
                    auto* e = reinterpret_cast<xcb_unmap_notify_event_t *>(ev);
                    // on_unmap_notify(e);
                    break;
                }
                case XCB_EXPOSE: {
                    auto* e = reinterpret_cast<xcb_expose_event_t *>(ev);
                    // on_expose(e);
                    break;
                }
            }
            free(ev);
        }
        xcb_flush(dpy_);
    }

}

