#include "ibrush.hh"
#include "window.hh"

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

static std::pair<IBrush*, Handle> brush_info(lua_State* L)
{
    Window* window = (Window *) lua_topointer(L, 1);
    lua_getglobal(L, "__brush_ptr");
    IBrush* ibrush = (IBrush *) lua_topointer(L, -1);
    lua_pop(L, 1);
    return { ibrush, window->gc };
}

static luaL_Reg brush_f[] = {
        { "set_color", [](lua_State *L) { auto [brush, gc] = brush_info(L); brush->set_color(gc, luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4)); return 0; } },
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