#include "window.hh"

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

template<> void luaw_push(lua_State* L, Window const& w)
{
    lua_newtable(L);

    // pointer
    lua_pushlightuserdata(L, (void *) &w); lua_setfield(L, -2, "_ptr");

    // properties
    lua_pushinteger(L, w.outer_id); lua_setfield(L, -2, "outer_id");
    lua_pushinteger(L, w.inner_id); lua_setfield(L, -2, "inner_id");
    lua_pushinteger(L, w.w); lua_setfield(L, -2, "w");
    lua_pushinteger(L, w.h); lua_setfield(L, -2, "h");

    // methods
    lua_pushcfunction(L, [](lua_State* L) {
        lua_getfield(L, 1, "_ptr");
        auto w = (Window *) lua_topointer(L, -1);
        lua_pop(L, 1);
        lua_pushinteger(L, w->w);
        return 1;
    });
    lua_setfield(L, -2, "width");
}
