#ifndef WINDOWMANAGER_HH_
#define WINDOWMANAGER_HH_

#include "../libengine/engine.hh"
#include "../libroot/root.hh"

class WindowManager {
public:
    WindowManager(Engine& engine, Root& root);

    void run_event_loop();

private:
    Engine& theme_;
    Root& root_;
};

#endif //WINDOWMANAGER_HH_
