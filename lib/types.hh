#ifndef TYPES_HH_
#define TYPES_HH_

#include <variant>

struct Window {
    uint32_t inner_id;
    uint32_t outer_id;
};

struct Padding {
    Padding() {}
    Padding(int top, int bottom, int left, int right) : top(top), bottom(bottom), left(left), right(right) {}
    Padding(int value) : top(value), bottom(value), left(value), right(value) {}

    int top, bottom, left, right;
};

#endif //TYPES_HH_
