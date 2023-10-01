#ifndef TYPES_HH_
#define TYPES_HH_

#include <variant>

struct Window {};

struct LuaWindowFunction {};

struct VariablePadding {
    int top, bottom, left, right;
};

using Padding = std::variant<int, VariablePadding, LuaWindowFunction>;

#endif //TYPES_HH_
