#ifndef TYPES_HH_
#define TYPES_HH_

#include <cstdio>

extern "C" {
#include <lua.h>
}

#include "../luaw.hh"

struct Point {
    int16_t x, y;
};
template<> Point luaw_to(lua_State* L, int index);

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
