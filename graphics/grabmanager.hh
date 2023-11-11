#ifndef GRABMANAGER_HH_
#define GRABMANAGER_HH_

#include "theme/types/lwindow.hh"
#include "theme/types/lwm.hh"

class GrabManager {
public:
    explicit GrabManager(class WindowManager* wm) : wm_(wm) {}

    void set_grab(LWindow *window, GrabType grab_type);

private:
    class WindowManager*    wm_;
};

#endif //GRABMANAGER_HH_
