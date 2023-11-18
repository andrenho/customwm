#include "window.hh"

#include "graphics/graphics.hh"
#include "theme/theme.hh"

Window_::Window_(Graphics *graphics, Rectangle const &rectangle)
        : graphics_(graphics), handle_(graphics_->create_window(rectangle))
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
    });
#undef THIS
}
