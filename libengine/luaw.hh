// Extensions to Lua functions, to support themes

#ifndef LUAW_HH_
#define LUAW_HH_

#include <optional>

extern "C" {
#include <lua.h>
}

void luaw_asserttop(lua_State* L, int expected);
void luaw_settop(lua_State* L, int left_in_stack);

inline void luaw_assertempty(lua_State* L) { luaw_asserttop(L, 0); }

bool luaw_getproperty(lua_State* L, int index, const char* k);

template <typename T> T                luaw_to(lua_State* L, int index);
template <typename T> T                luaw_to(lua_State* L, int index, T const& default_);
template <typename T> std::optional<T> luaw_to_opt(lua_State* L, int index);

template <typename T> void luaw_push(lua_State* L, T const& t);
template <typename T> void luaw_push(lua_State* L, T* t);


#include "luaw.inl"

#endif //LUAW_HH_
