#include "iwindow.hh"

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

template<> void luaw_push(lua_State* L, IWindow const& w)
{
    lua_newtable(L);

    // properties
    lua_pushstring(L, "window"); lua_setfield(L, -2, "type");
    lua_pushinteger(L, w.x); lua_setfield(L, -2, "x");
    lua_pushinteger(L, w.y); lua_setfield(L, -2, "y");
    lua_pushinteger(L, w.w); lua_setfield(L, -2, "w");
    lua_pushinteger(L, w.h); lua_setfield(L, -2, "h");
}
