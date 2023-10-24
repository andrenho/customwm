#include "theme/theme.hh"
#include "options.hh"

#include "customwm.embed"

int main(int argc, char* argv[])
{
    Options options(argc, argv);

    Theme theme;
    theme.load_theme(customwm_lua);

    if (options.theme_file)
        theme.load_theme_file(*options.theme_file);
}
