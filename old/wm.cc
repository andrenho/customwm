#include "wm.hh"

#include <stdexcept>
#include <xcb/xcb_keysyms.h>

#include <cstdio>
#include <cstdarg>

void WM::start()
{
    dpy = xcb_connect(config.display_name.c_str(), nullptr);
    if (xcb_connection_has_error(dpy))
        throw std::runtime_error("Could not connect to display " + config.display_name);

    scr = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;

    listen_to_events();
}

void WM::listen_to_events() const
{
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY; /* |
                      XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_PROPERTY_CHANGE; */
    xcb_change_window_attributes_checked(dpy, scr->root, XCB_CW_EVENT_MASK, &values);
    xcb_flush(dpy);  // TODO

    xcb_grab_button(dpy, 0, scr->root, XCB_EVENT_MASK_BUTTON_PRESS |
                                       XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
                    XCB_GRAB_MODE_ASYNC, scr->root, XCB_NONE, 1, XCB_MOD_MASK_ANY);
    xcb_flush(dpy);
}

void WM::run()
{
    for (;;) {
        if (xcb_connection_has_error(dpy) != 0)
            break;  // TODO - print error?

        xcb_generic_event_t *ev = xcb_wait_for_event(dpy);
        if (ev != nullptr) {
            switch (ev->response_type & ~0x80) {
                // case XCB_CONFIGURE_REQUEST: on_configure_request(*(xcb_configure_request_event_t *) ev); break;
                case XCB_MAP_REQUEST:       on_map_request(*(xcb_map_request_event_t *) ev); break;
                case XCB_UNMAP_NOTIFY:      on_unmap_notify(*(xcb_unmap_notify_event_t *) ev); break;
                // case XCB_BUTTON_PRESS:      on_button_press(*reinterpret_cast<xcb_button_press_event_t *>(ev)); break;
                default: log("Unmapped event", 0, "event 0x%x", ev->response_type);
            }
            free(ev);
        }
        xcb_flush(dpy);
    }
}

void WM::log(const std::string &event_type, uint32_t window_id, const char *fmt, ...) const
{
    if (!config.debug_messages)
        return;

    printf("%s: window %d ", event_type.c_str(), window_id);

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}

void WM::on_map_request(const xcb_map_request_event_t &e)
{
    log("MapWindow", e.window);

    uint32_t outer_w = xcb_generate_id(dpy);
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    xcb_create_window(dpy, XCB_COPY_FROM_PARENT, outer_w, scr->root, 10, 10, 100, 100, 3,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, XCB_CW_EVENT_MASK, &values);
    xcb_change_save_set(dpy, XCB_SET_MODE_INSERT, outer_w);
    xcb_reparent_window(dpy, e.window, outer_w, 3, 3);
    xcb_map_window(dpy, outer_w);
    xcb_map_window(dpy, e.window);

    xcb_flush(dpy);
}

void WM::on_unmap_notify(const xcb_unmap_notify_event_t &event)
{

}

void WM::on_button_press(xcb_button_press_event_t const& e)
{
}