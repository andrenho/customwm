// Extensions to Lua functions, to support themes

#ifndef LUAW_HH_
#define LUAW_HH_

extern "C" {
#include <lua.h>
}

void luaw_settop(lua_State* L, int left_in_stack);

bool luaw_getfield(lua_State* L, int index, const char* k);

template <typename T>
T luaw_to(lua_State* L, int index);

template <typename T>
void luaw_push(lua_State* L, T const& t);

void luaw_asserttop(lua_State* L, int expected);

#endif //LUAW_HH_
