#include "theme.hh"

#include "themeexception.hh"
#include "themehelper.embed"
#include "types/l_wm.hh"
#include "types/l_window.hh"

Theme::Theme()
    : Lptr(luaw_newstate(), [](lua_State* LL) { lua_close(LL); }), L(Lptr.get())
{
    luaw_do_z(L, themehelper_lua);

    l_wm_create_metadata(L);
    l_window_create_metadata(L);
}

void Theme::load_theme(LuaCompressedBytecode lsb[])
{
    int top = lua_gettop(L);

    luaw_do_z(L, lsb, true);

    if (lua_gettop(L) != top + 1)
        throw ThemeException("Theme file does not return a table.");

    merge_theme();
}

void Theme::load_theme_file(std::string const &filename)
{
    luaw_dofile(L, filename, 1);
    merge_theme();
}

void Theme::load_theme_code(std::string const &code)
{
    luaw_do(L, code, 1);
    merge_theme();
}

void Theme::print_effective_theme() const
{
    lua_getglobal(L, THEME_GLOBAL);
    printf("%s\n", luaw_dump(L, -1, true, 10).c_str());
    lua_pop(L, 1);
}

void Theme::merge_theme()
{
    lua_getglobal(L, "merge_theme");
    lua_pushvalue(L, -2);
    lua_call(L, 1, 0);
    lua_pop(L, 1);
}

Theme::Fonts Theme::fonts() const
{
    Fonts tf;

    lua_getglobal(L, THEME_GLOBAL);
    if (!luaw_hasfield(L, -1, "resources.fonts")) {
        lua_pop(L, 1);
        return {};
    }

    luaw_getfield(L, -1, "resources.fonts");

    luaw_spairs(L, -1, [&](lua_State* LL, std::string const& key) {
        if (lua_type(LL, -1) == LUA_TSTRING)
            tf.emplace(key, std::vector<std::string> { luaw_to<std::string>(LL, -1) });
        else if (lua_type(LL, -1) == LUA_TTABLE)
            tf.emplace(key, luaw_to<std::vector<std::string>>(LL, -1));
        else
            luaL_error(LL, "Unexpected type for 'resource.fonts'");
    });

    lua_pop(L, 2);

    return tf;
}