#ifndef RESOURCESX11_HH_
#define RESOURCESX11_HH_

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <unordered_map>
#include <string>

class ResourcesX11 {
public:
    ResourcesX11(Display* dpy): dpy_(dpy) {}
    ~ResourcesX11();

    XftFont* get_font(std::string const& name);

private:
    std::unordered_map<std::string, XftFont*> fonts_;

    Display* dpy_;
};

#endif //RESOURCESX11_HH_
