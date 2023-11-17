/*
#include "customwm/config/config.hh"
#include "theme/theme.hh"
 */

#include <memory>
#include "options/options.hh"

int main(int argc, char* argv[])
{
    // initialize services
    auto config = std::make_unique<Options>(argc, argv);
    /*
    auto theme = std::make_unique<Theme>();

    // startup services
    config->init(argc, argv);
     */
}