#include "window.hh"

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

template<> void luaw_push(lua_State* L, Window const& w)
{
    lua_newtable(L);

    // properties
    lua_pushstring(L, "window"); lua_setfield(L, -2, "type");
    lua_pushinteger(L, w.outer_id); lua_setfield(L, -2, "outer_id");
    lua_pushinteger(L, w.inner_id); lua_setfield(L, -2, "inner_id");
    lua_pushinteger(L, w.gc); lua_setfield(L, -2, "gc");
    lua_pushinteger(L, w.w); lua_setfield(L, -2, "w");
    lua_pushinteger(L, w.h); lua_setfield(L, -2, "h");

    // metatable
    luaL_setmetatable(L, "__brush_mt");
}
