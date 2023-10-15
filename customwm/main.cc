#include <memory>

#if (BACKEND == X11)
#  include "../libroot/x11/root_x11.hh"
#include "options.hh"
#include "../libtheme/theme.hh"
#include "windowmanager.hh"

#elif (BACKEND == WAYLAND)
#  include "../libroot/wayland/rootwayland.hh"
#endif

#include <iostream>

int main(int argc, char* argv[])
{
    Options options(argc, argv);

    Theme theme;
    // TODO - load theme

#if (BACKEND == X11)
    std::unique_ptr<Root> root = std::make_unique<RootX11>(options.display);
#elif (BACKEND == WAYLAND)
    std::unique_ptr<Root> root = std::make_unique<RootWayland>();
#endif
    std::cout << "Backend: " << root->interface_name() << "\n";

    WindowManager wm(theme, *root);
    wm.run_event_loop();
}