#include "types.hh"

#include <string>

extern "C" {
#include <lauxlib.h>
}

#include "../exceptions.hh"

//
// TO
//

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
                throw LuaException(L, "Invalid starting position '" + s + "'");
            break;
        }
        case LUA_TTABLE:
            window_starting_pos = {
                    .starting_pos = WindowStartingPos::Custom,
                    .point = luaw_to<Point>(L, index),
            };
            break;
        default:
            throw LuaException(L, "expected a string, table or function");
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

template<> Color luaw_to(lua_State* L, int index)
{
    std::string color = luaL_checkstring(L, index);

    try {
        if (color.size() != 7 || color[0] != '#')
            throw;
        return {
            (uint8_t) std::stoi(color.substr(1, 2), nullptr, 16),
            (uint8_t) std::stoi(color.substr(3, 2), nullptr, 16),
            (uint8_t) std::stoi(color.substr(5, 2), nullptr, 16)
        };

    } catch (std::exception& e) {
        throw LuaException(L, "Invalid get_color format");
    }
}

template<> Rectangle luaw_to(lua_State* L, int index)
{
    luaL_checktype(L, index, LUA_TTABLE);

    auto get_idx = [L, index](int idx) {
        lua_geti(L, index, idx);
        int16_t n = (int16_t) luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        return n;
    };
    return { get_idx(1), get_idx(2), (uint16_t) get_idx(3), (uint16_t) get_idx(4) };
}

//
// PUSH
//

template<> void luaw_push(lua_State* L, Rectangle const& r)
{
    lua_newtable(L);
    lua_pushinteger(L, r.x); lua_setfield(L, -2, "x");
    lua_pushinteger(L, r.y); lua_setfield(L, -2, "y");
    lua_pushinteger(L, r.w); lua_setfield(L, -2, "w");
    lua_pushinteger(L, r.h); lua_setfield(L, -2, "h");
}

