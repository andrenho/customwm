#include "windowmanager.hh"

WindowManager::WindowManager(Engine &engine, Root &root)
    : theme_(engine), root_(root)
{
    root.server().setup_event_listeners();
    root.server().capture_existing_windows();
}

void WindowManager::run_event_loop()
{
    root_.server().run_event_loop();
}
