#ifndef TYPES_HH_
#define TYPES_HH_

#include <cstdint>
#include <string>

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

struct Color {
    uint8_t r, g, b, a = 255;
    [[nodiscard]] bool is_white() const { return r == 255 && g == 255 && b == 255 & a == 255; }
    [[nodiscard]] bool is_black() const { return r == 0 && g == 0 && b == 0 & a == 255; }
    bool operator <(Color const& c) const {  // to use as key for std::map
        return ((((uint32_t) r) << 24) | (((uint32_t) g) << 16) | (((uint32_t) b) << 8) | a) <
               ((((uint32_t) c.r) << 24) | (((uint32_t) c.g) << 16) | (((uint32_t) b) << 8) | a);
    }

    static Color from_lua(lua_State* L, int index);
};

struct WindowStartingLocation {
    Rectangle parent_rect;
    Point     child_offset;

    static WindowStartingLocation from_lua(lua_State* L, int index);
    static bool lua_is(lua_State* L, int index);
};

struct TextProperties {
    std::string                        font = "basic";
    Color                              color { 0, 0, 0 };
    enum { Left, HCenter, Right }      halign = Left;
    enum { Top, VCenter, Bottom }      valign = Top;
    uint32_t                           w = 0;
    uint32_t                           h = 0;
    enum { Visible, Hidden, Ellipsis } overflow = Visible;

    static TextProperties from_lua(lua_State* L, int index);
};

#endif //TYPES_HH_
