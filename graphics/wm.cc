#include "wm.hh"
#include "theme/theme.hh"

void WindowManager::run()
{
    add_existing_windows();
    setup_event_listener();
    THEME.call_opt("wm.after_start");
    main_loop();
}

[[noreturn]] void WindowManager::main_loop()
{
    for (;;)
        parse_next_event();
}

void WindowManager::on_create_child(WHandle child_id)
{
    // where to place the new window?
    Rectangle child_rect = get_window_rectangle(child_id);
    Size screen_size = get_screen_size();
    auto [parent_rect, offset] = THEME.get_prop<WindowStartingLocation>("wm.window_starting_location", child_rect, screen_size);

    // create new window
    std::unique_ptr<LWindow> parent_window = create_window(child_rect);
#if 0
    WHandle parent_id = parent_window->id();
    auto [it,_] = windows_.emplace(parent_id, std::move(parent_window));
    L_Window* parent = it->second.get();

    // reparent window
    reparent_window(parent_id, child_id, offset);

    // set properties
    resources_->set_property(parent_id, "child", child_id);
    resources_->set_property(child_id, "parent", parent_id);

    THEME.call_opt("wm.after_window_registered", parent);
#endif
}

void WindowManager::on_destroy_child(WHandle child)
{

}

void WindowManager::on_move_pointer_desktop(Point new_pos)
{

}

void WindowManager::on_click_desktop(ClickEvent const &e)
{

}

void WindowManager::on_expose_window(WHandle parent, Rectangle rectangle)
{

}

void WindowManager::on_click_window(WHandle parent, ClickEvent const &e)
{

}

void WindowManager::on_move_pointer_window(WHandle parent, Point new_rel_pos)
{

}
