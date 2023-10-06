#include "luaw.hh"

#include <fstream>
#include <sstream>
#include <string>

#include "exceptions.hh"

void luaw_settop(lua_State* L, int left_in_stack)
{
    lua_pop(L, lua_gettop(L) - left_in_stack);
}

bool luaw_getfield(lua_State* L, int index, const char* k)
{
    luaw_asserttop(L, 1);

    std::istringstream iss(k);
    std::string property;

    while (std::getline(iss, property, '.')) {
        int type = lua_getfield(L, index, property.c_str());
        if (type == LUA_TNIL || (iss.peek() != EOF /* is not last */ && type != LUA_TTABLE)) {
            luaw_settop(L, 1);
            return false;
        }
    }

    // final queue should contain: #1 - the theme, #2 - the property
    lua_replace(L, 2);
    lua_settop(L, 2);

    return true;  // property found
}

void luaw_asserttop(lua_State* L, int expected)
{
    if (lua_gettop(L) != expected)
        throw LuaException("Stack size assertion error: expected " + std::to_string(expected) + ", found " + std::to_string(lua_gettop(L)));
}