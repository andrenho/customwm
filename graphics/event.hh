#ifndef EVENT_HH_
#define EVENT_HH_

#include <variant>
#include "windowhandle.hh"
#include "theme/types/types.hh"

struct WindowAdded {
    WindowHandle handle;
};

struct WindowRemoved {
    WindowHandle handle;
};

struct WindowExpose {
    WindowHandle handle;
    Rectangle    rectangle;
};

struct Quit {};

using Event = std::variant<WindowAdded, WindowRemoved, WindowExpose, Quit>;

#endif //EVENT_HH_
