#include "gwindow.hh"

#include "wm.hh"

void GWindow::set_child(WHandle child_id, Padding const &padding)
{
    child_id_ = child_id;
    child_padding_ = padding;
}

void GWindow::maximize()
{
    saved_rectangle_ = rectangle_;
    window_state_ = WindowState::Maximized;
    move({ 0, 0 });  // TODO - usable screen
    resize(wm_->usable_screen_size());
}
