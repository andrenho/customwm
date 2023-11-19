#include "window.hh"

#include "graphics/graphics.hh"
#include "theme/theme.hh"

Window_::Window_(Graphics *graphics, Rectangle const &rectangle)
        : graphics_(graphics), handle_(graphics_->create_window(rectangle)), rectangle_(rectangle)
{
}

Window_::~Window_()
{
    graphics_->destroy_window(handle_);
}

void Window_::create_lua_metatable(Theme* theme)
{
#define THIS (luaw_to<Window_*>(L, 1))
    theme->create_metatable<Window_>({
        { "handle", [](lua_State* L) {
            luaw_push(L, THIS->handle());
            return 1;
        }},
        { "fill", [](lua_State* L) {
            if (lua_gettop(L) >= 3)
                THIS->fill(luaw_to<Color>(L, 2), luaw_to<std::optional<Rectangle>>(L, 3));
            else
                THIS->fill(luaw_to<Color>(L, 2), {});
            return 0;
        }},
    });
#undef THIS
}

void Window_::fill(Color const &color, std::optional<Rectangle> rect)
{
    Rectangle rectangle = rect.value_or(Rectangle { 0, 0, rectangle_.w, rectangle_.h });
    graphics_->fill(handle_, color, rectangle);
}