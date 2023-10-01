#include "wm.hh"

void WM::run()
{
    setup();
    loop();
}

Point WM::window_starting_pos(Window const& w) const
{
    return { 20, 20 };   // TODO - read form theme file
}
