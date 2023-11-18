#include "wmevents.hh"

#include "graphics/graphics.hh"

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
                [this](WindowAdded& e)   { on_window_added(e); },
                [this](WindowRemoved& e) { on_window_removed(e); },
            }, *oe);
        }
    }
}

void WMEvents::on_window_added(WindowAdded const &added)
{

}

void WMEvents::on_window_removed(WindowRemoved const &removed)
{

}

