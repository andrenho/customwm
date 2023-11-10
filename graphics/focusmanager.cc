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
    }
}
