#include <cstdio>
#include <cstdlib>

#include <memory>

#if (BACKEND == X11)
#  include "../libroot/x11/root_x11.hh"
#include "options.hh"
#include "../libtheme/theme.hh"
#include "windowmanager.hh"

#elif (BACKEND == WAYLAND)
#  include "../libroot/wayland/rootwayland.hh"
#endif

int main(int argc, char* argv[])
{
    Options options(argc, argv);

    Theme theme;
    // TODO - load theme

    std::unique_ptr<Root> root;
    try {
#if (BACKEND == X11)
        root = std::make_unique<RootX11>(options.display);
#elif (BACKEND == WAYLAND)
        root = std::make_unique<RootWayland>();
#endif
        printf("Backend: %s\n", root->interface_name().c_str());
    } catch (std::exception& e) {
        fprintf(stderr, "Error initializing customwm: %s\n", e.what());
        exit(EXIT_FAILURE);
    }

    WindowManager wm(theme, *root);
    wm.run_event_loop();
}