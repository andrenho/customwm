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

Color Color::from_lua(lua_State *L, int index)
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
        luaL_error(L, "Invalid color format");
        return {};
    }
}

TextProperties TextProperties::from_lua(lua_State *L, int index)
{
    luaL_checktype(L, index, LUA_TTABLE);

    TextProperties text_properties;
    if (luaw_hasfield(L, index, "font"))
        text_properties.font = luaw_getfield<std::vector<std::string>>(L, index, "font");

    return text_properties;
}
