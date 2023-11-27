#include "xlibgraphics.hh"

#include "graphics/pencil/xlib/xlibpencil.hh"

std::unique_ptr<Pencil> XlibGraphics::create_pencil(Window_ *window)
{
    return std::make_unique<XlibPencil>(window, this);
}
