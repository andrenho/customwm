#ifndef XWINDOW_HH_
#define XWINDOW_HH_

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include "theme/types/lwindow.hh"
#include "xresources.hh"

#include <map>

class XWindow : public LWindow {
public:
    XWindow(class XWindowManager const& wm, XResources const& resources, Rectangle const &rectangle);
    ~XWindow() override;

    XWindow(XWindow const&) = delete;
    XWindow& operator=(XWindow const&) = delete;
    XWindow(XWindow&&) = delete;
    XWindow& operator=(XWindow&&) = delete;

    Window id_;
    bool deleted = false;

    WHandle                id() const override { return id_; }
    std::optional<WHandle> child_id() const override;
    Rectangle              rect(bool update_cache) const override;
    void                   fill(Color const &color, std::optional<Rectangle> rect) override;
    void                   text(int x, int y, std::string const &text, TextProperties const& tp) override;
    void                   draw(int x, int y, std::string const &slice) override;
    std::string            name() const override;
    void                   set_cursor(std::string const &key) override;
    bool                   focused() const override;

    void move(Point const &new_pos) override;

    std::vector<std::string> child_protocols() const;

private:
    class XWindowManager const& wm_;
    XResources const& resources_;
    GC gc_;
    XftDraw* xft_draw_ = nullptr;
};

#endif //XWINDOW_HH_
