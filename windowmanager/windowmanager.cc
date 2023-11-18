#include "windowmanager.hh"

#include "graphics/graphics.hh"
#include "theme/theme.hh"

void WindowManager::init()
{
    create_lua_metatable();

    // add_existing_windows();  TODO
    theme_->call_opt("wm.after_start", this);
}

void WindowManager::create_lua_metatable()
{
#define THIS (luaw_to<WindowManager*>(L, 1))
    theme_->create_metatable<WindowManager>({
        { "interface_name", [](lua_State* L) {
            luaw_push(L, THIS->graphics_->interface_name());
            return 1;
        }},
    });
#undef THIS
}
