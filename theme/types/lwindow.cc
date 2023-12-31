#include "lwindow.hh"

#include "luaw.hh"
#include ".old/common/logger.hh"

#define THIS (luaw_to<LWindow*>(L, 1))

void l_window_create_metadata(lua_State* L)
{
    std::string mt = luaw_set_metatable<LWindow>(L, {
            { "id", [](lua_State* L) {
                return luaw_push(L, THIS->id());
            }},
            { "child_id", [](lua_State* L) {
                return luaw_push(L, THIS->child_id());
            }},
            { "padding", [](lua_State* L) {
                return luaw_push(L, THIS->padding());
            }},
            { "rect", [](lua_State* L) {
                if (lua_gettop(L) >= 2)
                    return luaw_push(L, THIS->rect(luaL_checkinteger(L, 3)));
                else
                    return luaw_push(L, THIS->rect());
            }},
            { "fill", [](lua_State* L) {
                if (lua_gettop(L) >= 3)
                    THIS->fill(luaw_to<Color>(L, 2), luaw_to<std::optional<Rectangle>>(L, 3));
                else
                    THIS->fill(luaw_to<Color>(L, 2));
                return 0;
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
            { "set_cursor", [](lua_State *L) {
                THIS->set_cursor(luaL_checkstring(L, 2));
                return 0;
            }},
            { "focused", [](lua_State *L) {
                luaw_push(L, THIS->focused());
                return 1;
            }},
            { "move", [](lua_State* L) {
                THIS->move(luaw_to<Point>(L, 2));
                return 0;
            }},
            { "resize", [](lua_State* L) {
                THIS->resize(luaw_to<Size>(L, 2));
                return 0;
            }},
            { "maximize_restore", [](lua_State* L) {
                THIS->maximize_restore();
                return 0;
            }},
            { "close", [](lua_State* L) {
                THIS->close();
                return 0;
            }},
    });
    LOG.debug("Metatable setup for Window as '%s'", mt.c_str());
}