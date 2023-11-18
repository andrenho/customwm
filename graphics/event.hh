#ifndef EVENT_HH_
#define EVENT_HH_

#include <variant>
#include "whandle.hh"

struct WindowAdded {
    WindowHandle handle;
};

struct WindowRemoved {
    WindowHandle handle;
};

using Event = std::variant<WindowAdded, WindowRemoved>;

#endif //EVENT_HH_
