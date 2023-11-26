#ifndef PARENTWINDOW_HH_
#define PARENTWINDOW_HH_

#include "window/window.hh"

class ParentWindow : public Window_ {
public:
    ParentWindow(class Theme* theme, class Graphics* graphics, Rectangle const& rectangle)
            : Window_(theme, graphics, rectangle) {}

    void reparent_child(WindowHandle child_handle, Point const& offset, Padding const& padding);

    WindowHandle child_handle() const { return child_handle_; }

    static constexpr const char* mt_identifier = "Window";

private:
    WindowHandle child_handle_ = 0;
    Padding      child_padding_;
};

#endif //PARENTWINDOW_HH_
