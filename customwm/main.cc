#include <cstdio>
#include <cstdlib>

#include <memory>

#ifdef BACKEND_X11
#  include "../libroot/x11/root_x11.hh"
#endif
#ifdef BACKEND_WAYLAND
#  include "../libroot/wayland/root_wayland.hh"
#endif

#include "options.hh"
#include "../libtheme/theme.hh"
#include "windowmanager.hh"

int main(int argc, char* argv[])
{
    Options options(argc, argv);

    Theme theme;
    // TODO - load theme

    std::unique_ptr<Root> root;
    try {
#ifdef BACKEND_X11
        root = std::make_unique<RootX11>(options.display);
#endif
#ifdef BACKEND_WAYLAND
        root = std::make_unique<RootWayland>(options.display);
#endif
        printf("Backend: %s\n", root->interface_name().c_str());
    } catch (std::exception& e) {
        fprintf(stderr, "Error initializing customwm: %s\n", e.what());
        exit(EXIT_FAILURE);
    }

    WindowManager wm(theme, *root);
    wm.run_event_loop();
}