#include <memory>

#if (BACKEND == X11)
#  include "../libroot/x11/root_x11.hh"
#include "options.hh"

#elif (BACKEND == WAYLAND)
#  include "../libroot/wayland/rootwayland.hh"
#endif

#include <iostream>

int main(int argc, char* argv[])
{
    Options options(argc, argv);

    // TODO - load theme

#if (BACKEND == X11)
    std::unique_ptr<Root> root = std::make_unique<RootX11>(options.display);
#elif (BACKEND == WAYLAND)
    std::unique_ptr<Root> root = std::make_unique<RootWayland>();
#endif

    std::cout << "Backend: " << root->interface_name() << "...\n";
    std::unique_ptr<WM> wm = root->create_wm();
    wm->run();
}