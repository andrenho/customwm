#include "types.hh"

#include "contrib/luaw/luaw/luaw.hh"

Rectangle Rectangle::from_lua(lua_State *L, int index)
{
    return {
            .x = luaw_getfield<int32_t>(L, index, "x"),
            .y = luaw_getfield<int32_t>(L, index, "y"),
            .w = luaw_getfield<int32_t>(L, index, "w"),
            .h = luaw_getfield<int32_t>(L, index, "h"),
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

bool Rectangle::contains(Point const &p) const
{
    return p.x >= x && p.y >= y && p.x < (int32_t) (x + w) && p.y < (int32_t) (y + h);
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

void Point::to_lua(lua_State *L) const
{
    lua_newtable(L);
    luaw_setfield(L, -1, "x", x);
    luaw_setfield(L, -1, "y", y);
}

Size Size::from_lua(lua_State *L, int index)
{
    return { luaw_getfield<int32_t>(L, index, "w"), luaw_getfield<int32_t>(L, index, "h") };
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

void ClickEvent::to_lua(lua_State *L) const
{
    lua_newtable(L);
    luaw_setfield(L, -1, "pressed", pressed);
    luaw_setfield(L, -1, "pos", pos);
    luaw_setfield(L, -1, "abs_pos", abs_pos);
    switch (button) {
        case Left:   luaw_setfield(L, -1, "button", "left"); break;
        case Middle: luaw_setfield(L, -1, "button", "middle"); break;
        case Right:  luaw_setfield(L, -1, "button", "right"); break;
        default:     luaw_setfield(L, -1, "button", "other"); break;
    }
}

bool Hotspot::lua_is(lua_State *L, int index)
{
    return lua_type(L, index) == LUA_TTABLE &&
           luaw_hasfield(L, index, "area");
}

Hotspot Hotspot::from_lua(lua_State *L, int index)
{
    Hotspot hs {};
    hs.area = luaw_getfield<Rectangle>(L, index, "area");
    if (luaw_hasfield(L, index, "cursor"))
        hs.cursor = luaw_getfield<std::optional<std::string>>(L, index, "cursor");
    return hs;
}

Padding Padding::from_lua(lua_State *L, int index)
{
    return {
        .top = luaw_getfield<uint32_t>(L, index, "top"),
        .left = luaw_getfield<uint32_t>(L, index, "left"),
        .bottom = luaw_getfield<uint32_t>(L, index, "bottom"),
        .right = luaw_getfield<uint32_t>(L, index, "right"),
    };
}

bool Padding::lua_is(lua_State *L, int index)
{
    return lua_type(L, index) == LUA_TTABLE &&
           luaw_hasfield(L, index, "top") && luaw_hasfield(L, index, "left") &&
           luaw_hasfield(L, index, "bottom") && luaw_hasfield(L, index, "right");
}

void Padding::to_lua(lua_State* L) const
{
    lua_newtable(L);
    luaw_setfield(L, -1, "top", top);
    luaw_setfield(L, -1, "left", left);
    luaw_setfield(L, -1, "bottom", bottom);
    luaw_setfield(L, -1, "right", right);
}
