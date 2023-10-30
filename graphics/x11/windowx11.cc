#include "windowx11.hh"
#include "theme/logger.hh"

#include <stdexcept>

WindowX11::WindowX11(Display* dpy, ResourcesX11& resources, Window parent_id, Window child_id, Rectangle const &rectangle)
        : parent_id(parent_id), child_id(child_id), rectangle(rectangle), dpy_(dpy), resources_(resources)
{
    gc_ = XCreateGC(dpy_, parent_id, 0, nullptr);
    xft_draw_ = XftDrawCreate(dpy, parent_id, DefaultVisual(dpy, DefaultScreen(dpy)), DefaultColormap(dpy, DefaultScreen(dpy)));
}

WindowX11::~WindowX11()
{
    XftDrawDestroy(xft_draw_);
    XFreeGC(dpy_, gc_);
    XFlush(dpy_);
}

void WindowX11::fill(Color const &color)
{
    XSetForeground(dpy_, gc_, resources_.get_color(color));
    XFillRectangle(dpy_, parent_id, gc_, 0, 0, rectangle.w, rectangle.h);
    XFlush(dpy_);
}

void WindowX11::text(int x, int y, std::string const &text, TextProperties const& tp)
{
    XftFont* font = resources_.get_font(tp.font);

    XftDrawStringUtf8(xft_draw_, &(resources_.get_xft_color(tp.color)), font, x, y,
                      (FcChar8 const *) text.c_str(), (int) text.size());
}
