#include <string>
#include <tuple>

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

#include "ibrush.hh"
#include "window.hh"
#include "../exceptions.hh"


static std::tuple<IBrush*, Handle, Handle> brush_info(lua_State* L)
{
    static const char* error_msg = "Expected a window. Check if you're not using '.' instead of ':' when executing an action on a window.";

    if (lua_type(L, 1) != LUA_TTABLE)
        throw LuaException(error_msg);

    lua_getfield(L, 1, "type");
    if (std::string(lua_tostring(L, -1)) != "window")
        throw LuaException(error_msg);
    lua_pop(L, 1);

    Window* window = (Window *) lua_topointer(L, 1);
    lua_getglobal(L, "__brush_ptr");
    IBrush* ibrush = (IBrush *) lua_topointer(L, -1);
    lua_pop(L, 1);
    return { ibrush, window->outer_id, window->gc };
}

static luaL_Reg brush_f[] = {
        { "set_color", [](lua_State *L) {
            auto [brush, _, gc] = brush_info(L);
            brush->set_color(gc, luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4));
            return 0;
        } },
        { "draw_rect", [](lua_State* L) {
            auto [brush, w_id, gc] = brush_info(L);
            brush->draw_rect(w_id, gc,
                             luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4), luaL_checkinteger(L, 5),
                             lua_type(L, 6) == LUA_TSTRING && std::string(lua_tostring(L, 6)) == "fill");
            return 0;
        } },
        { nullptr, nullptr },
};

void ibrush_add_to_lua(lua_State* L, IBrush* ibrush)
{
    // add pointer
    lua_pushlightuserdata(L, ibrush);
    lua_setglobal(L, "__brush_ptr");

    // create metatable
    luaL_newmetatable(L, "__brush_mt");
    luaL_setfuncs(L, brush_f, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1);
}