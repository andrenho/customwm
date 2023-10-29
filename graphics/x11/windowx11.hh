#ifndef WINDOWX11_HH_
#define WINDOWX11_HH_

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

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

    void text(int x, int y, std::string const &text, std::string const &font_key, Color const &color) override;

private:
    Display* dpy_;
    ResourcesX11& resources_;
    Colormap colormap_;
    GC gc_;
    XftDraw* xft_draw_ = nullptr;
    std::map<Color, unsigned long> colors_;
    std::map<Color, XftColor>      xft_colors_;

    unsigned long get_color(Color const &color);
    XftColor&     get_xft_color(Color const& color);
};

#endif //WINDOWX11_HH_
