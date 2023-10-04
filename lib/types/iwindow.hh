#ifndef IWINDOW_HH_
#define IWINDOW_HH_

#include <cstdint>
#include <memory>

#include "../luaw.hh"
#include "types.hh"

struct IWindow {
    int x, y, w, h;
};

template<> void luaw_push(lua_State* L, IWindow const& w);

#endif //IWINDOW_HH_
