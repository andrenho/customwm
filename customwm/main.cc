#include <memory>
#include "options/options.hh"
#include "theme/theme.hh"
#include "graphics/x11/xgraphics.hh"
#include "windowmanager/windowmanager.hh"
#include "windowmanager/wmevents.hh"

int main(int argc, char* argv[])
{
    // initialize services
    auto config = std::make_unique<Options>(argc, argv);
    auto theme = std::make_unique<Theme>(config.get());

    std::unique_ptr<Graphics> graphics = std::make_unique<XGraphics>(config.get(), theme.get());

    auto window_manager = std::make_unique<WindowManager>(theme.get(), graphics.get());
    auto events = std::make_unique<WMEvents>(theme.get(), graphics.get(), window_manager.get());

    WindowManager::create_lua_metatable(theme.get());
    Window_::create_lua_metatable(theme.get());

    // startup services
    theme->init();
    graphics->init();
    events->init();
    window_manager->init();

    // run
    events->run();
}