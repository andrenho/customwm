#include "types.hh"

#include "../../contrib/luaw/luaw/luaw.hh"

Rectangle Rectangle::from_lua(lua_State *L, int index)
{
    return {
            .x = luaw_getfield<int32_t>(L, index, "x"),
            .y = luaw_getfield<int32_t>(L, index, "y"),
            .w = luaw_getfield<uint32_t>(L, index, "w"),
            .h = luaw_getfield<uint32_t>(L, index, "h"),
    };
}

void Rectangle::to_lua(lua_State *L) const
{
    lua_newtable(L);
    luaw_setfield(L, -1, "x", x);
    luaw_setfield(L, -1, "y", y);
    luaw_setfield(L, -1, "w", w);
    luaw_setfield(L, -1, "h", h);
}

Point Point::from_lua(lua_State *L, int index)
{
    return {
            .x = luaw_getfield<int32_t>(L, index, "x"),
            .y = luaw_getfield<int32_t>(L, index, "y"),
    };
}

void Size::to_lua(lua_State *L) const
{
    lua_newtable(L);
    luaw_setfield(L, -1, "w", w);
    luaw_setfield(L, -1, "h", h);
}

WindowStartingLocation WindowStartingLocation::from_lua(lua_State *L, int index)
{
    return {
            .parent_rect = luaw_getfield<Rectangle>(L, index, "parent_rect"),
            .child_offset = luaw_getfield<Point>(L, index, "child_offset"),
    };
}

bool WindowStartingLocation::lua_is(lua_State *L, int index)
{
    return lua_type(L, index) == LUA_TTABLE &&
            luaw_hasfield(L, index, "parent_rect") && luaw_hasfield(L, index, "child_offset");
}

