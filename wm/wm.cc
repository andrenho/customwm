#include "wm.hh"

#include <stdexcept>
#include <xcb/xcb_keysyms.h>

#include <cstdio>
#include <cstdarg>

WM& WM::start()
{
    dpy = xcb_connect(config.display_name.c_str(), nullptr);
    if (xcb_connection_has_error(dpy))
        throw std::runtime_error("Could not connect to display " + config.display_name);

    scr = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;

    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
                      | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
    xcb_change_window_attributes_checked(dpy, scr->root, XCB_CW_EVENT_MASK, &values);
    xcb_flush(dpy);  // TODO

    xcb_grab_button(dpy, 0, scr->root, XCB_EVENT_MASK_BUTTON_PRESS |
                                        XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
                    XCB_GRAB_MODE_ASYNC, scr->root, XCB_NONE, 1, XCB_MOD_MASK_ANY);
    xcb_flush(dpy);

    return *this;
}

void WM::run()
{
    int r = 0;
    while (r == 0)
        r = handle_event();
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


int WM::handle_event()
{
    int r = xcb_connection_has_error(dpy);
    if (r == 0) {
        xcb_generic_event_t *ev = xcb_wait_for_event(dpy);
        if (ev != nullptr) {
            printf("event: 0x%x\n", ev->response_type);
            switch (ev->response_type & ~0x80) {
                // case XCB_CREATE_NOTIFY:     on_create_notify(*(xcb_create_notify_event_t *)(ev)); break;
                case XCB_CONFIGURE_REQUEST: on_configure_request(*(xcb_configure_request_event_t *)(ev)); break;
                case XCB_MAP_REQUEST:       on_map_request(*(xcb_map_request_event_t *)(ev)); break;
                case XCB_BUTTON_PRESS:      on_button_press(*reinterpret_cast<xcb_button_press_event_t *>(ev)); break;
            }
            // TODO - handle event
            free(ev);
        }
    }
    xcb_flush(dpy);
    return r;
}

void WM::on_create_notify(xcb_create_notify_event_t const& e)
{
    log("CreateNotify", e.window, "pos: (%d %d)", e.x, e.y);
}

void WM::on_configure_request(const xcb_configure_request_event_t &e)
{
    log("ConfigureRequest", e.window, "pos: (%d %d) size: (%d %d) border: %d", e.x, e.y, e.width, e.height, e.border_width);

    static uint16_t config_mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH |
            XCB_CONFIG_WINDOW_HEIGHT | XCB_CONFIG_WINDOW_BORDER_WIDTH | XCB_CONFIG_WINDOW_SIBLING |
            XCB_CONFIG_WINDOW_STACK_MODE;

    xcb_configure_window_value_list_t cw = {
            .x = e.x,
            .y = e.y,
            .width = e.width,
            .height = e.height,
            .border_width = e.border_width,
            .sibling = e.sibling,
            .stack_mode = e.stack_mode,
    };

    xcb_configure_window_aux(dpy, e.window, config_mask, &cw);
    xcb_flush(dpy);
}

void WM::on_map_request(const xcb_map_request_event_t &e)
{
    log("MapWindow", e.window);
    xcb_map_window(dpy, e.window);

    uint32_t vals[5] = { 0, 0, 800, 800, 16 };
    xcb_configure_window(dpy, e.window,
            XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT | XCB_CONFIG_WINDOW_BORDER_WIDTH,
            vals);
    xcb_flush(dpy);

    /*
    int values[1] = { XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_FOCUS_CHANGE };
    xcb_change_window_attributes_checked(dpy, e.window, XCB_CW_EVENT_MASK, values);
    xcb_flush(dpy);
     */
}

void WM::on_button_press(xcb_button_press_event_t const& e)
{
}

