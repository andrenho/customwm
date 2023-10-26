#include "l_wm.hh"

#include "luaw.hh"
#include "logger.hh"

void l_wm_create_metadata(lua_State* L)
{
    std::string mt = luaw_set_metatable<L_WM>(L, {
            { "interface_name", [](lua_State* L) { return luaw_push(L, luaw_to<L_WM*>(L, 1)->interface_name()); } },
    });
    LOG.debug("Metatable setup for WM as '%s'", mt.c_str());
}