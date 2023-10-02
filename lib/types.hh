#ifndef TYPES_HH_
#define TYPES_HH_

#include <variant>

using Handle = uint32_t;

struct Point {
    int x, y;
};

struct Area {
    int x, y, w, h;
};

struct WindowStartingPos {
    enum { Cascade, Center, Random, Maximized, Requested, Custom } starting_pos;
    Point point;
};

struct Padding {
    Padding() {}
    Padding(int top, int bottom, int left, int right) : top(top), bottom(bottom), left(left), right(right) {}
    explicit Padding(int value) : top(value), bottom(value), left(value), right(value) {}

    int top, bottom, left, right;
};

#endif //TYPES_HH_
