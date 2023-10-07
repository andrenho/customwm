#include "window.hh"

Window::Window(xcb_connection_t *dpy, xcb_window_t root, Rectangle area, xcb_window_t child_id, Point child_pos)
    : dpy_(dpy), child_id(child_id), root_(root), area_(area), id(xcb_generate_id(dpy))
{
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_EXPOSURE;
    xcb_create_window(dpy, XCB_COPY_FROM_PARENT, id, root, area.x, area.y, area.w, area.h,
                      0, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, XCB_CW_EVENT_MASK, &values);

    xcb_change_save_set(dpy, XCB_SET_MODE_INSERT, child_id);

    xcb_reparent_window(dpy, child_id, id, child_pos.x, child_pos.y);
    xcb_map_window(dpy, id);
    xcb_map_window(dpy, child_id);
    xcb_flush(dpy);

    gc_ = xcb_generate_id(dpy);
    xcb_create_gc (dpy, gc_, id, 0, nullptr);

    xcb_flush(dpy);
}

Window::~Window()
{
    xcb_free_gc(dpy_, gc_);

    xcb_unmap_window(dpy_, id);
    xcb_reparent_window(dpy_, child_id, root_, 0, 0);
    xcb_change_save_set(dpy_, XCB_SET_MODE_DELETE, child_id);
    xcb_destroy_window(dpy_, id);

    xcb_flush(dpy_);
}

void Window::draw_rectangles(const std::vector<Rectangle> &rectangles, const std::string &color, bool filled)
{
    // TODO
}
