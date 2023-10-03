#include "luaw.hh"

#include <fstream>
#include <sstream>

void luaw_popstack(lua_State* L, int left_in_stack)
{
    lua_pop(L, lua_gettop(L) - left_in_stack);
}

bool luaw_getfield(lua_State* L, int index, const char* k)
{
    std::istringstream iss(k);
    std::string property;

    while (std::getline(iss, property, '.')) {
        int type = lua_getfield(L, index, property.c_str());
        if (type == LUA_TNIL || (iss.peek() != EOF /* is not last */ && type != LUA_TTABLE)) {
            luaw_popstack(L, 1);
            return false;
        }
    }

    return true;  // property found
}