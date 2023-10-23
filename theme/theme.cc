#include "theme.hh"

#include "themeexception.hh"

#include "theme-helper.embed"

Theme::Theme()
    : Lptr(luaw_newstate(), [](lua_State* LL) { lua_close(LL); }), L(Lptr.get())
{
    luaw_setglobal(L, "theme", nullptr);
    luaw_do_z(L, theme_helper);
}

void Theme::load_theme(LuaCompressedBytecode lsb[])
{
    int top = lua_gettop(L);

    luaw_do_z(L, lsb, true);

    if (lua_gettop(L) != top + 1)
        throw ThemeException("Theme file does not return a table.");

    luaw_call_global(L, "merge_theme");
}
