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
    std::unique_ptr<LWindow> parent_window = create_window(parent_rect);
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

    focus_manager_.set_focus(parent);
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
            focus_manager_.remove_window(window);
            windows_.erase(window->id());
        } catch (std::out_of_range&) {}
    }
}

void WindowManager::on_desktop_move_pointer(Point new_pos)
{
    grab_manager_.move_pointer(new_pos);
    THEME.call_opt("wm.on_desktop_mouse_move", new_pos);
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

void WindowManager::on_window_click(WHandle window_id, ClickEvent const &e)
{
    // check for clicks on the parent
    try {
        LWindow* window = windows_.at((WHandle const) window_id).get();

        focus_manager_.set_focus(window);

        auto hs = hotspot(window, e.pos);
        if (hs)
            THEME.call_opt("wm.on_hotspot_click", window, hs->first, e);

        THEME.call_opt("wm.on_window_click", window, e);
    } catch (std::out_of_range&) {}

    // check for clicks on the children
    try {
        WHandle parent_id = parents_.at(window_id);
        LWindow* window = windows_.at((WHandle const) parent_id).get();
        focus_manager_.set_focus(window);
    } catch (std::out_of_range&) {}
}

void WindowManager::on_window_move_pointer(WHandle parent, Point new_rel_pos)
{
    try {
        LWindow* window = windows_.at(parent).get();

        std::optional<std::string> new_hotspot {};
        auto hs = hotspot(window, new_rel_pos);
        if (hs) {
            new_hotspot = hs->first;

            // check if entering or leaving a hotspot
            if (new_hotspot != current_hotspot_) {
                if (current_hotspot_) {
                    window->set_cursor(Cursors::Pointer);
                    THEME.call_opt("wm.on_leave_hotspot", window, *current_hotspot_);
                }
                if (new_hotspot) {
                    window->set_cursor(hs->second.cursor.value_or(Cursors::Pointer));
                    THEME.call_opt("wm.on_enter_hotspot", window, *new_hotspot);
                }
                current_hotspot_ = new_hotspot;
            }
        }

        THEME.call_opt("wm.on_window_mouse_move", window, new_rel_pos);
    } catch (std::out_of_range&) {}
}

std::optional<std::pair<std::string, Hotspot>> WindowManager::hotspot(LWindow* window, Point const& p) const
{
    try {
        for (auto const& [name, hotspot]: THEME.get_prop<std::map<std::string, Hotspot>>("wm.hotspots", window)) {
            if (hotspot.area.contains(p))
                return std::make_pair(name, hotspot);
        }
        return {};
    } catch (std::out_of_range&) {
        return {};
    }
}