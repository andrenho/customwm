#ifndef FOCUSMANAGER_HH_
#define FOCUSMANAGER_HH_

#include <vector>

#include "gwindow.hh"

class FocusManager {
public:
    explicit FocusManager(class GWindowManager* wm) : wm_(wm) {}

    bool is_window_focused(GWindow const* window) const;
    void set_focus(std::optional<GWindow *> window);
    void remove_window(GWindow* window);

    std::optional<GWindow*> focused_window() const { return focused_window_; }

private:
    class GWindowManager*    wm_;
    std::optional<GWindow*> focused_window_ {};

    std::vector<GWindow*>   window_order_;
};

#endif //FOCUSMANAGER_HH_
