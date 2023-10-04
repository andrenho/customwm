#ifndef IBRUSH_HH_
#define IBRUSH_HH_

#include "types.hh"
#include "../luaw.hh"

class IBrush {
public:
    virtual ~IBrush() = default;

    virtual void set_color(const char* str) = 0;
    virtual void draw_rect(int x, int y, int w, int h, bool filled) = 0;

    static void create_lua_metatable(lua_State* L);
};

template<> void luaw_push(lua_State* L, IBrush const& w);

#endif //IBRUSH_HH_
