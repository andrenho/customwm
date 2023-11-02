#include "l_window.hh"

#include "luaw.hh"
#include "theme/logger.hh"

#define THIS (luaw_to<L_Window*>(L, 1))

void l_window_create_metadata(lua_State* L)
{
    std::string mt = luaw_set_metatable<L_Window>(L, {
            { "fill", [](lua_State* L) {
                THIS->fill(luaw_to<Color>(L, 2));
                return 0;
            }},
            { "rect", [](lua_State* L) {
                return luaw_push(L, THIS->rect());
            }},
            { "text", [](lua_State* L) {
                TextProperties tp {};
                if (lua_gettop(L) >= 5)
                    tp = luaw_to<TextProperties>(L, 5);
                THIS->text(luaw_to<int>(L, 2), luaw_to<int>(L, 3), luaw_to<std::string>(L, 4), tp);
                return 0;
            }},
            { "draw", [](lua_State* L) {
                THIS->draw(luaw_to<int>(L, 2), luaw_to<int>(L, 3), luaw_to<std::string>(L, 4));
                return 0;
            }},
            { "name", [](lua_State* L) {
                luaw_push(L, THIS->name());
                return 1;
            }},
    });
    LOG.debug("Metatable setup for Window as '%s'", mt.c_str());
}