#include "options.hh"
#include "../libtheme/theme.hh"

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "base/base.hh"

int main(int argc, char* argv[])
{
    srand(time(nullptr));

    Options options(argc, argv);
    if (!options.display) {
        fprintf(stderr, "%s: no display is set\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    Theme theme;
    theme.load_from_ram(base_lua, base_lua_len);
}