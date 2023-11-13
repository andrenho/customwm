#ifndef GRABMANAGER_HH_
#define GRABMANAGER_HH_

#include "theme/types/lwindow.hh"
#include "theme/types/lwm.hh"

#include <chrono>
using sc = std::chrono::system_clock;

class GrabManager {
public:
    explicit GrabManager(class WindowManager* wm);

    void set_grab(LWindow *window, GrabType grab_type, Point const& initial_pos);
    bool is_active() const { return current_grab_.has_value(); }
    bool is_moving() const { return current_grab_.has_value() && current_grab_->grab_type == GrabType::Move; }

    void move_pointer(Point const &current_pos);

private:
    class WindowManager* wm_;

    struct Grab {
        LWindow*  window;
        GrabType  grab_type;
        Point     initial_pos;
        Rectangle initial_rect;
        Size      minimum_window_size;
    };
    std::optional<Grab> current_grab_ {};

    sc::time_point last_resize_ = sc::now();
    sc::duration   resize_update_time_;

    void resize(Point const &current_pos);
};

#endif //GRABMANAGER_HH_
