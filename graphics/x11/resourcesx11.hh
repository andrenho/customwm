#ifndef RESOURCESX11_HH_
#define RESOURCESX11_HH_

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <unordered_map>
#include <string>
#include "theme/theme.hh"

class ResourcesX11 {
public:
    explicit ResourcesX11(Display* dpy, Theme& theme): dpy_(dpy), theme_(theme) {}
    ~ResourcesX11();

    XftFont* get_font(std::string const& key) const;

private:
    mutable std::unordered_map<std::string, XftFont*> fonts_;
    Display* dpy_;
    Theme& theme_;

    XftFont* load_font(std::string const& key) const;
};

#endif //RESOURCESX11_HH_
