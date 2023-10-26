#ifndef WMWINDOW_HH_
#define WMWINDOW_HH_

#include <X11/Xlib.h>
#include "theme/types/l_window.hh"

#include <map>

class WM_Window : public L_Window {
public:
    WM_Window(Display* dpy_, Window parent_id, Window child_id, Rectangle const &rectangle);
    ~WM_Window() override;

    WM_Window(WM_Window const&) = delete;
    WM_Window& operator=(WM_Window const&) = delete;
    WM_Window(WM_Window&&) = delete;
    WM_Window& operator=(WM_Window&&) = delete;

    Window parent_id;
    Window child_id;
    Rectangle rectangle;

    Rectangle rect() const override { return rectangle; }
    void fill(Color const &color) override;

private:
    Display* dpy_;
    Colormap colormap_;
    GC gc_;
    std::map<Color, unsigned long> colors_;

    unsigned long get_color(Color const &color);
};

#endif //WMWINDOW_HH_
