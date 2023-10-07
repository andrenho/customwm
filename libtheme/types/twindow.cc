#include "twindow.hh"

extern "C" {
#include <lauxlib.h>
}

template <> void luaw_push(lua_State* L, TWindow const& t)
{
    lua_newtable(L);
    lua_pushinteger(L, t.x); lua_setfield(L, -1, "x");
    lua_pushinteger(L, t.y); lua_setfield(L, -1, "y");
    lua_pushinteger(L, t.w); lua_setfield(L, -1, "w");
    lua_pushinteger(L, t.h); lua_setfield(L, -1, "h");
}