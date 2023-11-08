#include "graphics.hh"

#ifdef X11
#  include "x11/graphics_x11.hh"
#endif

#ifdef WAYLAND
#  include "wayland/graphics_wayland.hh"
#endif

std::unique_ptr<Graphics> Graphics::graphics_;

Graphics* Graphics::create(std::optional<std::string> const &display)
{
#ifdef X11
    Graphics::graphics_ = std::make_unique<Graphics_X11>(display);
    X = dynamic_cast<Graphics_X11*>(Graphics::graphics_.get());
#endif
#ifdef WAYLAND
    Graphics::graphics_ = std::make_unique<Graphics_Wayland>(display);
    W = dynamic_cast<Graphics_X11*>(Graphics::graphics_.get());
#endif
    return graphics_.get();
}

