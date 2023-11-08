#include "l_wm.hh"

#include "luaw.hh"
#include "util/logger.hh"

#define THIS (luaw_to<L_WM*>(L, 1))

void l_wm_create_metadata(lua_State* L)
{
    std::string mt = luaw_set_metatable<L_WM>(L, {
        { "interface_name", [](lua_State* L) {
            return luaw_push(L, THIS->interface_name());
        }},
        { "move_window_with_mouse", [](lua_State* L) {
            std::optional<L_Window *> window {};
            if (lua_gettop(L) >= 3)
                window = luaw_to<std::optional<L_Window*>>(L, 3);
            THIS->move_window_with_mouse(luaw_to<bool>(L, 2), window);
            return 0;
        }},
        { "set_focus", [](lua_State* L) {
            std::optional<L_Window *> window {};
            if (lua_gettop(L) >= 2)
                window = luaw_to<std::optional<L_Window*>>(L, 2);
            THIS->set_focus(window);
            return 0;
        }},
    });
    LOG.debug("Metatable setup for WM as '%s'", mt.c_str());
}