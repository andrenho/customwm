#ifndef WMWINDOW_HH_
#define WMWINDOW_HH_

#include <X11/Xlib.h>
#include "theme/types/l_window.hh"

class WM_Window : public L_Window {
public:
    WM_Window(Display* dpy_, Window parent_id, Window child_id, Rectangle const &rectangle);
    ~WM_Window() override;

    Window parent_id;
    Window child_id;
    Rectangle rectangle;

    Rectangle rect() const override { return rectangle; }

private:
    Display* dpy_;
    Colormap colormap_;
};

#endif //WMWINDOW_HH_
