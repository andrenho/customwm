#include "../lib/customwmlib.hh"

#include <ctime>
#include <cstdlib>

#include <memory>

#include "wm_x11.hh"

int main(int argc, char* argv[])
{
    srand(time(nullptr));
    CustomWMLib lib(argc, argv);
    WM_X11(lib).run();
}