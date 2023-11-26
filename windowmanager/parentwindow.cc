#include "parentwindow.hh"

#include "graphics/graphics.hh"

void ParentWindow::reparent_child(WindowHandle child_handle, Point const& offset, Padding const& padding)
{
    graphics_->reparent_window(handle_, child_handle, offset);
    child_handle_ = child_handle;
    child_padding_ = padding;
}