#include "theme.hh"

#include "exceptions.hh"
#include "luaw.hh"

Theme::Theme()
    : L(luaL_newstate())
{
    luaL_openlibs(L);

    lua_newtable(L);
    lua_setglobal(L, "theme");
}

Theme::~Theme()
{
    lua_close(L);
}

void Theme::load_from_ram(std::string const& name, uint8_t const* data, unsigned int len)
{
    if (luaL_loadbufferx(L, (const char *) data, len, name.c_str(), "b") != LUA_OK)
        throw LuaException("error loading " + name + " file");

    if (lua_pcall(L, 0, 1, 0) != LUA_OK)
        throw LuaException("error running " + name + " file");

    merge_theme();
}

void Theme::merge_theme()
{
    luaw_asserttop(L, 1);
    if (lua_type(L, -1) != LUA_TTABLE)
        throw LuaException("Not a valid theme file. The theme file is a lua script with the following format: `return { ... }`.");

    lua_getglobal(L, "theme");

    lua_pushnil(L);
    while (lua_next(L, -3) != 0) {  // get next key from the user supplied theme
        lua_pushvalue(L, -2);        // copy key
        lua_insert(L, 3);
        lua_settable(L, -4);        // set field in "theme", the key is left over
        printf("%s\n", lua_tostring(L, -1));
    }

    lua_pop(L, 2);  // remove the table in the original script
    luaw_assertempty(L);
}
