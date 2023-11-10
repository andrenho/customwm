#include "focusmanager.hh"

#include "common/logger.hh"
#include "wm.hh"

bool FocusManager::is_window_focused(LWindow const *window) const
{
    return focused_window_.value_or(nullptr) == window;
}

void FocusManager::set_focus(std::optional<LWindow *> window)
{
    auto previous_window = focused_window_;
    focused_window_ = window;

    if (previous_window) {
        LOG.debug("Window %d lost focus.", previous_window.value()->id());
        wm_->expose(previous_window.value());
    }

    if (focused_window_) {
        LOG.debug("Window %d gained focus.", focused_window_.value()->id());
        wm_->expose(focused_window_.value());

        // move window to end of order
        auto it = std::find(window_order_.begin(), window_order_.end(), *focused_window_);
        if (it == window_order_.end())
            window_order_.push_back(*focused_window_);
        else
            std::rotate(it, it+1, window_order_.end());
    }
}

void FocusManager::remove_window(LWindow *window)
{
    if (focused_window_ && *focused_window_ == window)
        focused_window_ = {};

    window_order_.erase(std::remove(window_order_.begin(), window_order_.end(), window), window_order_.end());

    if (!focused_window_ && !window_order_.empty()) {
        focused_window_ = window_order_.back();
    }
}
