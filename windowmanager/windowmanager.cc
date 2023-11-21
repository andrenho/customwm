#include "windowmanager.hh"

#include <algorithm>

#include "graphics/graphics.hh"
#include "theme/theme.hh"

void WindowManager::init()
{
    theme_->call_opt("wm.after_start", this);
    graphics_->add_existing_windows();
}

void WindowManager::create_lua_metatable(Theme* theme)
{
#define THIS (luaw_to<WindowManager*>(L, 1))
    theme->create_metatable<WindowManager>({
        { "interface_name", [](lua_State* L) {
            luaw_push(L, THIS->graphics_->interface_name());
            return 1;
        }},
    });
#undef THIS
}

void WindowManager::add_child_window(WindowHandle child_handle)
{
    // where to place the new window?
    Rectangle child_rect            = graphics_->get_window_rectangle(child_handle);
    Size      screen_size           = graphics_->screen_size();
    auto      [parent_rect, offset] = theme_->get_prop<WindowStartingLocation>("wm.window_starting_location", this, child_rect, screen_size);
    Padding   padding               = theme_->get_prop<Padding>("wm.padding", this);

    // create new window
    auto parent_uptr = std::make_unique<ParentWindow>(theme_, graphics_, parent_rect);
    ParentWindow* parent = parents_.emplace(parent_uptr->handle(), std::move(parent_uptr)).first->second.get();

    // reparent child window
    parent->reparent_child(child_handle, offset, padding);

    theme_->call_opt("wm.after_window_reparented", this, parent);

    // TODO - set focus
}

void WindowManager::remove_window(WindowHandle window_handle)
{
    for (auto const& [parent_handle, parent_window]: parents_) {
        if (parent_window->child_handle() == window_handle) {
            parents_.erase(parent_handle);
            return;
        }
    }
}

void WindowManager::expose_window(WindowHandle window, Rectangle const& rectangle)
{
    ParentWindow* parent_window = find_parent(window);
    if (parent_window)
        parent_window->expose(rectangle);
}

ParentWindow *WindowManager::find_parent(WindowHandle parent_handle) const
{
    try {
        return parents_.at(parent_handle).get();
    } catch (std::out_of_range&) {
        return nullptr;
    }
}

