#ifndef WM_HH_
#define WM_HH_


#include "../lib/customwmlib.hh"
#include "windows.hh"

class WM {
public:
    explicit WM(CustomWMLib const& lib) : lib_(lib) {}

    void run();

protected:
    virtual void setup() = 0;
    virtual void loop() = 0;

    Point window_starting_pos(Window const& w, int16_t win_x, int16_t win_y, int16_t win_w, int16_t win_h, int16_t scr_w, int16_t scr_h) const;

    CustomWMLib const& lib_;
    Windows windows_;
};

#endif //WM_HH_
