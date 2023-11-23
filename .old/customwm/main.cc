#include ".old/common/init.hh"

int main(int argc, char* argv[])
{
    std::unique_ptr<GWindowManager> wm = initialize(argc, argv);
    wm->run();
}
