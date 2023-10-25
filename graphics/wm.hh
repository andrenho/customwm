#ifndef WM_HH_
#define WM_HH_

#include <unordered_map>

#include "theme/types/l_wm.hh"
#include "theme/types/l_window.hh"

class WM : public L_WM {
public:
    virtual void run() = 0;

protected:
    std::unordered_map<intptr_t, std::unique_ptr<L_Window>> windows_;
};

#endif //WM_HH_
