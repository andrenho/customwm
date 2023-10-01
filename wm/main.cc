#include "../lib/customwmlib.hh"

#include <cstdio>
#include <variant>

#include "wm.hh"

int main(int argc, char* argv[])
{
    CustomWMLib lib(argc, argv);
    Window w;
    WM(lib).run();
}