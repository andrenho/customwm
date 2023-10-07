#include "window.hh"

#include <algorithm>

Window::Window(xcb_connection_t *dpy, xcb_screen_t* scr, Rectangle area, xcb_window_t child_id, Point child_pos)
    : dpy_(dpy), child_id(child_id), scr_(scr), area_(area), id(xcb_generate_id(dpy))
{
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                      XCB_EVENT_MASK_EXPOSURE;
    xcb_create_window(dpy, XCB_COPY_FROM_PARENT, id, scr_->root, area.x, area.y, area.w, area.h,
                      0, XCB_WINDOW_CLASS_INPUT_OUTPUT, XCB_COPY_FROM_PARENT, XCB_CW_EVENT_MASK, &values);

    xcb_change_save_set(dpy, XCB_SET_MODE_INSERT, child_id);

    xcb_reparent_window(dpy, child_id, id, child_pos.x, child_pos.y);
    xcb_map_window(dpy, id);
    xcb_map_window(dpy, child_id);
    xcb_flush(dpy);

    gc_ = xcb_generate_id(dpy);
    xcb_create_gc (dpy, gc_, id, 0, nullptr);

    colormap_ = xcb_generate_id(dpy_);
    xcb_create_colormap(dpy_, XCB_COLORMAP_ALLOC_NONE, colormap_, id, scr_->root_visual);
    colors_.emplace(Color { 0, 0, 0 }, scr_->black_pixel);
    colors_.emplace(Color { 255, 255, 255 }, scr_->white_pixel);

    xcb_flush(dpy);
}

Window::~Window()
{
    xcb_free_colormap(dpy_, colormap_);  // TODO - how to free colors?
    xcb_free_gc(dpy_, gc_);

    xcb_unmap_window(dpy_, id);
    xcb_reparent_window(dpy_, child_id, scr_->root, 0, 0);
    xcb_change_save_set(dpy_, XCB_SET_MODE_DELETE, child_id);
    xcb_destroy_window(dpy_, id);

    xcb_flush(dpy_);
}

void Window::draw_rectangles(const std::vector<Rectangle> &rectangles, Color const& color, bool filled)
{
    uint32_t vcolor = get_color(color);
    xcb_change_gc(dpy_, gc_, XCB_GC_FOREGROUND, &vcolor);

    std::vector<xcb_rectangle_t> rects;
    std::transform(rectangles.begin(), rectangles.end(), std::back_inserter(rects),
                   [](Rectangle const& r) { return xcb_rectangle_t { r.x, r.y, r.w, r.h }; });

    if (filled)
        xcb_poly_fill_rectangle(dpy_, id, gc_, rects.size(), rects.data());
    else
        xcb_poly_rectangle(dpy_, id, gc_, rects.size(), rects.data());

    xcb_flush(dpy_);
}

uint32_t Window::get_color(Color const& color)
{
    // look into the cache
    auto it = colors_.find(color);
    if (it != colors_.end())
        return it->second;

    // TODO
    return scr_->white_pixel;
}
