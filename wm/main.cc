#include "../lib/customwmlib.hh"

#include <cstdio>
#include <variant>

#include "wm_x_11.hh"

int main(int argc, char* argv[])
{
    CustomWMLib lib(argc, argv);
    WM_X11(lib).run();
}