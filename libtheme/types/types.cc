#include "types.hh"

#include <string>

extern "C" {
#include <lauxlib.h>
}

#include "../exceptions.hh"

template<> Point luaw_to(lua_State* L, int index)
{
    auto get_idx = [L, index](int idx) {
        lua_geti(L, index, idx);
        int16_t n = (int16_t) luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        return n;
    };
    int16_t x = get_idx(1);
    int16_t y = get_idx(2);
    return { x, y };
}

template<> WindowStartingPos luaw_to(lua_State* L, int index)
{
    WindowStartingPos window_starting_pos {};
    switch (lua_type(L, index)) {
        case LUA_TSTRING: {
            std::string s(lua_tostring(L, index));
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
                    .point = luaw_to<Point>(L, index),
            };
            break;
        default:
            throw LuaException("expected a string, table or function");
    }

    return window_starting_pos;
}


template<> Padding luaw_to(lua_State* L, int index)
{
    luaL_checktype(L, index, LUA_TTABLE);

    auto get_idx = [L, index](int idx) {
        lua_geti(L, index, idx);
        int16_t n = (int16_t) luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        return n;
    };
    return { get_idx(1), get_idx(2), get_idx(3), get_idx(4) };
}


