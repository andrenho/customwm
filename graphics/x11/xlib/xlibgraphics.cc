#include "xlibgraphics.hh"

#include "graphics/pencil/xlib/xlibpencil.hh"

std::unique_ptr<Pencil> XlibGraphics::create_pencil(Window_ *window)
{
    return std::make_unique<XlibPencil>(window, this);
}

void XlibGraphics::paint([[maybe_unused]] WindowHandle window, std::function<void()> paint_function)
{
    paint_function();
}
