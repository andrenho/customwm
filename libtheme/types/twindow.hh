#ifndef TWINDOW_HH_
#define TWINDOW_HH_

#include "../luaw.hh"

#include <cstdint>

struct TWindow {
    int16_t x, y;
    uint16_t w, h;
};

template <> void luaw_push(lua_State* L, TWindow const& t);

#endif //TWINDOW_HH_
