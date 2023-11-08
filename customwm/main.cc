#include "common/init.hh"

int main(int argc, char* argv[])
{
    std::unique_ptr<WM> wm = initialize(argc, argv);
    wm->run();
}
