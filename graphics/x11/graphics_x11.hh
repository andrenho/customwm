#ifndef GRAPHICS_X11_HH_
#define GRAPHICS_X11_HH_

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>

#include <optional>
#include <string>
#include "graphics/graphics.hh"

#include <optional>

struct Graphics_X11 : public Graphics {
    explicit Graphics_X11(std::optional<std::string> const& display);
    ~Graphics_X11() override;

    Display*      display;
    int           screen;
    Window        root;
    Visual*       visual;
    GC            gc;
    Colormap      colormap;
    int           depth;
    unsigned long black;
    unsigned long white;

    std::unique_ptr<WM> create_wm() override;
};

extern Graphics_X11* X;

#endif //GRAPHICS_X11_HH_
