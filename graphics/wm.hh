#ifndef WM_HH_
#define WM_HH_

#include <unordered_map>

#include "wm_window.hh"
#include "theme/types/l_wm.hh"

class WM : public L_WM {
public:
    virtual void run() = 0;

protected:
    std::unordered_map<intptr_t, WM_Window> windows_;
};

#endif //WM_HH_
