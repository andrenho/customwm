#ifndef WM_HH_
#define WM_HH_

#include <unordered_map>

#include "wm_window.hh"

class WM {
public:
    virtual ~WM() = default;

    virtual void run() = 0;

protected:
    std::unordered_map<intptr_t, WM_Window> windows_;
};

#endif //WM_HH_
