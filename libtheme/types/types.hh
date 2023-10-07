#ifndef TYPES_HH_
#define TYPES_HH_

#include <cstdio>
#include <cstdint>
#include <vector>

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

struct WindowStartingPos {
    enum { Cascade, Center, Random, Maximized, Requested, Custom } starting_pos;
    Point point;
};
template<> WindowStartingPos luaw_to(lua_State* L, int index);

struct Padding {
    Padding(int16_t top, int16_t bottom, int16_t left, int16_t right) : top(top), bottom(bottom), left(left), right(right) {}

    int16_t top, bottom, left, right;
};
template<> Padding luaw_to(lua_State* L, int index);

#endif //TYPES_HH_
