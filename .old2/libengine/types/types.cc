#include "types.hh"

#include <string>

extern "C" {
#include <lauxlib.h>
}

#include "../exceptions.hh"

//
// TO
//

template <typename T = long long>
static T to_int(lua_State* L, int index, const char* field)
{
    lua_getfield(L, index, field);
    T value = (T) lua_tointeger(L, -1);
    lua_pop(L, 1);
    return value;
}

template<> Point luaw_to(lua_State* L, int index)
{
    luaL_checktype(L, index, LUA_TTABLE);

    return {
        .x = to_int<int16_t>(L, index, "x"),
        .y = to_int<int16_t>(L, index, "y"),
    };
}

template<> Size luaw_to(lua_State* L, int index)
{
    luaL_checktype(L, index, LUA_TTABLE);

    return {
        .w = to_int<uint16_t>(L, index, "w"),
        .h = to_int<uint16_t>(L, index, "h"),
    };
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

    return {
            .x = to_int<int16_t>(L, index, "x"),
            .y = to_int<int16_t>(L, index, "y"),
            .w = to_int<uint16_t>(L, index, "w"),
            .h = to_int<uint16_t>(L, index, "h"),
    };
}

template<> Window luaw_to(lua_State* L, int index)
{
    luaL_checktype(L, index, LUA_TTABLE);

    Window window {
        .outer_id = to_int<uintptr_t>(L, index, "outer_id"),
        .child_id = to_int<uintptr_t>(L, index, "child_id"),
    };
    lua_getfield(L, -1, "outer_id"); window.outer_id = lua_tointeger(L, -1); lua_pop(L, 1);
    lua_getfield(L, -1, "child_id"); window.child_id = lua_tointeger(L, -1); lua_pop(L, 1);

    return window;
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

template<> void luaw_push(lua_State* L, Size const& s)
{
    lua_newtable(L);
    lua_pushinteger(L, s.w); lua_setfield(L, -2, "w");
    lua_pushinteger(L, s.h); lua_setfield(L, -2, "h");
}
