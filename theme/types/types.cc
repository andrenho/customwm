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

bool Rectangle::lua_is(lua_State *L, int index)
{
    return lua_istable(L, index) && luaw_hasfield(L, index, "x") && luaw_hasfield(L, index, "y")
            && luaw_hasfield(L, index, "w") && luaw_hasfield(L, index, "h");
}

Point Point::from_lua(lua_State *L, int index)
{
    return {
            .x = luaw_getfield<int32_t>(L, index, "x"),
            .y = luaw_getfield<int32_t>(L, index, "y"),
    };
}

bool Point::lua_is(lua_State *L, int index)
{
    return lua_istable(L, index) && luaw_hasfield(L, index, "x") && luaw_hasfield(L, index, "y");
}

void Size::to_lua(lua_State *L) const
{
    lua_newtable(L);
    luaw_setfield(L, -1, "w", w);
    luaw_setfield(L, -1, "h", h);
}

bool Size::lua_is(lua_State *L, int index)
{
    return lua_istable(L, index) && luaw_hasfield(L, index, "w") && luaw_hasfield(L, index, "h");
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

bool Color::lua_is(lua_State *L, int index)
{
    return lua_isstring(L, index) && luaw_len(L, index) == 7 && luaw_to<std::string>(L, index).starts_with("#");
}

TextProperties TextProperties::from_lua(lua_State* L, int index)
{
    TextProperties tp;

    if (luaw_hasfield(L, index, "font"))
        tp.font = luaw_getfield<std::string>(L, index, "font");
    if (luaw_hasfield(L, index, "color"))
        tp.color = luaw_getfield<Color>(L, index, "color");
    if (luaw_hasfield(L, index, "w"))
        tp.w = luaw_getfield<uint32_t>(L, index, "w");
    if (luaw_hasfield(L, index, "h"))
        tp.h = luaw_getfield<uint32_t>(L, index, "h");
    
    if (luaw_hasfield(L, index, "halign")) {
        std::string halign = luaw_getfield<std::string>(L, index, "halign");
        if (halign == "left")
            tp.halign = Left;
        else if (halign == "center")
            tp.halign = HCenter;
        else if (halign == "right")
            tp.halign = Right;
        else
            luaL_error(L, "Invalid value for halign field: %s", halign.c_str());
    }

    if (luaw_hasfield(L, index, "valign")) {
        std::string valign = luaw_getfield<std::string>(L, index, "valign");
        if (valign == "top")
            tp.valign = Top;
        else if (valign == "center")
            tp.valign = VCenter;
        else if (valign == "bottom")
            tp.valign = Bottom;
        else
            luaL_error(L, "Invalid value for valign field: %s", valign.c_str());
    }

    if (luaw_hasfield(L, index, "overflow")) {
        std::string overflow = luaw_getfield<std::string>(L, index, "overflow");
        if (overflow == "visible")
            tp.overflow = Visible;
        else if (overflow == "hidden")
            tp.overflow = Hidden;
        else if (overflow == "ellipsis")
            tp.overflow = Ellipsis;
        else
            luaL_error(L, "Invalid value for overflow field: %s", overflow.c_str());
    }

    return tp;
}

bool TextProperties::lua_is(lua_State *L, int index)
{
    return lua_istable(L, index);
}

Slice Slice::from_lua(lua_State *L, int index)
{
    return {
        .image = luaw_getfield<std::string>(L, index, "image"),
        .rect = luaw_getfield<Rectangle>(L, index, "rect"),
    };
}

bool Slice::lua_is(lua_State *L, int index)
{
    return lua_type(L, index) == LUA_TTABLE &&
           luaw_hasfield(L, index, "image") && luaw_hasfield(L, index, "rect");
}
