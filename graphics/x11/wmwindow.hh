#ifndef WMWINDOW_HH_
#define WMWINDOW_HH_

#include <X11/X.h>
#include "theme/types/l_window.hh"

class WM_Window : public L_Window {
public:
    WM_Window(Window parent_id, Window child_id, Rectangle const &rectangle)
        : parent_id(parent_id), child_id(child_id), rectangle(rectangle) {}

    Window parent_id;
    Window child_id;
    Rectangle rectangle;

    Rectangle rect() const override { return rectangle; }
};

#endif //WMWINDOW_HH_
