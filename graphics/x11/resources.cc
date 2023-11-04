#include "resources.hh"

#include <X11/Xft/Xft.h>

#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_JPEG
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "contrib/stb_image.h"

#include "theme/logger.hh"
#include "x11.hh"

Resources::~Resources()
{
    for (auto& [_, xft_color] : xft_colors_)
        XftColorFree(x11.display, x11.visual, x11.colormap, &xft_color);

    for (auto& [_, image] : images_) {
        XFreePixmap(x11.display, image.pixmap);
        XFreePixmap(x11.display, image.mask);
    }

    std::vector<unsigned long> pixels(colors_.size());
    std::transform(colors_.begin(), colors_.end(), std::back_inserter(pixels),
                   [](std::pair<const Color, unsigned long>& p) { return p.second; });
    XFreeColors(x11.display, x11.colormap, pixels.data(), (int) pixels.size(), 0);

    for (auto const& [_, font]: fonts_)
        XftFontClose(x11.display, font);
}

unsigned long Resources::get_color(Color const &color) const
{
    // black or white?
    if (color.is_white())
        return x11.white;
    else if (color.is_black())
        return x11.black;

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
    if (XAllocColor(x11.display, x11.colormap, &xcolor) == 0)
        LOG.info("Color allocation failed (%d %d %d)", color.r, color.g, color.b);
    colors_.emplace(color, xcolor.pixel);
    return xcolor.pixel;
}

XftColor& Resources::get_xft_color(Color const& color) const
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
    if (XftColorAllocValue(x11.display, x11.visual, x11.colormap, &xcolor, &xft_color) == 0)
        LOG.info("XftColor allocation failed (%d %d %d)", color.r, color.g, color.b);
    auto kv = xft_colors_.emplace(color, xft_color);
    return kv.first->second;
}

XftFont* Resources::get_font(std::string const& key) const
{
    auto it = fonts_.find(key);
    if (it != fonts_.end())
        return it->second;

    auto [it2, _] = fonts_.emplace(key, load_font(key));
    return it2->second;
}

std::pair<Image, Rectangle> Resources::get_slice_image(std::string const& slice_name) const
{
    auto slice = theme.resource_slice(slice_name);
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

XftFont* Resources::load_font(std::string const& key) const
{
    auto font_names = theme.resource_font(key);

    for (auto const& font_name: font_names) {
        XftFont* font = XftFontOpenName(x11.display, x11.screen, font_name.c_str());
        if (font) {
            LOG.debug("Loaded font '%s' as %p", font_name.c_str(), font);
            return font;
        }
    }

    throw std::runtime_error("A font for resource '" + key + "' could not be loaded.");
}

Image Resources::load_image(std::string const& key) const
{
    auto image = theme.resource_image(key);

    // load image pixels
    int w, h;
    int channels;
    uint8_t* data = stbi_load_from_memory(image.data(), image.size(), &w, &h, &channels, 4);

    // convert RGB (from STI) to BGR (expected by X) and create mask
    auto* image_data = new uint8_t[w * h * 4];  // it'll be freed by XDestroyImage

    uint8_t mask_data[w * h / 8];
    memset(mask_data, 0, sizeof mask_data);

    for (int i = 0; i < w * h * 4; i += 4) {

        image_data[i] = data[i+2];
        image_data[i+1] = data[i+1];
        image_data[i+2] = data[i];
        image_data[i+3] = 0;

        if (data[i+3] == 0xff) { // non-transparent
            mask_data[i / 4 / 8] |= (1 << ((i / 4) % 8));
        }
    }

    // create image
    XImage* ximage = XCreateImage(x11.display, x11.visual, x11.depth, ZPixmap, 0, (char *) image_data, w, h, 32, 0);
    Pixmap pixmap = XCreatePixmap(x11.display, x11.root, w, h, x11.depth);
    XPutImage(x11.display, pixmap, x11.gc, ximage, 0, 0, 0, 0, w, h);

    XDestroyImage(ximage);
    free(data);

    // create mask
    Pixmap mask = XCreatePixmapFromBitmapData(x11.display, x11.root, (char *) mask_data, w, h, x11.white, x11.black, 1);

    LOG.debug("Image created for '%s'", key.c_str());

    return { .pixmap = pixmap, .mask = mask };
}

template<>
void Resources::set_property(Window window, std::string const& name, Window const& value)
{
    Atom atom = XInternAtom(x11.display, name.c_str(), false);

    XChangeProperty(x11.display, window, atom, XA_WINDOW, 32, PropModeReplace,
                    (unsigned char *) &value, 1);
}

template<> Window
Resources::get_property(Window window, std::string const& name) const
{
    Atom atom = XInternAtom(x11.display, name.c_str(), false);

    Atom type;
    int format;
    unsigned long nret, left;
    unsigned char* data;
    XGetWindowProperty(x11.display, window, atom, 0, 1, False, XA_WINDOW, &type, &format, &nret, &left, &data);
    if (nret == None)
        return None;
    Window ret = ((Window *) data)[0];
    XFree(data);
    return ret;
}
