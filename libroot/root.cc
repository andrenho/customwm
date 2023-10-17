#include "root.hh"

#if (BACKEND == X11)
#  include "x11/root_x11.hh"
#elif (BACKEND == WAYLAND)
#  include "wayland/rootwayland.hh"
#endif

std::unique_ptr<Root> Root::build([[maybe_unused]] std::optional<std::string> const& display)
{
#if (BACKEND == X11)
    return std::make_unique<RootX11>(display);
#elif (BACKEND == WAYLAND)
    return std::make_unique<RootWayland>();
#endif
}
