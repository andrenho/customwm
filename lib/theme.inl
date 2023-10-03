#ifndef THEME_INL_
#define THEME_INL_

#include "exceptions.hh"
#include "luaw.hh"

template<typename T>
T Theme::read(std::string const &prop_name, std::optional<Window const*> w, std::optional<T> default_value) const
{
    if (lua_gettop(L) != 1)
        throw LuaException("reading '" + prop_name + "': stack in incorrect position");

    // get the property
    if (!luaw_getfield(L, -1, prop_name.c_str())) {
        if (default_value)
            return *default_value;
        else
            throw PropertyNotFoundException(prop_name);
    }

    // if the property is a function, run it, put the result back on the stack
    if (lua_type(L, -1) == LUA_TFUNCTION) {
        if (w && *w) {
            luaw_push(L, **w);
            lua_call(L, 1, 1);
        } else {
            throw LuaException("Unexpected function for property " + prop_name);
        }
    }

    // read the value
    try {
        T t = luaw_to<T>(L, -1);
        luaw_popstack(L, 1);
        return t;
    } catch (LuaException& e) {
        throw LuaException(prop_name + ": " + e.what());
    }
}

#endif //THEME_INL_