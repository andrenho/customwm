#include "l_window.hh"

#include "luaw.hh"

void L_Window::to_lua(lua_State* L) const
{
    lua_newtable(L);
    luaw_setfield(L, -1, "parent_id", parent_id);
    luaw_setfield(L, -1, "child_id", child_id);
    luaw_setfield(L, -1, "x", x);
    luaw_setfield(L, -1, "y", y);
    luaw_setfield(L, -1, "w", w);
    luaw_setfield(L, -1, "h", h);
}
