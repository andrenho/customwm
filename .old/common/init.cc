#include "init.hh"

#include "../../customwm/config/config.hh"
#include ".old/theme/theme.hh"

#include ".old/graphics/graphics.hh"
#include "logger.hh"

std::unique_ptr<GWindowManager> initialize(int argc, char* argv[])
{
    Options options(argc, argv);

    if (!options.throw_exceptions)
        THEME.set_error_action(ErrorAction::ERROR);

    if (options.theme_file)
        THEME.load_theme_file(*options.theme_file);

#if 0
    if (LOG.level == Logger::Debug)
        THEME.print_effective_theme();
#endif

    Graphics* graphics = Graphics::create(options.display);

    std::unique_ptr<GWindowManager> wm = graphics->create_wm();
    THEME.create_global_object("wm", wm.get());

    if (!options.throw_exceptions)
        THEME.set_error_action(ErrorAction::LOG);

    return wm;
}
