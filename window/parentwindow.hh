#ifndef PARENTWINDOW_HH_
#define PARENTWINDOW_HH_

#include "window.hh"

class ParentWindow : public Window_ {
public:
    ParentWindow(class Graphics* graphics, Rectangle const& rectangle) : Window_(graphics, rectangle) {}
    ~ParentWindow() override;

    void reparent_child(WindowHandle child_handle, Point const& offset);

    WindowHandle child_handle() const { return child_handle_; }

private:
    WindowHandle child_handle_ = 0;
};

#endif //PARENTWINDOW_HH_
