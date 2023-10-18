#ifndef TYPES_HH_
#define TYPES_HH_

#include <cstdio>
#include <cstdint>
extern "C" {
#include <lua.h>
}

#include "../luaw.hh"

struct Point {
    int16_t x, y;
};
template<> Point luaw_to(lua_State* L, int index);

struct Size {
    uint16_t w, h;
};
template<> Size luaw_to(lua_State* L, int index);

struct Rectangle {
    int16_t x, y;
    uint16_t w, h;
};
template<> void luaw_push(lua_State* L, Rectangle const& r);
template<> Rectangle luaw_to(lua_State* L, int index);

struct Color {
    uint8_t r, g, b;
    bool is_white() const { return r == 255 && g == 255 && b == 255; }
    bool is_black() const { return r == 0 && g == 0 && b == 0; }
    bool operator <(Color const& c) const {
        return ((((uint32_t) r) << 16) | (((uint32_t) g) << 8) | b) < ((((uint32_t) c.r) << 16) | (((uint32_t) c.g) << 8) | c.b);
    }
};
template<> Color luaw_to(lua_State* L, int index);

struct Window {
    uintptr_t outer_id;
    uintptr_t child_id;
};
template<> Window luaw_to(lua_State* L, int index);

#endif //TYPES_HH_
