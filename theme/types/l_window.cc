#include "l_window.hh"

#include "luaw.hh"
#include "logger.hh"

void l_window_create_metadata(lua_State* L)
{
    std::string mt = luaw_set_metatable<L_Window>(L, {
            { "rect", [](lua_State* L) { return luaw_push(L, luaw_to<L_Window*>(L, 1)->rect()); } },
    });
    LOG.debug("Metatable setup for Window as '%s'", mt.c_str());
}