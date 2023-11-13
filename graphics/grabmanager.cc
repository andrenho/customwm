#include <random>
#include "grabmanager.hh"

#include "wm.hh"
#include "theme/theme.hh"

GrabManager::GrabManager(WindowManager *wm)
        : wm_(wm)
{
    resize_update_time_ = std::chrono::milliseconds(THEME.get_prop<unsigned long>("wm.resize_update_ms"));
}

void GrabManager::set_grab(LWindow *window, GrabType grab_type, Point const& initial_pos)
{
    if (grab_type == GrabType::NoGrab) {
        if (current_grab_)
            current_grab_->window->set_cursor(Cursors::Pointer);
        current_grab_ = {};
    } else {
        current_grab_ = {
                .window = window,
                .grab_type = grab_type,
                .initial_pos = initial_pos,
                .initial_rect = window->rect(),
                .minimum_window_size = THEME.get_prop<Size>("wm.minimum_window_size", wm_, window),
                .minimum_window_location = THEME.get_prop<Point>("wm.minimum_window_location", wm_, window),
                .maximum_window_location = THEME.get_prop<Point>("wm.maximum_window_location", wm_, window),
        };
    }
}

void GrabManager::move_pointer(Point const &current_pos)
{
    if (current_grab_) {

        if (current_grab_->grab_type == GrabType::Move) {
            move(current_pos);
        } else if (current_grab_->grab_type != GrabType::NoGrab) {
            resize(current_pos);
        }
    }
}

void GrabManager::move(Point const &current_pos)
{
    LWindow* window = this->current_grab_->window;
    Point new_pos = this->current_grab_->initial_rect.topleft() + current_pos - this->current_grab_->initial_pos;

    new_pos.x = std::max(new_pos.x, this->current_grab_->minimum_window_location.x);
    new_pos.x = std::min(new_pos.x, this->current_grab_->maximum_window_location.x);
    new_pos.y = std::max(new_pos.y, this->current_grab_->minimum_window_location.y);
    new_pos.y = std::min(new_pos.y, this->current_grab_->maximum_window_location.y);

    window->move(new_pos);
}

void GrabManager::resize(Point const &current_pos)
{
    LWindow* window = current_grab_->window;
    Size diff = current_pos - current_grab_->initial_pos;
    Rectangle const& initial_rect = current_grab_->initial_rect;

    if (sc::now() < (last_resize_ + resize_update_time_))
        return;

    auto limit = [&](Size const& sz) {
        return Size(
                std::max(sz.w, current_grab_->minimum_window_size.w),
                std::max(sz.h, current_grab_->minimum_window_size.h));
    };

    switch (current_grab_->grab_type) {
        case GrabType::TopLeft:
            window->move({ initial_rect.x + diff.w, initial_rect.y + diff.h });
            window->resize(limit({ initial_rect.w - diff.w, initial_rect.h - diff.h }));
            break;
        case GrabType::Top:
            window->move({ initial_rect.x, initial_rect.y + diff.h });
            window->resize(limit({ initial_rect.w, initial_rect.h - diff.h }));
            break;
        case GrabType::TopRight:
            window->move({ initial_rect.x, initial_rect.y + diff.h });
            window->resize(limit({ initial_rect.w + diff.w, initial_rect.h - diff.h }));
            break;
        case GrabType::Left:
            window->move({ initial_rect.x + diff.w, initial_rect.y });
            window->resize(limit({ initial_rect.w - diff.w, initial_rect.h }));
            break;
        case GrabType::Right:
            window->resize(limit({ initial_rect.w + diff.w, initial_rect.h }));
            break;
        case GrabType::BottomLeft:
            window->move({ initial_rect.x + diff.w, initial_rect.y });
            window->resize(limit({ initial_rect.w - diff.w, initial_rect.h + diff.h }));
            break;
        case GrabType::Bottom:
            window->resize(limit({ initial_rect.w, initial_rect.h + diff.h }));
            break;
        case GrabType::BottomRight:
            window->resize(limit(initial_rect.size() + diff));
            break;
        default: break;
    }

    last_resize_ = sc::now();
}

