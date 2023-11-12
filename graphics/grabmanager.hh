#ifndef GRABMANAGER_HH_
#define GRABMANAGER_HH_

#include "theme/types/lwindow.hh"
#include "theme/types/lwm.hh"

class GrabManager {
public:
    explicit GrabManager(class WindowManager* wm) : wm_(wm) {}

    void set_grab(LWindow *window, GrabType grab_type);
    bool is_active() const { return current_grab_.has_value(); }
    bool is_moving() const { return current_grab_.has_value() && current_grab_->grab_type == GrabType::Move; }

    void move_pointer(Point const &p);

private:
    class WindowManager* wm_;

    struct Grab {
        LWindow* window;
        GrabType grab_type;
    };
    std::optional<Grab> current_grab_ {};

    Point last_pointer_ { 0, 0 };
};

#endif //GRABMANAGER_HH_
