#include "window.hh"

#include "graphics/graphics.hh"
#include "theme/theme.hh"

Window_::Window_(Theme* theme, Graphics *graphics, Rectangle const &rectangle)
        : theme_(theme), graphics_(graphics), handle_(graphics_->create_window(rectangle)), rectangle_(rectangle),
          pencil_(graphics->create_pencil(this))
{
}

Window_::~Window_()
{
    graphics_->destroy_window(handle_);
}

void Window_::expose(Rectangle const &rectangle)
{
    pencil_->on_expose(rectangle);
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
            Rectangle rectangle = luaw_to<std::optional<Rectangle>>(L, 3)
                    .value_or(Rectangle::from_size(THIS->rectangle_.size()));
            THIS->pencil_->fill(luaw_to<Color>(L, 2), rectangle);
            return 0;
        }},
   });
#undef THIS
}