#include "resourcesx11.hh"
#include "theme/logger.hh"

#include <X11/Xft/Xft.h>

// see: https://archive.irixnet.org/apocrypha/nekonomicon/forum/15/8794/1.html

ResourcesX11::~ResourcesX11()
{
    for (auto const& [_, font]: fonts_)
        XftFontClose(dpy_, font);
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