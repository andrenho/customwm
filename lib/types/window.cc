#include "window.hh"

template<> void luaw_push(lua_State* L, Window const& w)
{
    lua_newtable(L);
    lua_pushinteger(L, w.outer_id); lua_setfield(L, -2, "outer_id");
    lua_pushinteger(L, w.inner_id); lua_setfield(L, -2, "inner_id");
    lua_pushinteger(L, w.w); lua_setfield(L, -2, "w");
    lua_pushinteger(L, w.h); lua_setfield(L, -2, "h");
}
