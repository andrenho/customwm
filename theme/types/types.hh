#ifndef TYPES_HH_
#define TYPES_HH_

#include <cstdint>

struct lua_State;

struct Rectangle {
    int32_t x, y;
    uint32_t w, h;

    void to_lua(lua_State* L) const;
    static Rectangle from_lua(lua_State* L, int index);
};

struct Point {
    int32_t x, y;

    static Point from_lua(lua_State* L, int index);
};

struct Size {
    uint32_t w, h;

    void to_lua(lua_State* L) const;
};

struct WindowStartingLocation {
    Rectangle parent_rect;
    Point     child_offset;

    static WindowStartingLocation from_lua(lua_State* L, int index);
    static bool lua_is(lua_State* L, int index);
};

#endif //TYPES_HH_
