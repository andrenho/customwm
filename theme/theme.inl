#include "themeexception.hh"

template <typename T> T Theme::get_prop(std::string const& prop, auto&&... args) const
{
    lua_getglobal(L, THEME_GLOBAL);
    luaw_getfield(L, -1, prop);

    if (lua_type(L, -1) == LUA_TFUNCTION)
        return luaw_call<T>(L, args...);

    if (!luaw_is<T>(L, -1))
        throw ThemeException("Property '" + prop + "' is of the incorrect type.");
    return luaw_pop<T>(L);
}

void Theme::call_opt(std::string const& prop, auto&&... args)
{
    lua_getglobal(L, THEME_GLOBAL);
    try {
        luaw_getfield(L, -1, prop);
    } catch (LuaException&) {
        return;  // field not found, ignore
    }

    if (lua_type(L, -1) == LUA_TFUNCTION) {
        luaw_call(L, args...);
    } else if (!lua_isnil(L, -1)) {
        throw ThemeException("Property '" + prop + "' is not a function.");
    }
}

template <typename T> void Theme::create_global_object(std::string const& lua_name, T* object)
{
    luaw_push(L, object);
    lua_setglobal(L, lua_name.c_str());
}