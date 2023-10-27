#include "window.hh"

#include <cstring>
#include <algorithm>

Window::Window(xcb_connection_t *dpy, xcb_screen_t* scr, Rectangle area, xcb_window_t child_id, Point child_pos, ResourceManager* res)
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

    colors_ = std::make_unique<Colors>(dpy, scr, id);

    xcb_flush(dpy);
}

Window::~Window()
{
    colors_.reset();

    xcb_free_gc(dpy_, gc_);

    xcb_reparent_window_checked(dpy_, child_id, scr_->root, 0, 0);      // checked to ignore errors if the window holder was killed
    xcb_change_save_set_checked(dpy_, XCB_SET_MODE_DELETE, child_id);
    xcb_unmap_window(dpy_, id);
    xcb_destroy_window(dpy_, id);

    xcb_flush(dpy_);
}

void Window::draw_rectangles(const std::vector<Rectangle> &rectangles, Color const& color, bool filled)
{
    uint32_t vcolor = colors_->get_color(color);
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
    uint32_t vcolor = colors_->get_color(color);
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

void Window::draw_image(Point p, std::string const &image_idx, std::string const &slice)
{
    auto [pixmap, rect] = res_->image(image_idx, slice);
    xcb_copy_area(dpy_, pixmap, id, gc_, rect.x, rect.y, p.x, p.y, rect.w, rect.h);
    xcb_flush(dpy_);
}

void Window::write(Point p, std::string const &text, std::string const &font_name, Color const &color, std::optional<TextBoundingBox> attrib)
{
    ResourceManager::Font font = res_->font(font_name);

    if (font.type == FontType::X11) {

        int16_t x = p.x;
        int16_t y = p.y;

        size_t i = 0;
        xcb_char2b_t wstr[text.length()];
        for (char c: text)
            wstr[i++] = { .byte1 = 0, .byte2 = (uint8_t) c };

        xcb_generic_error_t *err = nullptr;
        xcb_query_text_extents_reply_t* ext = xcb_query_text_extents_reply(
                dpy_, xcb_query_text_extents(dpy_, font.x11_font, text.length(), wstr), &err);

        if (attrib) {

            if (attrib->halign == TextBoundingBox::HAlignment::RIGHT)
                x += attrib->size.w - ext->overall_width;
            else if (attrib->halign == TextBoundingBox::HAlignment::CENTER)
                x += (attrib->size.w / 2) - (ext->overall_width / 2);

            if (attrib->valign == TextBoundingBox::VAlignment::TOP)
                y += ext->overall_ascent;
            else if (attrib->valign == TextBoundingBox::VAlignment::CENTER)
                y += (attrib->size.h / 2) + (ext->overall_ascent / 2);
            else if (attrib->valign == TextBoundingBox::VAlignment::BOTTOM)
                y += (attrib->size.h / 2) + ext->overall_ascent;
        } else {
            y += ext->overall_ascent;
        }

        uint32_t vcolor = colors_->get_color(color);
        uint32_t values[] = { vcolor, font.x11_font };
        xcb_change_gc(dpy_, gc_, XCB_GC_FOREGROUND | XCB_GC_FONT, values);

        uint8_t len = std::min(text.length(), (size_t) 256);
        struct {
            uint8_t nchars;
            int8_t delta;
            char text[256];
        } item = { len, 0 };
        memcpy(item.text, text.c_str(), len);
        xcb_poly_text_8(dpy_, id, gc_, x, y, item.nchars + 2, (const uint8_t*) &item);

        xcb_flush(dpy_);
    }
}

std::string Window::name() const
{
    auto r = xcb_get_property_reply(dpy_, xcb_get_property(dpy_, 0, child_id, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 256), nullptr);
    const char* window_name = (const char *) xcb_get_property_value(r);
    std::string name = window_name;
    free(r);
    return name;
}
