#ifndef GRAPHICSX11_HH_
#define GRAPHICSX11_HH_

#include "graphics/graphics.hh"

#include <X11/Xlib.h>

class GraphicsX11 : public Graphics {
public:
    explicit GraphicsX11(class Options *options) : Graphics(options) {}
    ~GraphicsX11() override;

    void init() override;
    std::string interface_name() const override { return "X11"; }

    void subscribe_to_wm_events() override;

    std::optional<Event> next_event() override;

private:
    Display*      display  = nullptr;
    int           screen   = 0;
    Window        root     = None;
    Visual*       visual   = nullptr;
    GC            gc       = None;
    Colormap      colormap = None;
    int           depth    = 0;
    unsigned long black    = 0;
    unsigned long white    = 0;
};

#endif //GRAPHICSX11_HH_
