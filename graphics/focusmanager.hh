#ifndef FOCUSMANAGER_HH_
#define FOCUSMANAGER_HH_

#include "theme/types/lwindow.hh"

class FocusManager {
public:
    explicit FocusManager(class WindowManager* wm) : wm_(wm) {}

    bool is_window_focused(LWindow const* window) const;
    void set_focus(std::optional<LWindow *> window);
    void remove_window(LWindow* window);

private:
    class WindowManager*    wm_;
    std::optional<LWindow*> focused_window_ {};

    std::vector<LWindow*>   window_order_;
};

#endif //FOCUSMANAGER_HH_
