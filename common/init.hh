#ifndef INIT_HH_
#define INIT_HH_

#include <memory>
#include "graphics/wm.hh"

std::unique_ptr<WindowManager> initialize(int argc, char* argv[]);

#endif //INIT_HH_
