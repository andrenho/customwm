#include "theme.hh"

#include <sys/stat.h>

#include "exceptions.hh"
#include "luaw.hh"
#include "types/interfaces.hh"

Theme::Theme()
    : L(luaL_newstate())
{
    luaL_openlibs(L);

    IWindow::create_metatable(L);

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
        throw LuaException(L, "error loading " + name + " file");

    if (lua_pcall(L, 0, 1, 0) != LUA_OK)
        throw LuaException(L, "error running " + name + " file");

    merge_theme();
}

void Theme::load_from_file(std::string const& file)
{
    struct stat buffer {};
    if (stat(file.c_str(), &buffer) == 0)
        last_modification_ = buffer.st_mtime;

    int r = luaL_loadfile(L, file.c_str());
    if (r == LUA_ERRFILE)
        throw LuaException(L, "Could not open theme file.");
    if (r != LUA_OK)
        throw LuaException(L, "Error loading theme file.");
    if (lua_pcall(L, 0, 1, 0) != LUA_OK)
        throw LuaException(L, "Error running theme file.");

    loaded_file_ = file;

    merge_theme();
}

bool Theme::theme_file_modified() const
{
    struct stat buffer {};
    if (!loaded_file_.empty() && (stat(loaded_file_.c_str(), &buffer) == 0))
        return buffer.st_mtime > last_modification_;
    return false;
}

void Theme::merge_theme()
{
    luaw_asserttop(L, 1);
    if (lua_type(L, -1) != LUA_TTABLE)
        throw LuaException(L, "Not a valid theme file. The theme file is a lua script with the following format: `return { ... }`.");

    lua_getglobal(L, "theme");

    lua_pushnil(L);
    while (lua_next(L, -3) != 0) {  // get next key from the user supplied theme
        lua_pushvalue(L, -2);        // copy key
        lua_insert(L, 3);
        lua_settable(L, -4);        // set field in "theme", the key is left over
    }

    lua_pop(L, 2);  // remove the table in the original script
    luaw_assertempty(L);
}

std::vector<std::string> Theme::keys(std::string const& parent_prop) const
{
    int top = lua_gettop(L);

    std::vector<std::string> keys;

    lua_getglobal(L, "theme");

    if (!luaw_getproperty(L, -1, parent_prop.c_str()))
        throw PropertyNotFoundException(parent_prop);

    luaL_checktype(L, -1, LUA_TTABLE);

    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        if (lua_type(L, -2) == LUA_TSTRING)
            keys.push_back(lua_tostring(L, -2));
        lua_pop(L, 1);
    }

    lua_pop(L, 2);

    luaw_asserttop(L, top);

    return keys;
}
