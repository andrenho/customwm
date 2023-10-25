#include "types.hh"

#include "../../contrib/luaw/luaw/luaw.hh"

Rectangle Rectangle::from_lua(struct lua_State *L, int index)
{
    return {
            .x = luaw_getfield<int32_t>(L, index, "x"),
            .y = luaw_getfield<int32_t>(L, index, "y"),
            .w = luaw_getfield<uint32_t>(L, index, "w"),
            .h = luaw_getfield<uint32_t>(L, index, "h"),
    };
}

Point Point::from_lua(struct lua_State *L, int index)
{
    return {
            .x = luaw_getfield<int32_t>(L, index, "x"),
            .y = luaw_getfield<int32_t>(L, index, "y"),
    };
}

WindowStartingLocation WindowStartingLocation::from_lua(struct lua_State *L, int index)
{
    return {
            .parent_rect = luaw_getfield<Rectangle>(L, index, "parent_rect"),
            .child_offset = luaw_getfield<Point>(L, index, "child_offset"),
    };
}
