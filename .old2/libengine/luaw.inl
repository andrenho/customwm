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

template<class P>
concept Pair = requires(P p) {
    typename P::first_type;
    typename P::second_type;
    p.first;
    p.second;
    requires std::same_as<decltype(p.first), typename P::first_type>;
    requires std::same_as<decltype(p.second), typename P::second_type>;
};

template<typename T>
concept NumericType = requires(T param)
{
    requires std::is_integral_v<T>;
};

template< typename T >
concept Optional = requires( T t )
{
    typename T::value_type;
    std::same_as< T, std::optional< typename T::value_type > >;
};


template <Iterable T>
T luaw_to(lua_State* L, int index)
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

template <Pair P>
P luaw_to(lua_State* L, int index)
{
    int top = lua_gettop(L);

    luaL_checktype(L, index, LUA_TTABLE);
    if (luaL_len(L, index) != 2)
        luaL_error(L, "Expected array of size 2.");

    lua_geti(L, index, 1);
    typename P::first_type t = luaw_to<typename P::first_type>(L, -1);
    lua_pop(L, 1);

    lua_geti(L, index, 2);
    typename P::second_type u = luaw_to<typename P::second_type>(L, -1);
    lua_pop(L, 1);

    luaw_asserttop(L, top);

    return { t, u };
}

template <Optional T>
T luaw_to(lua_State* L, int index)
{
    if (lua_isnil(L, index))
        return {};
    return luaw_to<typename T::value_type>(L, index);
}

template <typename T>
T luaw_to(lua_State* L, int index, T const& default_)
{
    if (lua_isnil(L, index))
        return default_;
    return luaw_to<T>(L, index);
}

template <NumericType T> void luaw_push(lua_State* L, T t)
{
    lua_pushinteger(L, t);
}
