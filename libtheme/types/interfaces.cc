#include "interfaces.hh"

extern "C" {
#include <lauxlib.h>
}

struct IWindowUserData {
    IWindow* iwindow;
};

static IWindow& get_window(lua_State* L, int idx) {
    luaL_checkudata(L, idx, "__iwindow_mt");
    return *((IWindowUserData *) lua_touserdata(L, idx))->iwindow;
}

static luaL_Reg iwindow_f[] = {
        { "area", [](lua_State *L) {
            luaw_push(L, get_window(L, 1).area());
            return 1;
        } },
        { "draw_rectangle", [](lua_State *L) {
            get_window(L, 1).draw_rectangles({ luaw_to<Rectangle>(L, 2) }, luaw_to<Color>(L, 3), lua_toboolean(L, 4));
            return 0;
        } },
        {nullptr, nullptr},
};

void IWindow::create_metatable(lua_State *L)
{
    luaL_newmetatable(L, "__iwindow_mt");
    luaL_setfuncs(L, iwindow_f, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
}

template<> void luaw_push(lua_State* L, IWindow* window)
{
    IWindowUserData* ud = (IWindowUserData *) lua_newuserdata(L, sizeof(IWindowUserData));
    ud->iwindow = window;
    luaL_setmetatable(L, "__iwindow_mt");
}