#include "grabmanager.hh"

#include "wm.hh"

void GrabManager::set_grab(LWindow *window, GrabType grab_type)
{
    if (grab_type == GrabType::NoGrab) {
        if (current_grab_)
            current_grab_->window->set_cursor(Cursors::Pointer);
        current_grab_ = {};
    } else {
        current_grab_ = { window, grab_type };
    }
}

void GrabManager::move_pointer(Point const &p)
{
    if (current_grab_) {
        Point diff = p - last_pointer_;
        LWindow* window = current_grab_->window;
        Rectangle rect = window->rect(true);

        Point new_pos { 0, 0 };
        switch (current_grab_->grab_type) {
            case GrabType::NoGrab:
                break;
            case GrabType::Move:
                new_pos = rect.topleft() + diff;
                window->move(new_pos);
                break;
            case GrabType::TopLeft:
                window->move({ rect.x + diff.x, rect.y + diff.y });
                window->resize({ rect.w - diff.x, rect.h - diff.y });
                break;
            case GrabType::Top:
                window->move({ rect.x, rect.y + diff.y });
                window->resize({ rect.w, rect.h - diff.y });
                break;
            case GrabType::TopRight:
                window->move({ rect.x, rect.y + diff.y });
                window->resize({ rect.w + diff.x, rect.h - diff.y });
                break;
            case GrabType::Left:
                window->move({ rect.x + diff.x, rect.y });
                window->resize({ rect.w - diff.x, rect.h });
                break;
            case GrabType::Right:
                window->resize({ rect.w + diff.x, rect.h });
                break;
            case GrabType::BottomLeft:
                window->move({ rect.x + diff.x, rect.y });
                window->resize({ rect.w - diff.x, rect.h + diff.y });
                break;
            case GrabType::Bottom:
                window->resize({ rect.w, rect.h + diff.y });
                break;
            case GrabType::BottomRight:
                window->resize({ rect.w + diff.x, rect.h + diff.y });
                break;
        }
    }

    last_pointer_ = p;
}

#if 0
// check if moving window
    if (moving_window_with_mouse_.has_value()) {
        XWindowAttributes xwa;
        XGetWindowAttributes(X->display, (*moving_window_with_mouse_)->id, &xwa);
        int x = xwa.x + e.x_root - last_mouse_position_.x;
        int y = xwa.y + e.y_root - last_mouse_position_.y;
        XMoveWindow(X->display, (*moving_window_with_mouse_)->id, x, y);
    }

    last_mouse_position_ = { e.x_root, e.y_root };
#endif
