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
        current_grab_ = { window, grab_type, initial_pos, window->rect() };
    }
}

void GrabManager::move_pointer(Point const &current_pos)
{
    if (current_grab_) {

        if (current_grab_->grab_type == GrabType::Move) {
            LWindow* window = current_grab_->window;
            window->move(current_grab_->initial_rect.topleft() + current_pos - current_grab_->initial_pos);
        } else if (current_grab_->grab_type != GrabType::NoGrab) {
            resize(current_pos);
        }
    }
}

void GrabManager::resize(Point const &current_pos)
{
    LWindow* window = current_grab_->window;
    Size diff = current_pos - current_grab_->initial_pos;
    Rectangle const& initial_rect = current_grab_->initial_rect;

    if (sc::now() < (last_resize_ + resize_update_time_))
        return;

    switch (current_grab_->grab_type) {
        case GrabType::TopLeft:
            window->move({ initial_rect.x + (int32_t) diff.w, initial_rect.y + (int32_t) diff.h });
            window->resize({ initial_rect.w - diff.w, initial_rect.h - diff.h });
            break;
        case GrabType::Top:
            window->move({ initial_rect.x, initial_rect.y + (int32_t) diff.h });
            window->resize({ initial_rect.w, initial_rect.h - diff.h });
            break;
        case GrabType::TopRight:
            window->move({ initial_rect.x, initial_rect.y + (int32_t) diff.h });
            window->resize({ initial_rect.w + diff.w, initial_rect.h - diff.h });
            break;
        case GrabType::Left:
            window->move({ initial_rect.x + (int32_t) diff.w, initial_rect.y });
            window->resize({ initial_rect.w - diff.w, initial_rect.h });
            break;
        case GrabType::Right:
            window->resize({ initial_rect.w + diff.w, initial_rect.h });
            break;
        case GrabType::BottomLeft:
            window->move({ initial_rect.x + (int32_t) diff.w, initial_rect.y });
            window->resize({ initial_rect.w - diff.w, initial_rect.h + diff.h });
            break;
        case GrabType::Bottom:
            window->resize({ initial_rect.w, initial_rect.h + diff.h });
            break;
        case GrabType::BottomRight:
            window->resize(initial_rect.size() + diff);
            break;
        default: break;
    }

    last_resize_ = sc::now();
}

