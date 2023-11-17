#ifndef XWINDOW_HH_
#define XWINDOW_HH_

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include "xresources.hh"
#include ".old/graphics/gwindow.hh"

#include <map>

class XWindow : public GWindow {
public:
    XWindow(class XWindowManager const& wm, XResources const& resources, Rectangle const &rectangle);
    ~XWindow() override;

    XWindow(XWindow const&) = delete;
    XWindow& operator=(XWindow const&) = delete;
    XWindow(XWindow&&) = delete;
    XWindow& operator=(XWindow&&) = delete;

    Window id_;

    WHandle                id() const override { return id_; }
    Rectangle              rect(bool update_cache) const override;
    void                   fill(Color const &color, std::optional<Rectangle> rect) override;
    void                   text(int x, int y, std::string const &text, TextProperties const& tp) override;
    void                   draw(int x, int y, std::string const &slice) override;
    std::string            name() const override;
    void                   set_cursor(std::string const &key) override;
    void                   move(Point const& new_pos) override;
    void                   resize(Size const& new_size) override;
    void                   close() override;
    void                   bring_to_front() override;

    // these functions are used to accelerate the painting of the screen (expose) while moving
    void                   update_backbuffer(Rectangle const& rectangle);
    void                   update_backbuffer_size(Size const& sz);
    void                   expose_from_backbuffer(Rectangle const& rectangle);

    std::vector<std::string> child_protocols() const;

private:
    class XWindowManager const& xwm_;
    XResources const&           resources_;
    GC                          gc_;
    XftDraw*                    xft_draw_ = nullptr;
    Pixmap                      backbuffer_;
};

#endif //XWINDOW_HH_
