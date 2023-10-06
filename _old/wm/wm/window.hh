#ifndef WINDOW_HH_
#define WINDOW_HH_

#include "../../lib/types/iwindow.hh"

struct Window : public IWindow {
    Handle outer_id;
    Handle inner_id;
};

#endif //WINDOW_HH_
