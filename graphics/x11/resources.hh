#ifndef RESOURCES_HH_
#define RESOURCES_HH_

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

class Resources {
public:
    Resources();
    ~Resources();

    unsigned long get_color(Color const &color) const;
    XftColor&     get_xft_color(Color const& color) const;
    XftFont*      get_font(std::string const& key) const;
    std::pair<Image, Rectangle> get_slice_image(std::string const& slice_name) const;
    Cursor        get_cursor(std::string const& key) const;

    template<typename T> void set_property(Window window, std::string const& name, T const& value);
    template<typename T> T    get_property(Window window, std::string const& name) const;

private:
    mutable std::map<Color, unsigned long>            colors_;
    mutable std::map<Color, XftColor>                 xft_colors_;
    mutable std::unordered_map<std::string, XftFont*> fonts_;
    mutable std::unordered_map<std::string, Image>    images_;
    mutable std::unordered_map<std::string, Cursor>   cursors_;

    XftFont* load_font(std::string const& key) const;
    Image    load_image(std::string const& key) const;

    template<> void   set_property(Window window, std::string const& name, Window const& value);
    template<> Window get_property(Window window, std::string const& name) const;
};

#endif //RESOURCES_HH_
