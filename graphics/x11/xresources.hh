#ifndef XRESOURCES_HH_
#define XRESOURCES_HH_

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <unordered_map>
#include <string>
#include "theme/theme.hh"
#include "theme/types/types.hh"
#include "graphics/resources.hh"

struct Image {
    Pixmap pixmap;
    Pixmap mask;
};

class XResources : public Resources {
public:
    XResources();
    ~XResources();

    unsigned long get_color(Color const &color) const;
    XftColor&     get_xft_color(Color const& color) const;
    XftFont*      get_font(std::string const& key) const;
    std::pair<Image, Rectangle> get_slice_image(std::string const& slice_name) const;
    Cursor        get_cursor(std::string const& key) const;

    void set_property(WHandle window, std::string const &name, WHandle const &value) override;
    std::optional<WHandle> get_property_whandle(WHandle window, std::string const &name) override;

private:
    mutable std::map<Color, unsigned long>            colors_;
    mutable std::map<Color, XftColor>                 xft_colors_;
    mutable std::unordered_map<std::string, XftFont*> fonts_;
    mutable std::unordered_map<std::string, Image>    images_;
    mutable std::unordered_map<std::string, Cursor>   cursors_;

    XftFont* load_font(std::string const& key) const;
    Image    load_image(std::string const& key) const;
};

#endif //XRESOURCES_HH_
