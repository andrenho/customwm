#include "iwindow.hh"
#include "../exceptions.hh"

extern "C" {
#include <lauxlib.h>
}

struct IWindowUserData {
    IWindow* iwindow;
};

static IWindow& get_window(lua_State* L, int idx) {
    luaL_checkudata(L, idx, "__iwindow_mt");
    return *((IWindowUserData *) lua_touserdata(L, idx))->iwindow;
}

static luaL_Reg iwindow_f[] = {
        { "area", [](lua_State *L) {
            luaw_push(L, get_window(L, 1).area());
            return 1;
        } },
        { "name", [](lua_State *L) {
            lua_pushstring(L, get_window(L, 1).name().c_str());
            return 1;
        } },
        { "draw_rectangle", [](lua_State *L) {
            get_window(L, 1).draw_rectangles({ luaw_to<Rectangle>(L, 2) }, luaw_to<Color>(L, 3), lua_toboolean(L, 4));
            return 0;
        } },
        { "draw_rectangles", [](lua_State *L) {
            get_window(L, 1).draw_rectangles({ luaw_to<std::vector<Rectangle>>(L, 2) }, luaw_to<Color>(L, 3), lua_toboolean(L, 4));
            return 0;
        } },
        { "draw_polygon", [](lua_State *L) {
            get_window(L, 1).draw_polygon(luaw_to<std::vector<Point>>(L, 2), luaw_to<Color>(L, 3), lua_toboolean(L, 4));
            return 0;
        } },
        { "draw_line", [](lua_State *L) {
            get_window(L, 1).draw_polygon({ luaw_to<Point>(L, 2), luaw_to<Point>(L, 3) }, luaw_to<Color>(L, 4), false);
            return 0;
        } },
        { "draw_image", [](lua_State *L) {
            std::string image_slice = luaL_checkstring(L, 3);
            size_t pos = image_slice.find_first_of('.');
            std::string slice = image_slice.substr(pos + 1), image = image_slice.substr(0, pos);
            get_window(L, 1).draw_image(luaw_to<Point>(L, 2), image, slice);
            return 0;
        } },
        { "write", [](lua_State *L) {
            get_window(L, 1).write(
                    luaw_to<Point>(L, 2), luaL_checkstring(L, 3), luaL_checkstring(L, 4), luaw_to<Color>(L, 5),
                    lua_gettop(L) >= 6 ? luaw_to<TextBoundingBox>(L, 6) : std::optional<TextBoundingBox> {});
            return 0;
        } },
        {nullptr, nullptr},
};

void IWindow::create_metatable(lua_State *L)
{
    luaL_newmetatable(L, "__iwindow_mt");
    luaL_setfuncs(L, iwindow_f, 0);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
}

template<> void luaw_push(lua_State* L, IWindow* window)
{
    IWindowUserData* ud = (IWindowUserData *) lua_newuserdata(L, sizeof(IWindowUserData));
    ud->iwindow = window;
    luaL_setmetatable(L, "__iwindow_mt");
}

template<> TextBoundingBox luaw_to(lua_State* L, int index)
{
    int top = lua_gettop(L);

    TextBoundingBox attr = {
            .size = { 0, 0 },
            .halign = TextBoundingBox::HAlignment::LEFT,
            .valign = TextBoundingBox::VAlignment::BOTTOM
    };

    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, index, "size");
    attr.size = luaw_to<Size>(L, -1, { 0, 0 });
    lua_pop(L, 1);

    lua_getfield(L, index, "halign");
    if (!lua_isnil(L, -1)) {
        std::string align = luaL_checkstring(L, -1);
        if (align == "left")
            attr.halign = TextBoundingBox::HAlignment::LEFT;
        else if (align == "center")
            attr.halign = TextBoundingBox::HAlignment::CENTER;
        else if (align == "right")
            attr.halign = TextBoundingBox::HAlignment::RIGHT;
        else
            throw LuaException(L, "Incorrect horizontal alignment.");
    }
    lua_pop(L, 1);

    lua_getfield(L, index, "valign");
    if (!lua_isnil(L, -1)) {
        std::string align = luaL_checkstring(L, -1);
        if (align == "top")
            attr.valign = TextBoundingBox::VAlignment::TOP;
        else if (align == "center")
            attr.valign = TextBoundingBox::VAlignment::CENTER;
        else if (align == "bottom")
            attr.valign = TextBoundingBox::VAlignment::BOTTOM;
        else
            throw LuaException(L, "Incorrect horizontal alignment.");
    }
    lua_pop(L, 1);

    return attr;
}