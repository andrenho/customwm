#include "wm.hh"
#include "theme/theme.hh"
#include "common/logger.hh"

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
    WHandle parent_id = parent_window->id();
    auto [it,_] = windows_.emplace(parent_id, std::move(parent_window));
    LWindow* parent = it->second.get();
    LOG.debug("Created parent window id %d", parent->id());

    // reparent window
    reparent_window(parent_id, child_id, offset);
    parents_[child_id] = parent_id;
    LOG.info("Reparented window %d (parent %d)", child_id, parent_id);

    // set properties
    resources_->set_property(parent_id, "child", child_id);
    resources_->set_property(child_id, "parent", parent_id);

    THEME.call_opt("wm.after_window_registered", parent);
}

void WindowManager::on_destroy_child(WHandle child_id)
{
    auto it = parents_.find(child_id);
    if (it != parents_.end()) {
        try {
            LWindow* window = windows_.at(it->second).get();
            parents_.erase(child_id);
            THEME.call_opt("wm.on_window_unregistered", window);
            LOG.debug("Destroyed parent window id %d", window->id());
            windows_.erase(window->id());
        } catch (std::out_of_range&) {}
    }
}

void WindowManager::on_desktop_move_pointer(Point new_pos)
{

}

void WindowManager::on_desktop_click(ClickEvent const &e)
{
    THEME.call_opt("wm.on_desktop_click", e);
}

void WindowManager::on_window_expose(WHandle parent, Rectangle rectangle)
{
    try {
        LWindow* window = windows_.at(parent).get();
        THEME.call_opt("wm.on_expose", window, rectangle);
    } catch (std::out_of_range&) {}
}

void WindowManager::on_window_click(WHandle parent, ClickEvent const &e)
{
    LWindow* window = windows_.at((WHandle const) parent).get();

    auto hs = hotspot(window, e.pos);
    if (hs)
        THEME.call_opt("wm.on_hotspot_click", window, hs, e);

    THEME.call_opt("wm.on_window_click", window, e);
}

void WindowManager::on_window_move_pointer(WHandle parent, Point new_rel_pos)
{

}

std::optional<std::string> WindowManager::hotspot(LWindow* window, Point const& p) const
{
    try {
        for (auto const& [hs, rect]: THEME.get_prop<std::map<std::string, Rectangle>>("wm.hotspots", window)) {
            if (rect.contains(p))
                return hs;
        }
        return {};
    } catch (std::out_of_range&) {
        return {};
    }
}