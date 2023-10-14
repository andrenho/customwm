#include "windowmanager.hh"

WindowManager::WindowManager(Theme &theme, Root &root)
    : theme_(theme), root_(root)
{
    root.setup_event_listeners(*this);
    root.capture_existing_windows();
}

void WindowManager::run_event_loop()
{
    root_.run_event_loop();
}
