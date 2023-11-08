#include "common/init.hh"

int main(int argc, char* argv[])
{
    WM wm = initialize(argc, argv);
    wm.run();
}
