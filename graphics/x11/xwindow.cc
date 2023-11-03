#include "xwindow.hh"
#include "theme/logger.hh"
#include "x11.hh"

#include <stdexcept>

XWindow::XWindow(Resources& resources, Window parent_id, Window child_id, Rectangle const &rectangle)
        : parent_id(parent_id), child_id(child_id), rectangle(rectangle), resources_(resources)
{
    gc_ = XCreateGC(x11.display, parent_id, 0, nullptr);
    xft_draw_ = XftDrawCreate(x11.display, parent_id, x11.visual, x11.colormap);
}

XWindow::~XWindow()
{
    XftDrawDestroy(xft_draw_);
    XFreeGC(x11.display, gc_);
    XFlush(x11.display);
}

void XWindow::fill(Color const &color)
{
    XSetForeground(x11.display, gc_, resources_.get_color(color));
    XFillRectangle(x11.display, parent_id, gc_, 0, 0, rectangle.w, rectangle.h);
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
    XftTextExtentsUtf8(x11.display, font, (FcChar8 const *) text.c_str(), (int) text.size(), &glyph_info);

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
    XSetClipMask(x11.display, gc_, image.mask);
    XSetClipOrigin(x11.display, gc_, -rect.x + x, -rect.y + y);
    XCopyArea(x11.display, image.pixmap, parent_id, gc_, rect.x, rect.y, rect.w, rect.h, x, y);
    XSetClipMask(x11.display, gc_, None);
}

std::string XWindow::name() const
{
    XTextProperty p;
    if ((XGetWMName(x11.display, child_id, &p) == 0) || p.value == nullptr)
        return L_Window::name();

    return (char *) p.value;
}
