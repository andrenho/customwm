#include "wm.hh"

#include <stdexcept>
#include <xcb/xcb_keysyms.h>

WM& WM::start()
{
    dpy = xcb_connect(config.display_name.c_str(), nullptr);
    if (xcb_connection_has_error(dpy))
        throw std::runtime_error("Could not connect to display " + config.display_name);

    scr = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;

    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
                      | XCB_EVENT_MASK_STRUCTURE_NOTIFY
                      | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
                      | XCB_EVENT_MASK_PROPERTY_CHANGE;
    xcb_change_window_attributes_checked(dpy, scr->root, XCB_CW_EVENT_MASK, &values);

    xcb_grab_button(dpy, 0, scr->root, XCB_EVENT_MASK_BUTTON_PRESS |
                                        XCB_EVENT_MASK_BUTTON_RELEASE, XCB_GRAB_MODE_ASYNC,
                    XCB_GRAB_MODE_ASYNC, scr->root, XCB_NONE, 1, XCB_MOD_MASK_ANY);
    xcb_flush(dpy);

    return *this;
}

void WM::run()
{
    for (;;);
}

