#include "luaw.hh"

#include <optional>

extern "C" {
#include <lauxlib.h>
}

template <typename T>
concept Iterable = requires(T t) {
    begin(t);
    end(t);
    t.push_back(typename T::value_type{});
};

template <Iterable T> T luaw_to(lua_State* L, int index)
{
    int top = lua_gettop(L);

    luaL_checktype(L, index, LUA_TTABLE);

    T ts;
    lua_pushnil(L);
    while (lua_next(L, index) != 0) {
        ts.push_back(luaw_to<typename T::value_type>(L, -1));
        lua_pop(L, 1);
    }

    luaw_asserttop(L, top);

    return ts;
}

template <typename T>
std::optional<T> luaw_to_opt(lua_State* L, int index)
{
    if (lua_isnil(L, index))
        return {};
    return luaw_to<T>(L, index);
}

template <typename T>
T luaw_to(lua_State* L, int index, T const& default_)
{
    if (lua_isnil(L, index))
        return default_;
    return luaw_to<T>(L, index);
}
