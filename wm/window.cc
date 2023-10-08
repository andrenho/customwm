#include "window.hh"

#include <algorithm>

Window::Window(xcb_connection_t *dpy, xcb_screen_t* scr, Rectangle area, xcb_window_t child_id, Point child_pos, Resources* res)
    : dpy_(dpy), child_id(child_id), scr_(scr), area_(area), id(xcb_generate_id(dpy)), res_(res)
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

    xcb_flush(dpy);
}

Window::~Window()
{
    std::vector<uint32_t> pixels(colors_.size());
    std::transform(colors_.begin(), colors_.end(), std::back_inserter(pixels),
                   [](std::pair<const Color, uint32_t>& p) { return p.second; });
    xcb_free_colors(dpy_, colormap_, ~0, pixels.size(), pixels.data());

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

    std::vector<xcb_rectangle_t> rects(rectangles.size());
    std::transform(rectangles.begin(), rectangles.end(), std::back_inserter(rects),
                   [](Rectangle const& r) { return xcb_rectangle_t { r.x, r.y, r.w, r.h }; });

    if (filled)
        xcb_poly_fill_rectangle(dpy_, id, gc_, rects.size(), rects.data());
    else
        xcb_poly_rectangle(dpy_, id, gc_, rects.size(), rects.data());

    xcb_flush(dpy_);
}

void Window::draw_polygon(const std::vector<Point> &points, const Color &color, bool filled)
{
    uint32_t vcolor = get_color(color);
    xcb_change_gc(dpy_, gc_, XCB_GC_FOREGROUND, &vcolor);

    std::vector<xcb_point_t> pts(points.size());
    std::transform(points.begin(), points.end(), std::back_inserter(pts),
                   [](Point const& p) { return xcb_point_t { p.x, p.y }; });

    if (filled)
        xcb_fill_poly(dpy_, id, gc_, XCB_POLY_SHAPE_COMPLEX, XCB_COORD_MODE_ORIGIN, pts.size(), pts.data());
    else
        xcb_poly_line(dpy_, XCB_COORD_MODE_ORIGIN, id, gc_, pts.size(), pts.data());

    xcb_flush(dpy_);
}

uint32_t Window::get_color(Color const& color)
{
    if (color.is_black())
        return scr_->black_pixel;
    else if (color.is_white())
        return scr_->white_pixel;

        // look into the cache
    auto it = colors_.find(color);
    if (it != colors_.end())
        return it->second;

    // allocate color
    auto* t = xcb_alloc_color_reply(dpy_,
                                    xcb_alloc_color(dpy_, colormap_, color.r * 256, color.g * 256, color.b * 256),
                                    nullptr);
    uint32_t pixel = t->pixel;
    free(t);

    return pixel;
}

