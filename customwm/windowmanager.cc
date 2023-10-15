#include "windowmanager.hh"

WindowManager::WindowManager(Theme &theme, Root &root)
    : theme_(theme), root_(root)
{
    root.server().setup_event_listeners();
    root.server().capture_existing_windows();
}

void WindowManager::run_event_loop()
{
    root_.server().run_event_loop();
}
