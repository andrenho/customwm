#include "window.hh"

#include "graphics/graphics.hh"

Window_::Window_(Graphics *graphics, Rectangle const &rectangle)
        : graphics_(graphics), handle_(graphics_->create_window(rectangle))
{
}

Window_::~Window_()
{
    graphics_->destroy_window(handle_);
}
