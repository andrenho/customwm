#ifndef IWINDOW_HH_
#define IWINDOW_HH_

#include <string>
#include <vector>

#include "types.hh"
#include "../luaw.hh"

struct TextBoundingBox {
    Size size;
    enum class HAlignment { LEFT, CENTER, RIGHT } halign;
    enum class VAlignment { TOP, CENTER, BOTTOM } valign;
};

template<> TextBoundingBox luaw_to(lua_State* L, int index);

class IWindow {
public:
    virtual Rectangle area() const = 0;
    virtual void draw_rectangles(std::vector<Rectangle> const& rectangles, Color const& color, bool filled) = 0;
    virtual void draw_polygon(std::vector<Point> const& points, Color const& color, bool filled) = 0;
    virtual void draw_image(Point p, std::string const& image_idx, std::string const& slice) = 0;
    virtual void write(Point p, std::string const& text, std::string const& font, Color const& color,
                       std::optional<TextBoundingBox> attrib) = 0;

    static void create_metatable(lua_State* L);
};

template<> void luaw_push(lua_State* L, IWindow* window);

#endif //IWINDOW_HH_
