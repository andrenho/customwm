#ifndef TYPES_HH_
#define TYPES_HH_

#include <variant>

#include "../luaw.hh"

using Handle = uint32_t;

struct Point {
    int x, y;
};
template<> Point luaw_to(lua_State* L, int index);

struct Area {
    int x, y, w, h;
};

struct WindowStartingPos {
    enum { Cascade, Center, Random, Maximized, Requested, Custom } starting_pos;
    Point point;
};
template<> WindowStartingPos luaw_to(lua_State* L, int index);

struct Padding {
    Padding() {}
    Padding(int top, int bottom, int left, int right) : top(top), bottom(bottom), left(left), right(right) {}
    explicit Padding(int value) : top(value), bottom(value), left(value), right(value) {}

    int top, bottom, left, right;
};
template<> Padding luaw_to(lua_State* L, int index);

#endif //TYPES_HH_
