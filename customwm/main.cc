#include <memory>
#include "options/options.hh"
#include "theme/theme.hh"
#include "graphics/x11/graphicsx11.hh"
#include "windowmanager/windowmanager.hh"
#include "windowmanager/events.hh"

int main(int argc, char* argv[])
{
    // initialize services
    auto config = std::make_unique<Options>(argc, argv);
    auto theme = std::make_unique<Theme>(config.get());

    std::unique_ptr<Graphics> graphics = std::make_unique<GraphicsX11>(config.get());

    auto window_manager = std::make_unique<WindowManager>(theme.get());
    auto events = std::make_unique<Events>(theme.get(), window_manager.get());

    // startup services
    theme->init();
    graphics->init();
    window_manager->init();
    events->init();

    // run
    events->run();
}