#ifndef WINDOWMANAGER_HH_
#define WINDOWMANAGER_HH_

#include "../libtheme/theme.hh"
#include "../libroot/root.hh"

class WindowManager {
public:
    WindowManager(Theme& theme, Root& root);

    void run_event_loop();

private:
    Theme& theme_;
    Root& root_;
};

#endif //WINDOWMANAGER_HH_
