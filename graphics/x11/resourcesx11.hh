#ifndef RESOURCESX11_HH_
#define RESOURCESX11_HH_

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <unordered_map>
#include <string>
#include "theme/theme.hh"
#include "theme/types/types.hh"

struct Image {
    Pixmap pixmap;
    Pixmap mask;
};

class ResourcesX11 {
public:
    ResourcesX11(Display* dpy, Theme& theme);
    ~ResourcesX11();

    unsigned long get_color(Color const &color) const;
    XftColor&     get_xft_color(Color const& color) const;
    XftFont*      get_font(std::string const& key) const;
    std::pair<Image, Rectangle> get_slice_image(std::string const& slice_name) const;

private:
    mutable std::map<Color, unsigned long>            colors_;
    mutable std::map<Color, XftColor>                 xft_colors_;
    mutable std::unordered_map<std::string, XftFont*> fonts_;
    mutable std::unordered_map<std::string, Image>    images_;

    Display* dpy_;
    Theme& theme_;

    XftFont* load_font(std::string const& key) const;
    Image    load_image(std::string const& key) const;
};

#endif //RESOURCESX11_HH_
