#ifndef WM_HH_
#define WM_HH_


#include "theme/types/l_wm.hh"
#include "theme/types/l_window.hh"

class WM : public L_WM {
public:
    virtual void run() = 0;
};

#endif //WM_HH_
