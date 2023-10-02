#include "../lib/customwmlib.hh"

#include <ctime>
#include <cstdlib>

#include <memory>

#include "wm_x_11.hh"

std::unique_ptr<CustomWMLib> lib;

int main(int argc, char* argv[])
{
    srand(time(nullptr));

    lib = std::make_unique<CustomWMLib>(argc, argv);

    signal(SIGHUP, [](int signo) { if (signo == SIGHUP) lib->reload_theme(); });

    WM_X11(*lib).run();
}