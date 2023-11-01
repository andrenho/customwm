#include "resourcesx11.hh"

#include <X11/Xft/Xft.h>

#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_JPEG
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "contrib/stb_image.h"
#include "theme/logger.hh"

// see: https://archive.irixnet.org/apocrypha/nekonomicon/forum/15/8794/1.html

ResourcesX11::ResourcesX11(Display* dpy, Theme& theme)
    : dpy_(dpy), theme_(theme)
{

}

ResourcesX11::~ResourcesX11()
{
    for (auto& [_, xft_color] : xft_colors_)
        XftColorFree(dpy_, DefaultVisual(dpy_, DefaultScreen(dpy_)), DefaultColormap(dpy_, DefaultScreen(dpy_)), &xft_color);

    for (auto& [_, image] : images_) {
        XFreePixmap(dpy_, image.pixmap);
        XFreePixmap(dpy_, image.mask);
    }

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

std::pair<Image, Rectangle> ResourcesX11::get_slice_image(std::string const& slice_name) const
{
    auto slice = theme_.resource_slice(slice_name);
    if (!slice.has_value())
        throw std::runtime_error("Slice '" + slice_name + "' not found in theme file.");

    Image image;
    auto it = images_.find(slice->image);
    if (it == images_.end()) {
        auto [it2, _] = images_.emplace(slice->image, load_image(slice->image));
        image = it2->second;
    } else {
        image = it->second;
    }

    return { image, slice->rect };
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

Image ResourcesX11::load_image(std::string const& key) const
{
    auto image = theme_.resource_image(key);

    // load image pixels
    int w, h;
    int channels;
    uint8_t* data = stbi_load_from_memory(image.data(), image.size(), &w, &h, &channels, 4);

    // convert RGB (from STI) to BGR (expected by X) and create mask
    auto* image_data = new uint8_t[w * h * 4];  // it'll be freed by XDestroyImage

    uint8_t mask_data[w * h / 8];
    memset(mask_data, 0, sizeof mask_data);

    for (int i = 0; i < w * h * 4; i += 4) {

        int y = (i / 4) / w;
        int x = (i / 4) - (y * w);
        printf("%d\n", x);

        image_data[i] = data[i+2];
        image_data[i+1] = data[i+1];
        image_data[i+2] = data[i];
        image_data[i+3] = 0;

        if (data[i+3] == 0xff) { // non-transparent
            mask_data[i / 4 / 8] |= (1 << ((i / 4) % 8));
        }
    }

    // create image
    int depth = DefaultDepthOfScreen(DefaultScreenOfDisplay(dpy_));
    int scr = DefaultScreen(dpy_);

    XImage* ximage = XCreateImage(dpy_, DefaultVisual(dpy_, DefaultScreen(dpy_)), depth, ZPixmap, 0,
                                  (char *) image_data, w, h, 32, 0);
    Pixmap pixmap = XCreatePixmap(dpy_, DefaultRootWindow(dpy_), w, h, depth);
    XPutImage(dpy_, pixmap, DefaultGC(dpy_, scr), ximage, 0, 0, 0, 0, w, h);

    XDestroyImage(ximage);
    free(data);

    // create mask
    Pixmap mask = XCreatePixmapFromBitmapData(dpy_, DefaultRootWindow(dpy_), (char *) mask_data, w, h,
                                              WhitePixel(dpy_, scr), BlackPixel(dpy_, scr), 1);

    LOG.debug("Image created for '%s'", key.c_str());

    return { .pixmap = pixmap, .mask = mask };
}

