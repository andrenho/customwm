#include "xwindow.hh"

#include ".old/common/logger.hh"
#include "x.hh"
#include "xwm.hh"

XWindow::XWindow(XWindowManager const& wm, XResources const& resources, Rectangle const &rectangle)
        : GWindow((GWindowManager *) &wm, rectangle), xwm_(wm), resources_(resources)
{
    id_ = XCreateWindow(X->display, X->root, rectangle.x, rectangle.y, rectangle.w, rectangle.h, 0,
                        CopyFromParent, InputOutput, CopyFromParent, 0, nullptr);

    backbuffer_ = XCreatePixmap(X->display, id_, rectangle.w, rectangle.h, X->depth);

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
    XFreePixmap(X->display, backbuffer_);
    XFreeGC(X->display, gc_);
    XDestroyWindow(X->display, id_);
}

void XWindow::fill(Color const &color, std::optional<Rectangle> rect)
{
    XSetForeground(X->display, gc_, resources_.get_color(color));
    if (rect) {
        XFillRectangle(X->display, id_, gc_, rect->x, rect->y, rect->w, rect->h);
    } else {
        XFillRectangle(X->display, id_, gc_, 0, 0, rectangle_.w, rectangle_.h);
    }
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

Rectangle XWindow::rect(bool update_cache) const
{
    if (update_cache) {
        XWindowAttributes xwa;
        XGetWindowAttributes(X->display, id_, &xwa);
        rectangle_ = Rectangle { xwa.x, xwa.y, xwa.width, xwa.height };
    }

    return rectangle_;
}

std::string XWindow::name() const
{
    auto child = child_id();
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

std::vector<std::string> XWindow::child_protocols() const
{
    std::vector<std::string> protocol_list;

    Atom* protocols;
    int n_protocols = 0;
    auto child_id_ = child_id();
    if (child_id_) {
        if (XGetWMProtocols(X->display, *child_id_, &protocols, &n_protocols) != 0) {
            char* names[n_protocols];
            if (XGetAtomNames(X->display, protocols, n_protocols, names)) {
                for (int i = 0; i < n_protocols; ++i)
                    protocol_list.emplace_back(names[i]);
        }
            XFree(protocols);
        }
    }

    return protocol_list;
}

void XWindow::move(Point const &new_pos)
{
    XMoveWindow(X->display, id_, new_pos.x, new_pos.y);
}

void XWindow::resize(Size const& new_size)
{
    auto child = child_id();
    if (child) {
        XResizeWindow(X->display, *child,
                      new_size.w - (child_padding_.left + child_padding_.right) - 1,
                      new_size.h - (child_padding_.top + child_padding_.bottom) - 1);
    }

    XResizeWindow(X->display, id_, new_size.w, new_size.h);
}

void XWindow::update_backbuffer_size(Size const& sz)
{
    if (rectangle_.size() != sz) {
        XFreePixmap(X->display, backbuffer_);
        XCreatePixmap(X->display, id_, sz.w, sz.h, X->depth);
    }
}

void XWindow::update_backbuffer(Rectangle const& rectangle)
{
    XCopyArea(X->display, id_, backbuffer_, X->gc, rectangle.x, rectangle.y, rectangle.w, rectangle.h,
              rectangle.x, rectangle.y);
}

void XWindow::expose_from_backbuffer(Rectangle const& rectangle)
{
    XCopyArea(X->display, backbuffer_, id_, X->gc, rectangle.x, rectangle.y, rectangle.w, rectangle.h,
              rectangle.x, rectangle.y);
}

void XWindow::close()
{
    auto protocols = child_protocols();
    WHandle target = child_id_.value_or(id_);

    if (std::find(protocols.begin(), protocols.end(), "WM_DELETE_WINDOW") != protocols.end()) {
        XEvent e {
                .xclient {
                        .type = ClientMessage,
                        .window = target,
                        .message_type = XInternAtom(X->display, "WM_PROTOCOLS", true),
                        .format = 32,
                }
        };
        e.xclient.data.l[0] = (long) (XInternAtom(X->display, "WM_DELETE_WINDOW", false));
        e.xclient.data.l[1] = CurrentTime;
        XSendEvent(X->display, target, false, NoEventMask, &e);
    } else {
        XDestroyWindow(X->display, target);
    }
}

void XWindow::bring_to_front()
{
    XRaiseWindow(X->display, id_);
}
