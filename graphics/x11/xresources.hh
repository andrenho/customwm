#ifndef XRESOURCES_HH_
#define XRESOURCES_HH_

#include <unordered_map>
#include <string>

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include "graphics/cursors.hh"
#include "graphics/windowhandle.hh"
#include "theme/types/types.hh"

struct Image {
    Pixmap pixmap;
    Pixmap mask;
};

class XResources {
public:
    XResources(class XGraphics* X, class Theme* theme)
            : X(X), theme_(theme) {}
    ~XResources();

    void          init();

    unsigned long get_color(Color const &color) const;
    XftColor&     get_xft_color(Color const& color) const;
    XftFont*      get_font(std::string const& key) const;
    std::pair<Image, Rectangle> get_slice_image(std::string const& slice_name) const;
    Cursor        get_cursor(std::string const& key) const;

    void set_property(WindowHandle window, std::string const &name, WindowHandle const &value);
    std::optional<WindowHandle> get_property_handle(WindowHandle window, std::string const &name) const;

private:
    class XGraphics* X;
    class Theme* theme_;

    mutable std::unordered_map<Color, unsigned long>  colors_;
    mutable std::unordered_map<Color, XftColor>       xft_colors_;
    mutable std::unordered_map<std::string, XftFont*> fonts_;
    mutable std::unordered_map<std::string, Image>    images_;
    mutable std::unordered_map<std::string, Cursor>   cursors_;

    XftFont* load_font(std::string const& key) const;
    Image    load_image(std::string const& key) const;
};

#endif //XRESOURCES_HH_
