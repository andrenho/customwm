#ifndef WM_WINDOW_HH_
#define WM_WINDOW_HH_

#include <cstdint>

struct WM_Window {
    uintptr_t parent_id;
    uintptr_t child_id;
    int32_t  x, y;
    uint32_t w, h;
};

#endif //WM_WINDOW_HH_
