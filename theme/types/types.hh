#ifndef TYPES_HH_
#define TYPES_HH_

#include <cstdint>

struct Rectangle {
    int32_t x, y;
    uint32_t w, h;

    static Rectangle from_lua(struct lua_State* L, int index);
};

struct Point {
    int32_t x, y;

    static Point from_lua(struct lua_State* L, int index);
};

struct WindowStartingLocation {
    Rectangle parent_rect;
    Point     child_offset;

    static WindowStartingLocation from_lua(struct lua_State* L, int index);
};

#endif //TYPES_HH_
