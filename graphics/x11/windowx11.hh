#ifndef WINDOWX11_HH_
#define WINDOWX11_HH_

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <cairo.h>

#include "theme/types/l_window.hh"
#include "resourcesx11.hh"

#include <map>

class WindowX11 : public L_Window {
public:
    WindowX11(Display* dpy_, ResourcesX11& resources, Window parent_id, Window child_id, Rectangle const &rectangle);
    ~WindowX11() override;

    WindowX11(WindowX11 const&) = delete;
    WindowX11& operator=(WindowX11 const&) = delete;
    WindowX11(WindowX11&&) = delete;
    WindowX11& operator=(WindowX11&&) = delete;

    Window parent_id;
    Window child_id;
    Rectangle rectangle;

    Rectangle rect() const override { return rectangle; }
    void fill(Color const &color) override;
    void text(int x, int y, std::string const &text, TextProperties const& tp) override;
    void draw(int x, int y, std::string const &slice) override;

    void flush();

private:
    Display* dpy_;
    ResourcesX11& resources_;
    cairo_surface_t* cairo_sf;
    cairo_t* cr;
    GC gc_;
    XftDraw* xft_draw_ = nullptr;
};

#endif //WINDOWX11_HH_
