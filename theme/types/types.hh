#ifndef TYPES_HH_
#define TYPES_HH_

#include <cstdint>
#include <optional>
#include <string>

struct lua_State;

struct Point {
    int32_t x, y;

    Point operator+(Point const& o) const { return { x + o.x, y + o.y }; }
    Point operator-(Point const& o) const { return { x - o.x, y - o.y }; }

    Point operator+(struct Size const& o) const;

    static Point from_lua(lua_State* L, int index);
    static bool lua_is(lua_State* L, int index);
    void to_lua(lua_State* L) const;
};

struct Size {
    int32_t w, h;

    Size(int32_t w, int32_t h) : w(w), h(h) {}
    Size(Point const& p) : w(p.x), h(p.y) {}

    bool operator==(Size const& sz) const { return sz.w == w && sz.h == h; }
    Size operator+(Size const& o) const { return { w + o.w, h + o.h }; }
    Size operator-(Size const& o) const { return { w - o.w, h - o.h }; }

    static Size from_lua(lua_State* L, int index);
    void to_lua(lua_State* L) const;
    static bool lua_is(lua_State* L, int index);
};

inline Point Point::operator+(Size const& o) const { return { x + (int32_t) o.w, y + (int32_t) o.h }; }

struct Rectangle {
    int32_t x, y;
    int32_t w, h;

    bool  contains(Point const& p) const;
    Point topleft() const { return { x, y }; }
    Size  size() const { return { w, h }; }

    void to_lua(lua_State* L) const;
    static Rectangle from_lua(lua_State* L, int index);
    static bool lua_is(lua_State* L, int index);

    static Rectangle from_size(Size const& sz) { return { 0, 0, sz.w, sz.h }; }
};

struct Color {
    uint8_t r, g, b, a = 255;
    [[nodiscard]] bool is_white() const { return (r == 255) && (g == 255) && (b == 255) & (a == 255); }
    [[nodiscard]] bool is_black() const { return (r == 0) && (g == 0) && (b == 0) & (a == 255); }

    bool operator==(const Color& o) const {
        return (r == o.r) && (g == o.g) && (b == o.b) && (a == o.a);
    }

    static Color from_lua(lua_State* L, int index);
    static bool lua_is(lua_State* L, int index);
};

template<>
struct std::hash<Color> {
    size_t operator()(Color const& c) const {
        return (((uint32_t) c.r) << 24) | (((uint32_t) c.g) << 16) | (((uint32_t) c.b) << 8) | c.a;
    }
};

struct Padding {
    uint32_t top;
    uint32_t left;
    uint32_t bottom;
    uint32_t right;

    static Padding from_lua(lua_State* L, int index);
    static bool lua_is(lua_State* L, int index);
    void to_lua(lua_State* L) const;
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
    static bool lua_is(lua_State* L, int index);
};

struct Slice {
    std::string image;
    Rectangle   rect;

    static Slice from_lua(lua_State* L, int index);
    static bool lua_is(lua_State* L, int index);
};

struct ClickEvent {
    bool  pressed;
    Point pos;
    Point abs_pos;
    enum { Other, Left, Middle, Right } button;

    void to_lua(lua_State* L) const;
};

struct Hotspot {
    Rectangle area;
    std::optional<std::string> cursor;

    static Hotspot from_lua(lua_State* L, int index);
    static bool lua_is(lua_State* L, int index);
};

#endif //TYPES_HH_
