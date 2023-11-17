#include "windowmanager.hh"

#include "graphics/graphics.hh"
#include "theme/theme.hh"

void WindowManager::init()
{
    // add_existing_windows();  TODO
    graphics_->listen_to_wm_events();
    theme_->call_opt("wm.after_start");
}
