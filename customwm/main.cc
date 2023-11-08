#include "common/init.hh"

int main(int argc, char* argv[])
{
    std::unique_ptr<WindowManager> wm = initialize(argc, argv);
    wm->run();
}
