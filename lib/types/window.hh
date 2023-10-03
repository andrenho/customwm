#ifndef WINDOW_HH_
#define WINDOW_HH_

#include <cstdint>
#include <memory>

#include "../luaw.hh"
#include "types.hh"

struct Window {
    Handle outer_id;
    Handle inner_id;
    int w, h;
};

template<> void luaw_push(lua_State* L, Window const& w);

#endif //WINDOW_HH_