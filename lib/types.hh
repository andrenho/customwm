#ifndef TYPES_HH_
#define TYPES_HH_

#include <variant>

struct Point {
    int16_t x, y;
};

struct Window {
    uint32_t inner_id;
    uint32_t outer_id;
};

struct WindowStartingPos {
    enum { Cascade, Center, Random, Maximized, Requested, Custom } starting_pos;
    Point point;
};

struct Padding {
    Padding() {}
    Padding(int16_t top, int16_t bottom, int16_t left, int16_t right) : top(top), bottom(bottom), left(left), right(right) {}
    Padding(int16_t value) : top(value), bottom(value), left(value), right(value) {}

    int16_t top, bottom, left, right;
};

#endif //TYPES_HH_
