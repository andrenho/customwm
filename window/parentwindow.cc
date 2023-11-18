#include "parentwindow.hh"

#include "graphics/graphics.hh"

void ParentWindow::reparent_child(WindowHandle child_handle, Point const& offset)
{
    graphics_->reparent_window(handle_, child_handle, offset);
    child_handle_ = child_handle;
}

ParentWindow::~ParentWindow()
{
    if (child_handle_ != 0)
        graphics_->unparent_window(child_handle_);
}
