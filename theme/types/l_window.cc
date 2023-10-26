#include "l_window.hh"

#include "luaw.hh"
#include "logger.hh"

#define THIS (luaw_to<L_Window*>(L, 1))

void l_window_create_metadata(lua_State* L)
{
    std::string mt = luaw_set_metatable<L_Window>(L, {
            { "fill", [](lua_State* L) { THIS->fill(luaw_to<Color>(L, 2)); return 0; } },
            { "rect", [](lua_State* L) { return luaw_push(L, THIS->rect()); } },
    });
    LOG.debug("Metatable setup for Window as '%s'", mt.c_str());
}