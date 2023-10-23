#ifndef ENGINE_INL_
#define ENGINE_INL_

#include <string>

#include "luaw.hh"
#include "exceptions.hh"

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

template<typename T, typename... Types>
T Engine::read(std::string const &prop_name, Types&... args) const
{
    int top = lua_gettop(L);

    lua_getglobal(L, "get_property");
    lua_pushstring(L, prop_name.c_str());
    ([&] { luaw_push(L, args); } (), ...);
    lua_call(L, sizeof...(args) + 1, 1);

    // read the value
    try {
        T t = luaw_to<T>(L, -1);
        lua_pop(L, 1);
        luaw_asserttop(L, top);
        return t;
    } catch (LuaException& e) {
        throw LuaException(L, prop_name + ": " + e.what());
    }
}

template<typename... Types>
void Engine::call(std::string const& prop_name, Types&... args)
{
    int top = lua_gettop(L);

    lua_getglobal(L, "call_property");
    lua_pushstring(L, prop_name.c_str());
    ([&] { luaw_push(L, args); } (), ...);
    lua_call(L, sizeof...(args) + 1, 0);

    luaw_asserttop(L, top);
}

template<typename... Types>
void Engine::call_opt(std::string const& prop_name, Types&... args) {

    int top = lua_gettop(L);

    lua_getglobal(L, "call_property_opt");
    lua_pushstring(L, prop_name.c_str());
    ([&] { luaw_push(L, args); } (), ...);
    lua_call(L, sizeof...(args) + 1, 0);

    luaw_asserttop(L, top);
}

#endif
