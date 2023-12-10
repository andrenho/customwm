#include "xlibpencil.hh"
#include "window/window.hh"
#include "graphics/x11/xlib/xlibgraphics.hh"

XlibPencil::XlibPencil(Window_* window, XlibGraphics const* X)
        : Pencil(window, X), X(X)
{
    gc_ = XCreateGC(X->display, window->handle(), 0, nullptr);
    on_resize(window->rectangle().size());
}

XlibPencil::~XlibPencil()
{
    XFreeGC(X->display, gc_);
}

void XlibPencil::fill(Color const &color, Rectangle const& rect) const
{
    XSetForeground(X->display, gc_, X->resources_.get_color(color));
    XFillRectangle(X->display, backbuffer_, gc_, rect.x, rect.y, rect.w, rect.h);
}

void XlibPencil::on_resize(Size const &new_size)
{
    if (backbuffer_ != None)
        XFreePixmap(X->display, backbuffer_);
    backbuffer_ = XCreatePixmap(X->display, window_->handle(), new_size.w, new_size.h, X->depth);
}

void XlibPencil::on_expose(Rectangle const &rectangle) const
{
    XCopyArea(X->display, backbuffer_, window_->handle(), gc_, rectangle.x, rectangle.y, rectangle.w, rectangle.h,
              rectangle.x, rectangle.y);
}

