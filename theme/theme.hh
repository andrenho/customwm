#ifndef THEME_HH_
#define THEME_HH_

#include <memory>
#include "luaw.hh"

class Theme {
public:
    Theme();

    void load_theme(LuaCompressedBytecode lsb[]);

private:
    std::unique_ptr<lua_State, std::function<void(lua_State*)>> Lptr;
    lua_State* L;
};

#endif //THEME_HH_
