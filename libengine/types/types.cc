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
    int top = lua_gettop(L);

    auto get_idx = [L, index](int idx) {
        lua_geti(L, index, idx);
        int16_t n = (int16_t) luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        return n;
    };
    int16_t x = get_idx(1);
    int16_t y = get_idx(2);

    luaw_asserttop(L, top);

    return { x, y };
}

template<> Size luaw_to(lua_State* L, int index)
{
    int top = lua_gettop(L);

    auto get_idx = [L, index](int idx) {
        lua_geti(L, index, idx);
        uint16_t n = (uint16_t) luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        return n;
    };
    uint16_t w = get_idx(1);
    uint16_t h = get_idx(2);

    luaw_asserttop(L, top);

    return { w, h };
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
    int top = lua_gettop(L);

    luaL_checktype(L, index, LUA_TTABLE);

    auto get_idx = [L, index](int idx) {
        lua_geti(L, index, idx);
        int16_t n = (int16_t) luaL_checkinteger(L, -1);
        lua_pop(L, 1);
        return n;
    };
    Rectangle r = { get_idx(1), get_idx(2), (uint16_t) get_idx(3), (uint16_t) get_idx(4) };
    luaw_asserttop(L, top);
    return r;
}

template<> Window luaw_to(lua_State* L, int index)
{
    int top = lua_gettop(L);

    luaL_checktype(L, index, LUA_TTABLE);

    Window window {};
    lua_getfield(L, -1, "outer_id"); window.outer_id = lua_tointeger(L, -1); lua_pop(L, 1);
    lua_getfield(L, -1, "child_id"); window.child_id = lua_tointeger(L, -1); lua_pop(L, 1);

    luaw_asserttop(L, top);
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

