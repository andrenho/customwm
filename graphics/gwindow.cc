#include "gwindow.hh"

#include "gwm.hh"

void GWindow::set_child(WHandle child_id, Padding const &padding)
{
    child_id_ = child_id;
    child_padding_ = padding;
}

void GWindow::maximize_restore()
{
    if (window_state_ != WindowState::Maximized) {
        saved_rectangle_ = rectangle_;
        window_state_ = WindowState::Maximized;
        move({ 0, 0 });  // TODO - usable screen
        resize(wm_->usable_screen_size());
    } else {
        resize(saved_rectangle_.size());
        move(saved_rectangle_.topleft());
        window_state_ = WindowState::Normal;
    }
}

bool GWindow::focused() const
{
    return wm_->focus_manager().is_window_focused(this);
}

