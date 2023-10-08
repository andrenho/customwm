#include "luaw.hh"

extern "C" {
#include <lauxlib.h>
}

template <typename T>
concept Iterable = requires(T t) {
    begin(t);
    end(t);
    t.push_back(typename T::value_type{});
};

template <Iterable T> T luaw_to(lua_State* L, int index)
{
    int top = lua_gettop(L);

    luaL_checktype(L, index, LUA_TTABLE);

    T ts;
    lua_pushnil(L);
    while (lua_next(L, index) != 0) {
        ts.push_back(luaw_to<typename T::value_type>(L, -1));
        lua_pop(L, 1);
    }

    luaw_asserttop(L, top);

    return ts;
}
