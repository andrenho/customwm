#include "xwindow.hh"

#include "common/logger.hh"
#include "x.hh"
#include "xwm.hh"

XWindow::XWindow(XWindowManager const& wm, XResources const& resources, Rectangle const &rectangle)
        : rectangle(rectangle), wm_(wm), resources_(resources)
{
    id_ = XCreateWindow(X->display, X->root, rectangle.x, rectangle.y, rectangle.w, rectangle.h, 0,
                        CopyFromParent, InputOutput, CopyFromParent, 0, nullptr);

    XSelectInput(X->display, id_, SubstructureNotifyMask | StructureNotifyMask | ExposureMask |
                                  ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

    // redirect events to window manager
    XGrabButton(X->display, AnyButton, AnyModifier, id_, false, ButtonPressMask, GrabModeSync, GrabModeAsync, None, None);

    XMapWindow(X->display, id_);

    gc_ = XCreateGC(X->display, id_, 0, nullptr);
    xft_draw_ = XftDrawCreate(X->display, id_, X->visual, X->colormap);

    XDefineCursor(X->display, id_, resources_.get_cursor(Cursors::Pointer));
}

XWindow::~XWindow()
{
    XUnmapWindow(X->display, id_);

    XftDrawDestroy(xft_draw_);
    XFreeGC(X->display, gc_);
    XDestroyWindow(X->display, id_);
}

void XWindow::fill(Color const &color)
{
    XSetForeground(X->display, gc_, resources_.get_color(color));
    XFillRectangle(X->display, id_, gc_, 0, 0, rectangle.w, rectangle.h);
}

void XWindow::text(int x, int y, std::string const &text_, TextProperties const& tp_)
{
    TextProperties tp = tp_;
    std::string text = text_;

try_again:
    if (text.empty())
        return;

    XftFont* font = resources_.get_font(tp.font);

    XGlyphInfo glyph_info;
    XftTextExtentsUtf8(X->display, font, (FcChar8 const *) text.c_str(), (int) text.size(), &glyph_info);

    if (tp.w == 0)
        tp.w = glyph_info.width;
    if (tp.h == 0)
        tp.h = glyph_info.height;

    if (tp.w < glyph_info.width) {
        if (tp.overflow == TextProperties::Hidden) {
            text = text.substr(0, text.length() - 1);
            goto try_again;
        } else if (tp.overflow == TextProperties::Ellipsis) {
            text = text.substr(0, text.length() - (text.ends_with("...") ? 4 : 1)) + "...";
            goto try_again;
        }
    }

    if (tp.halign == TextProperties::HCenter) {
        x += ((int32_t) tp.w / 2) - (glyph_info.width / 2);
    } else if (tp.halign == TextProperties::Right) {
        x += (int32_t) tp.w - glyph_info.width;
    }

    if (tp.valign == TextProperties::Top) {
        y += glyph_info.height;
    } else if (tp.valign == TextProperties::VCenter) {
        y += (glyph_info.height / 2) + ((int32_t) tp.h / 2);
    } else if (tp.valign == TextProperties::Bottom) {
        y += (int32_t) tp.h;
    }

    XftDrawStringUtf8(xft_draw_, &(resources_.get_xft_color(tp.color)), font, x, y,
                      (FcChar8 const *) text.c_str(), (int) text.size());
}

void XWindow::draw(int x, int y, std::string const &slice)
{
    auto [image, rect] = resources_.get_slice_image(slice);
    XSetClipMask(X->display, gc_, image.mask);
    XSetClipOrigin(X->display, gc_, -rect.x + x, -rect.y + y);
    XCopyArea(X->display, image.pixmap, id_, gc_, rect.x, rect.y, rect.w, rect.h, x, y);
    XSetClipMask(X->display, gc_, None);
}

std::string XWindow::name() const
{
    auto child = resources_.get_property_whandle(id_, "child");
    if (child.has_value()) {
        XTextProperty p;
        if ((XGetWMName(X->display, *child, &p) == 0) || p.value == nullptr)
            goto not_found;

        return (char *) p.value;
    }

not_found:
    return LWindow::name();
}

void XWindow::set_cursor(std::string const &key)
{
    XDefineCursor(X->display, id_, resources_.get_cursor(key));
}

bool XWindow::focused() const
{
    return wm_.focus_manager().is_window_focused(this);
}
