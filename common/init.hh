#ifndef INIT_HH_
#define INIT_HH_

#include <memory>
#include "graphics/gwm.hh"

std::unique_ptr<GWindowManager> initialize(int argc, char* argv[]);

#endif //INIT_HH_
