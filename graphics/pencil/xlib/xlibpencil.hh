#ifndef XLIBPENCIL_HH_
#define XLIBPENCIL_HH_

#include <X11/Xlib.h>
#include "graphics/pencil/pencil.hh"

class XlibPencil : public Pencil {
public:
    XlibPencil(class Window_* window, class XlibGraphics const* X);
    ~XlibPencil() override;

    void fill(Color const &color, Rectangle const& rect) const override;

    void on_resize(Size const& new_size);
    void on_expose(Rectangle const& rectangle) const override;

private:
    class XlibGraphics const* X;
    Pixmap backbuffer_ = None;
    GC gc_ = None;
};

#endif //XLIBPENCIL_HH_
