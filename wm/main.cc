#include "config.hh"
#include "wm.hh"

#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    Config config(argc, argv);

    WM wm(config);

    try {
        wm.start();
    } catch (std::exception& e) {
        std::cerr << "Error starting customwm: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    wm.run();
}
