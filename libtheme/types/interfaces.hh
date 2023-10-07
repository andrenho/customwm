#ifndef INTERFACES_HH_
#define INTERFACES_HH_

#include <string>
#include <vector>

#include "types.hh"
#include "../luaw.hh"

class IWindow {
public:
    virtual Rectangle area() const = 0;
    virtual void draw_rectangles(std::vector<Rectangle> const& rectangles, Color const& color, bool filled) = 0;
    virtual void draw_polygon(std::vector<Point> const& points, Color const& color, bool filled) = 0;

    static void create_metatable(lua_State* L);
};

template<> void luaw_push(lua_State* L, IWindow* window);

#endif //INTERFACES_HH_
