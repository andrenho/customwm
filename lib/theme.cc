#include "theme.hh"

#include <fstream>
#include <sstream>

extern "C" {
#include <lualib.h>
#include <lauxlib.h>
}

#include "luaw.hh"
#include "exceptions.hh"

Theme::Theme(const std::string &theme_name)
        : L_ptr(luaL_newstate(), [](lua_State* LL) { lua_close(LL); }), L(L_ptr.get()), theme_file_(theme_name)
{
    load_theme_file();
}

void Theme::load_theme_file()
{
    std::ifstream t(theme_file_.full_path());
    std::stringstream sbuffer;
    sbuffer << t.rdbuf();

    std::string buffer = sbuffer.str();
    int r = luaL_loadbuffer(L, buffer.c_str(), buffer.size(), "theme");
    if (r != LUA_OK)
        throw LuaException("Error loading theme file.");

    if (lua_gettop(L) != 1)
        throw LuaException("The theme file doesn't return anything. Make sure the syntax is `return { ... }`.\n");

    lua_call(L, 0, 1);
    if (lua_type(L, -1) != LUA_TTABLE)
        throw LuaException("The theme file doesn't return a table. Make sure the syntax is `return { ... }`.\n");

    theme_file_.mark_modification_time();
}

void Theme::reload_if_modified()
{
    if (theme_file_.is_modified()) {
        luaw_popstack(L, 0);
        load_theme_file();
        printf("Theme file reloaded.\n");
    }
}