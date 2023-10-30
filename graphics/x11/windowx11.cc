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

void WindowX11::text(int x, int y, std::string const &text, TextProperties const& tp_)
{
    TextProperties tp = tp_;

    XftFont* font = resources_.get_font(tp.font);

    XGlyphInfo glyph_info;
    XftTextExtentsUtf8(dpy_, font, (FcChar8 const *) text.c_str(), (int) text.size(), &glyph_info);

    if (tp.w == 0)
        tp.w = glyph_info.width;
    if (tp.h == 0)
        tp.h = glyph_info.height;

    if (tp.halign == TextProperties::HCenter) {
        x += ((int32_t) tp.w / 2) - (glyph_info.width / 2);
    } else if (tp.halign == TextProperties::Right) {
        x += (int32_t) tp.w - glyph_info.width;
    }

    XftDrawStringUtf8(xft_draw_, &(resources_.get_xft_color(tp.color)), font, x, y,
                      (FcChar8 const *) text.c_str(), (int) text.size());
}
