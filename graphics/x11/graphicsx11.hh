#ifndef GRAPHICSX11_HH_
#define GRAPHICSX11_HH_

#include "../graphics.hh"
#include "resourcesx11.hh"

#include <memory>
#include <optional>
#include <string>

#include <X11/Xlib.h>

class GraphicsX11 : public Graphics {
public:
    explicit GraphicsX11(Theme& theme, std::optional<std::string> const& display);
    ~GraphicsX11() override;

    std::unique_ptr<WM> create_wm_() override;

private:
    Display* dpy_;
    std::unique_ptr<ResourcesX11> resources_;
};

#endif //GRAPHICSX11_HH_
