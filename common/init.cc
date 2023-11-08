#include "init.hh"

#include "options.hh"
#include "theme/theme.hh"

#if GRAPHICS==X11
#  include "graphics/x11/x11.hh"
#  include "graphics/x11/wm.hh"
#endif

#include "base_theme.embed"

WM initialize(int argc, char* argv[])
{
    Options options(argc, argv);

    if (!options.throw_exceptions)
        THEME.set_error_action(ErrorAction::ERROR);

#ifdef DEBUG
    THEME.load_theme_file("./common/base_theme.lua");
#else
    THEME.load_theme(base_theme_lua);
#endif

    if (options.theme_file)
        THEME.load_theme_file(*options.theme_file);

#if GRAPHICS==X11
    x11.init(options.display);
#endif

    WM wm;
    THEME.create_global_object("wm", &wm);

    if (!options.throw_exceptions)
        THEME.set_error_action(ErrorAction::LOG);

    return wm;
}
