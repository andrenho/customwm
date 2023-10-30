#include "resourcesx11.hh"
#include "theme/logger.hh"

#include <X11/Xft/Xft.h>

// see: https://archive.irixnet.org/apocrypha/nekonomicon/forum/15/8794/1.html

ResourcesX11::ResourcesX11(Display* dpy, Theme& theme)
    : dpy_(dpy), theme_(theme)
{

}

ResourcesX11::~ResourcesX11()
{
    for (auto& [_, xft_color] : xft_colors_)
        XftColorFree(dpy_, DefaultVisual(dpy_, DefaultScreen(dpy_)), DefaultColormap(dpy_, DefaultScreen(dpy_)), &xft_color);

    std::vector<unsigned long> pixels(colors_.size());
    std::transform(colors_.begin(), colors_.end(), std::back_inserter(pixels),
                   [](std::pair<const Color, unsigned long>& p) { return p.second; });
    XFreeColors(dpy_, DefaultColormap(dpy_, DefaultScreen(dpy_)), pixels.data(), (int) pixels.size(), 0);

    for (auto const& [_, font]: fonts_)
        XftFontClose(dpy_, font);
}

unsigned long ResourcesX11::get_color(Color const &color) const
{
    // black or white?
    if (color.is_white())
        return WhitePixel(dpy_, DefaultScreen(dpy_));
    else if (color.is_black())
        return BlackPixel(dpy_, DefaultScreen(dpy_));

    // cached?
    auto it = colors_.find(color);
    if (it != colors_.end())
        return it->second;

    // non-cached
    XColor xcolor = {
            .red = static_cast<unsigned short>(color.r * 256),
            .green = static_cast<unsigned short>(color.g * 256),
            .blue = static_cast<unsigned short>(color.b * 256),
            .flags = DoRed | DoGreen | DoBlue,
    };
    if (XAllocColor(dpy_, DefaultColormap(dpy_, DefaultScreen(dpy_)), &xcolor) == 0)
        LOG.info("Color allocation failed (%d %d %d)", color.r, color.g, color.b);
    colors_.emplace(color, xcolor.pixel);
    return xcolor.pixel;
}

XftColor& ResourcesX11::get_xft_color(Color const& color) const
{
    // cached?
    auto it = xft_colors_.find(color);
    if (it != xft_colors_.end())
        return it->second;

    // non-cached
    XRenderColor xcolor = {
            .red = static_cast<unsigned short>(color.r * 256),
            .green = static_cast<unsigned short>(color.g * 256),
            .blue = static_cast<unsigned short>(color.b * 256),
            .alpha = static_cast<unsigned short>(color.a * 256),
    };
    XftColor xft_color;
    if (XftColorAllocValue(dpy_, DefaultVisual(dpy_, DefaultScreen(dpy_)), DefaultColormap(dpy_, DefaultScreen(dpy_)),
                           &xcolor, &xft_color) == 0)
        LOG.info("XftColor allocation failed (%d %d %d)", color.r, color.g, color.b);
    auto kv = xft_colors_.emplace(color, xft_color);
    return kv.first->second;
}

XftFont* ResourcesX11::get_font(std::string const& key) const
{
    auto it = fonts_.find(key);
    if (it != fonts_.end())
        return it->second;

    auto [it2, _] = fonts_.emplace(key, load_font(key));
    return it2->second;
}

XftFont* ResourcesX11::load_font(std::string const& key) const
{
    auto font_names = theme_.resource_font(key);

    for (auto const& font_name: font_names) {
        XftFont* font = XftFontOpenName(dpy_, DefaultScreen(dpy_), font_name.c_str());
        if (font) {
            LOG.debug("Loaded font '%s' as %p", font_name.c_str(), font);
            return font;
        }
    }

    throw std::runtime_error("A font for resource '" + key + "' could not be loaded.");
}

