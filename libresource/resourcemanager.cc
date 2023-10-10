#include "resourcemanager.hh"

#include <algorithm>

#include <xcb/xcb_image.h>

#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "contrib/stb_image.h"

ResourceManager::~ResourceManager()
{
    for (auto const& kv: images_)
        xcb_free_pixmap(dpy_, kv.second.pixmap);
}

void ResourceManager::load_resources(Theme const& theme)
{
    try {
        for (std::string const& key : theme.keys("resources.images")) {
            load_image(key, theme.read<ImageResource>("resources.images." + key));
        }
    } catch (PropertyNotFoundException& unused) {}

    try {
        for (std::string const& key : theme.keys("resources.fonts")) {
            // load_font(key, theme.read<Font>("resources.fonts." + key));
        }
    } catch (PropertyNotFoundException& unused) {}
}

void ResourceManager::load_image(std::string const& name, ImageResource const &df)
{
    int w, h;
    int comp;
    uint8_t* data = stbi_load_from_memory((stbi_uc const *) df.data.data(), (int) df.data.length(),
                                          &w, &h, &comp, 4);

    unsigned* dp = (unsigned*) data;
    for(size_t i = 0, len = w * h; i < len; i++) //rgba to bgra
        dp[i] = dp[i] & 0xff00ff00 | ((dp[i] >> 16) & 0xFF) | ((dp[i] << 16) & 0xFF0000);

    xcb_pixmap_t px = xcb_generate_id(dpy_);
    xcb_gcontext_t gc = xcb_generate_id(dpy_);

    xcb_create_pixmap(dpy_, scr_->root_depth, px, scr_->root, w, h);
    xcb_create_gc(dpy_, gc, px, 0, nullptr);

    xcb_image_t* image = xcb_image_create_native(dpy_, w, h, XCB_IMAGE_FORMAT_Z_PIXMAP, scr_->root_depth, data, w * h * 4, data);
    xcb_image_put(dpy_, px, gc, image, 0, 0, 0);

    xcb_image_destroy(image);
    xcb_free_gc(dpy_, gc);

    images_.emplace(name, Image { px, df.slices });
}

std::pair<xcb_pixmap_t, Rectangle> ResourceManager::image(
        std::string const& image, std::string const& slice) const
{
    auto it_img = images_.find(image);
    if (it_img == images_.end())
        throw ResourceNotFound("Image '" + image + "' not found.");

    auto& slices = it_img->second.slices;
    auto it_slc = slices.find(slice);
    if (it_slc == slices.end())
        throw ResourceNotFound("Slice '" + slice + "' for image '" + image + "' not found.");

    return {it_img->second.pixmap, it_slc->second };
}
