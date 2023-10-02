#include "wm.hh"

void WM::run()
{
    setup();
    loop();
}

Point WM::window_starting_pos(Window const& w, int16_t win_x, int16_t win_y, int16_t win_w, int16_t win_h,
                              int16_t scr_w, int16_t scr_h, int16_t pad_x, int16_t pad_y) const
{
    WindowStartingPos wsp = lib_.theme().window_starting_pos(w);
    switch (wsp.starting_pos) {
        case WindowStartingPos::Cascade: {
            Point p = { (int16_t) (cascade_ * pad_x), (int16_t) (cascade_ * pad_y) };
            ++cascade_;
            if (cascade_ == 5)
                cascade_ = 0;
            return p;
        }
        case WindowStartingPos::Center:
            return { (int16_t) ((scr_w / 2) - (win_w / 2)), (int16_t) ((scr_h / 2) - (win_h / 2)) };
        case WindowStartingPos::Random:
            return { (int16_t) (rand() % (scr_w / 2)), (int16_t) (rand() % (scr_h / 2)) };
        case WindowStartingPos::Maximized:
            throw std::runtime_error("Not implemented");  // TODO
        case WindowStartingPos::Requested:
            throw Point { win_x, win_y };
        case WindowStartingPos::Custom:
            return wsp.point;
    }
}
