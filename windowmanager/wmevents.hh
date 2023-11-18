#ifndef WMEVENTS_HH_
#define WMEVENTS_HH_

#include "graphics/event.hh"

class WMEvents {
public:
    WMEvents(class Theme* theme, class Graphics* graphics, class WindowManager* window_manager)
            : theme_(theme), graphics_(graphics), window_manager_(window_manager) {}

    void init();
    void run();

private:
    class Theme* theme_;
    class Graphics* graphics_;
    class WindowManager* window_manager_;
};

#endif //WMEVENTS_HH_
