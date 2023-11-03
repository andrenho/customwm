#ifndef X11_HH_
#define X11_HH_

#include <X11/Xlib.h>

#include <optional>
#include <string>

struct X11 {
    Display*      display;
    int           screen;
    Window        root;
    Visual*       visual;
    GC            gc;
    Colormap      colormap;
    int           depth;
    unsigned long black;
    unsigned long white;

    void init(std::optional<std::string> const& display);
    ~X11();
};

extern X11 x11;

#endif //X11_HH_
