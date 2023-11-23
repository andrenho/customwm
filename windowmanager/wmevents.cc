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
    bool quit = false;

    while(!quit) {
        auto oe = graphics_->next_event();
        if (oe) {
            std::visit(overloaded {
                [this](WindowAdded& e)   { window_manager_->add_child_window(e.handle); },
                [this](WindowRemoved& e) { window_manager_->remove_window(e.handle); },
                [this](WindowExpose& e)  { window_manager_->expose_window(e.handle, e.rectangle); },
                [&quit](Quit& _)         { (void)_; quit = true; }
            }, *oe);
        }
    }
}