#include "init.hh"

#include "options.hh"
#include "theme/theme.hh"

#include "graphics/graphics.hh"

std::unique_ptr<WindowManager> initialize(int argc, char* argv[])
{
    Options options(argc, argv);

    if (!options.throw_exceptions)
        THEME.set_error_action(ErrorAction::ERROR);

    if (options.theme_file)
        THEME.load_theme_file(*options.theme_file);

    Graphics* graphics = Graphics::create(options.display);

    std::unique_ptr<WindowManager> wm = graphics->create_wm();
    THEME.create_global_object("wm", wm.get());

    if (!options.throw_exceptions)
        THEME.set_error_action(ErrorAction::LOG);

    return wm;
}
