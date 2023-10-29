#include "resourcesx11.hh"
#include "theme/logger.hh"

#include <X11/Xft/Xft.h>

// see: https://archive.irixnet.org/apocrypha/nekonomicon/forum/15/8794/1.html

ResourcesX11::~ResourcesX11()
{
    for (auto const& [_, font]: fonts_)
        XftFontClose(dpy_, font);
}

void ResourcesX11::load_resources(Theme& theme)
{
    load_fonts(theme);
}

void ResourcesX11::load_fonts(Theme &theme)
{
    Theme::Fonts fonts = theme.fonts();

    for (auto const& [key, font_names] : fonts) {

        bool found = false;

        for (auto const& font_name: font_names) {
            XftFont* font = XftFontOpenName(dpy_, DefaultScreen(dpy_), font_name.c_str());
            if (font) {
                fonts_.emplace(key, font);
                found = true;
                LOG.debug("Loaded font '%s' as %p", font_name.c_str(), font);
                break;
            }
        }

        if (!found) {
            throw std::runtime_error("A font for resource '" + key + "' could not be found.");
        }
    }
}