#ifndef XGRAPHICS_HH_
#define XGRAPHICS_HH_

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>

#include <optional>
#include <string>
#include "graphics/graphics.hh"

#include <optional>

struct XGraphics : public Graphics {
    explicit XGraphics(std::optional<std::string> const& display);
    ~XGraphics() override;

    Display*      display;
    int           screen;
    Window        root;
    Visual*       visual;
    GC            gc;
    Colormap      colormap;
    int           depth;
    unsigned long black;
    unsigned long white;

    std::unique_ptr<WindowManager> create_wm() override;
};

extern XGraphics* X;

#endif //XGRAPHICS_HH_
