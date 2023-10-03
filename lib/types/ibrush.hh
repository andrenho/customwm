#ifndef IBRUSH_HH_
#define IBRUSH_HH_

#include "types.hh"
#include "../luaw.hh"

class IBrush {
public:
    virtual void set_color(Handle gc, uint8_t r, uint8_t g, uint8_t b) = 0;
};

void ibrush_add_to_lua(lua_State* L, IBrush* ibrush);

#endif //IBRUSH_HH_
