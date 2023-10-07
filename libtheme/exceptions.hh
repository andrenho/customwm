#ifndef EXCEPTIONS_HH_
#define EXCEPTIONS_HH_

#include <stdexcept>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

class PropertyNotFoundException : public std::runtime_error {
public:
    explicit PropertyNotFoundException(std::string const& property)
        : std::runtime_error("Property '" + property + "' not found") {}
};

class LuaException : public std::runtime_error {
public:
    explicit LuaException(lua_State* L, std::string const& error) : std::runtime_error(error) {
        luaL_error(L, "%s", error.c_str());
    }
};

class RestartException : public std::runtime_error {};

#endif //EXCEPTIONS_HH_
