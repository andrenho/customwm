#include "theme/theme.hh"
#include "options.hh"

#include "customwm.embed"

#if GRAPHICS==X11
#  include "graphics/x11/x11.hh"
#  include "graphics/x11/wm.hh"
#endif

#include <memory>

int main(int argc, char* argv[])
{
    Options options(argc, argv);

#ifdef DEBUG
    theme.load_theme_file("./customwm/customwm.lua");
#else
    theme.load_theme(customwm_lua);
#endif

    if (options.theme_file)
        theme.load_theme_file(*options.theme_file);

#if GRAPHICS==X11
    x11.init(options.display);
#endif

    WM wm;
    theme.create_global_object("wm", &wm);
    wm.run();
}
