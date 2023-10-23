#include "colors.hh"

#include <vector>

Colors::Colors(xcb_connection_t *dpy, xcb_screen_t *scr, xcb_window_t window)
    : dpy_(dpy), scr_(scr)
{
    colormap_ = xcb_generate_id(dpy_);
    xcb_create_colormap(dpy_, XCB_COLORMAP_ALLOC_NONE, colormap_, window, scr_->root_visual);
}

Colors::~Colors()
{
    std::vector<uint32_t> pixels(colors_.size());
    std::transform(colors_.begin(), colors_.end(), std::back_inserter(pixels),
                   [](std::pair<const Color, uint32_t>& p) { return p.second; });
    xcb_free_colors(dpy_, colormap_, ~0, pixels.size(), pixels.data());

    xcb_free_colormap(dpy_, colormap_);  // TODO - how to free colors?
}

uint32_t Colors::get_color(Color const &color) const
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
