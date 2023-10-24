#include "theme/theme.hh"
#include "options.hh"

#include "customwm.embed"
#include "graphics/graphics.hh"
#if GRAPHICS==X11
#  include "graphics/x11/graphicsx11.hh"
#endif

#include <memory>

int main(int argc, char* argv[])
{
    Options options(argc, argv);

    Theme theme;
    theme.load_theme(customwm_lua);

    if (options.theme_file)
        theme.load_theme_file(*options.theme_file);

    std::unique_ptr<Graphics> graphics;
#if GRAPHICS==X11
    graphics = std::make_unique<GraphicsX11>(options.display);
#endif

    WM* wm = graphics->create_wm();
    wm->run();
}
