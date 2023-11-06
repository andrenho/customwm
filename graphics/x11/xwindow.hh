#ifndef XWINDOW_HH_
#define XWINDOW_HH_

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include "theme/types/l_window.hh"
#include "resources.hh"

#include <map>

class XWindow : public L_Window {
public:
    XWindow(Resources& resources, Rectangle const &rectangle);
    ~XWindow() override;

    XWindow(XWindow const&) = delete;
    XWindow& operator=(XWindow const&) = delete;
    XWindow(XWindow&&) = delete;
    XWindow& operator=(XWindow&&) = delete;

    Window id;
    Rectangle rectangle;
    bool deleted = false;

    Rectangle rect() const override { return rectangle; }
    void fill(Color const &color) override;
    void text(int x, int y, std::string const &text, TextProperties const& tp) override;
    void draw(int x, int y, std::string const &slice) override;
    std::string name() const override;
    void set_cursor(std::string const &key) override;

private:
    Resources& resources_;
    GC gc_;
    XftDraw* xft_draw_ = nullptr;
};

#endif //XWINDOW_HH_
