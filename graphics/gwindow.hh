#ifndef GWINDOW_HH_
#define GWINDOW_HH_

#include "theme/types/lwindow.hh"

class GWindow : public LWindow {
public:
    GWindow(class GWindowManager* wm, Rectangle const& rectangle)
            : wm_(wm), rectangle_(rectangle) {}

    std::optional<WHandle> child_id() const override { return child_id_; }
    Padding                padding() const override { return child_padding_; }
    void                   maximize_restore() override;
    bool                   focused() const override;

    void                   set_child(WHandle child_id, Padding const& padding);
    void                   update_rectangle(Rectangle const& r) { rectangle_ = r; }
    void                   set_state(WindowState state) { window_state_ = state; }
    virtual void           bring_to_front() = 0;

protected:
    class GWindowManager*  wm_;
    std::optional<WHandle> child_id_;
    Padding                child_padding_;
    mutable Rectangle      rectangle_ { 0, 0, 0, 0 };
    Rectangle              saved_rectangle_;
    WindowState            window_state_;
};

#endif //GWINDOW_HH_
