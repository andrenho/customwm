#include "l_window.hh"

#include "luaw.hh"
#include "theme/logger.hh"

void L_Window::to_lua(lua_State* L) const
{
    lua_newtable(L);
    luaw_setfield(L, -1, "parent_id", parent_id);
    luaw_setfield(L, -1, "child_id", child_id);
    luaw_setfield(L, -1, "x", x);
    luaw_setfield(L, -1, "y", y);
    luaw_setfield(L, -1, "w", w);
    luaw_setfield(L, -1, "h", h);
}

std::string L_Window::to_string() const
{
    char buf[1024];
    snprintf(buf, sizeof buf, "{ parent_id: %zu, child_id: %zu, x: %d, y: %d, w: %d, h: %d }",
            parent_id, child_id, x, y, w, h);
    return buf;
}

void l_window_create_metadata(lua_State* L)
{
    std::string mt = luaw_set_metatable<L_Window>(L, {
        { "parent_id", [](lua_State *L) { return luaw_push(L, luaw_to<L_Window *>(L, 1)->parent_id); } },
        { "child_id", [](lua_State *L) { return luaw_push(L, luaw_to<L_Window *>(L, 1)->child_id); } },
        { "x", [](lua_State *L) { return luaw_push(L, luaw_to<L_Window *>(L, 1)->x); } },
        { "y", [](lua_State *L) { return luaw_push(L, luaw_to<L_Window *>(L, 1)->y); } },
        { "w", [](lua_State *L) { return luaw_push(L, luaw_to<L_Window *>(L, 1)->w); } },
        { "h", [](lua_State *L) { return luaw_push(L, luaw_to<L_Window *>(L, 1)->h); } },
        { "__tostring", [](lua_State *L) { return luaw_push(L, luaw_to<L_Window *>(L, 1)->to_string()); } },
    });
    LOG.debug("Metatable setup for Window as '%s'", mt.c_str());
}