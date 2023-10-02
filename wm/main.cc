#include "../lib/customwmlib.hh"
#include "wm/wm.hh"

#include <ctime>
#include <cstdlib>

#include <memory>

int main(int argc, char* argv[])
{
    srand(time(nullptr));
    CustomWMLib lib(argc, argv);
    WM(lib).run();
}