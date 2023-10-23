#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <thread>

#include "../libtheme/theme.hh"
#include "options.hh"
#include "wm.hh"

int main(int argc, char* argv[])
{
    srand(time(nullptr));

    Options options(argc, argv);
    if (!options.display) {
        fprintf(stderr, "%s: no display is set\n", argv[0]);
        exit(EXIT_FAILURE);
    }

start:
   try {
       Theme theme;

       if (options.theme_file)
           theme.load_from_file(*options.theme_file);

       WM(*options.display, theme).run();

    } catch (RestartException&) {
        goto start;

    } catch (LuaException& e) {
       fprintf(stderr, "lua error: %s\n", e.what());
       exit(EXIT_FAILURE);
   }
}