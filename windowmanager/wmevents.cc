#include "wmevents.hh"

#include "graphics/graphics.hh"
#include "theme/theme.hh"
#include "theme/types/types.hh"
#include "windowmanager.hh"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

void WMEvents::init()
{
    graphics_->subscribe_to_wm_events();
}

void WMEvents::run()
{
    for (;;) {
        auto oe = graphics_->next_event();
        if (oe) {
            std::visit(overloaded {
                [this](WindowAdded& e)   { window_manager_->add_child_window(e.handle); },
                [this](WindowRemoved& e) { window_manager_->remove_window(e.handle); },
            }, *oe);
        }
    }
}