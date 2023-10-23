#include "metatables.hh"
#include "../libroot/root.hh"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static Root* root(lua_State* L) {
    luaL_checkudata(L, 1, "__root_mt");
    return ((RootUserData *) lua_touserdata(L, 1))->root;
}

void metatable_root_create(lua_State* L)
{
    static luaL_Reg root_f[] = {
            { "interface_name", [](lua_State* L) {
                lua_pushstring(L, root(L)->interface_name().c_str());
                return 1;
            } },
            {nullptr, nullptr}
    };

    luaL_newmetatable(L, "__root_mt");
    luaL_setfuncs(L, root_f, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
}