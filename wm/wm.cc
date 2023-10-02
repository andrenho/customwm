#include "wm.hh"

void WM::run()
{
    setup();
    loop();
}

Point WM::window_starting_pos(Window const& w, int16_t win_x, int16_t win_y, int16_t win_w, int16_t win_h, int16_t scr_w, int16_t scr_h) const
{
    WindowStartingPos wsp = lib_.theme().window_starting_pos(w);
    switch (wsp.starting_pos) {
        case WindowStartingPos::Cascade:
            throw std::runtime_error("Not implemented");  // TODO
        case WindowStartingPos::Center:
            throw std::runtime_error("Not implemented");  // TODO
        case WindowStartingPos::Random:
            throw std::runtime_error("Not implemented");  // TODO
        case WindowStartingPos::Maximized:
            throw std::runtime_error("Not implemented");  // TODO
        case WindowStartingPos::Requested:
            throw std::runtime_error("Not implemented");  // TODO
        case WindowStartingPos::Custom:
            return wsp.point;
    }
}
