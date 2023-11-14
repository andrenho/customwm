#ifndef GWINDOW_HH_
#define GWINDOW_HH_

#include "theme/types/lwindow.hh"

class GWindow : public LWindow {
public:
    GWindow(class WindowManager* wm) : wm_(wm) {}

    void set_child(WHandle child_id, Padding const& padding);
    void update_rectangle(Rectangle const& r) { rectangle_ = r; }
    void set_state(WindowState state) { window_state_ = state; }
    virtual void bring_to_front() = 0;

    void maximize() override;

protected:
    class WindowManager*   wm_;
    mutable Rectangle      rectangle_ { 0, 0, 0, 0 };
    Rectangle              saved_rectangle_;
    WindowState            window_state_;
};

#endif //GWINDOW_HH_
