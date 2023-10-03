#include "types.hh"

#include <string>

extern "C" {
#include <lauxlib.h>
}

#include "../exceptions.hh"

template<> WindowStartingPos luaw_to(lua_State* L, int index)
{
    WindowStartingPos window_starting_pos {};
    switch (lua_type(L, -1)) {
        case LUA_TSTRING: {
            std::string s(lua_tostring(L, -1));
            if (s == "cascade")
                window_starting_pos.starting_pos = WindowStartingPos::Cascade;
            else if (s == "center")
                window_starting_pos.starting_pos = WindowStartingPos::Center;
            else if (s == "random")
                window_starting_pos.starting_pos = WindowStartingPos::Random;
            else if (s == "maximized")
                window_starting_pos.starting_pos = WindowStartingPos::Maximized;
            else if (s == "requested")
                window_starting_pos.starting_pos = WindowStartingPos::Requested;
            else
                throw LuaException("Invalid starting position '" + s + "'");
            break;
        }
        case LUA_TTABLE:
            window_starting_pos = {
                    .starting_pos = WindowStartingPos::Custom,
                    .point = luaw_to<Point>(L, -1),
            };
            break;
        default:
            throw LuaException("expected a string, table or function");
    }

    return window_starting_pos;
}


template<> Padding luaw_to(lua_State* L, int index)
{
    luaL_checktype(L, -1, LUA_TTABLE);

    auto get_idx = [L](int idx) {
        lua_geti(L, -1, idx);
        int n = (int) luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        return n;
    };
    return { get_idx(1), get_idx(2), get_idx(3), get_idx(4) };
}


template<> Point luaw_to(lua_State* L, int index)
{
    auto get_idx = [L](int idx) {
        lua_geti(L, -1, idx);
        int n = (int) luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        return n;
    };
    int x = get_idx(1);
    int y = get_idx(2);
    return { x, y };
}