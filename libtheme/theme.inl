#ifndef THEME_INL_
#define THEME_INL_

#include "theme.hh"
#include "exceptions.hh"
#include "luaw.hh"

template<typename T, typename... Types>
T Theme::read(std::string const &prop_name, Types&... args) const
{
    int top = lua_gettop(L);

    lua_getglobal(L, "theme");

    // get the property
    if (!luaw_getproperty(L, -1, prop_name.c_str()))
        throw PropertyNotFoundException(prop_name);

    // if the property is a function, run it, put the result back on the stack
    if (lua_type(L, -1) == LUA_TFUNCTION) {
        ([&] {
            luaw_push(L, args);
        } (), ...);
        lua_call(L, sizeof...(args), 0);
    }

    // read the value
    try {
        T t = luaw_to<T>(L, -1);
        lua_pop(L, 2);
        luaw_asserttop(L, top);
        return t;
    } catch (LuaException& e) {
        throw LuaException(L, prop_name + ": " + e.what());
    }
}

template<typename... Types>
void Theme::call(std::string const& prop_name, Types&... args)
{
    int top = lua_gettop(L);

    lua_getglobal(L, "theme");

    // get the property
    if (!luaw_getproperty(L, -1, prop_name.c_str()))
        throw PropertyNotFoundException(prop_name);
    if (lua_type(L, -1) != LUA_TFUNCTION)
        throw LuaException(L, "expected function for property '" + prop_name + "'");

    // push arguments
    ([&] {
        luaw_push(L, args);
    } (), ...);

    // execute function
    lua_call(L, sizeof...(args), 0);

    lua_pop(L, 1);

    luaw_asserttop(L, top);
}

#endif //THEME_INL_
