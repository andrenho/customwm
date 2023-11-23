#include "gwm.hh"
#include ".old/theme/theme.hh"
#include ".old/common/logger.hh"

void GWindowManager::run()
{
    add_existing_windows();
    setup_event_listener();
    THEME.call_opt("wm.after_start");
    main_loop();
}

[[noreturn]] void GWindowManager::main_loop()
{
    for (;;)
        parse_next_event();
}

void GWindowManager::on_create_child(WHandle child_id)
{
    // where to place the new window?
    Rectangle child_rect = get_window_rectangle(child_id);
    Size screen_size_ = usable_screen_size();
    auto [parent_rect, offset] = THEME.get_prop<WindowStartingLocation>("wm.window_starting_location", child_rect, screen_size_);
    Padding padding = THEME.get_prop<Padding>("wm.padding");

    // create new window
    std::unique_ptr<GWindow> parent_window = create_window(parent_rect);
    WHandle parent_id = parent_window->id();
    auto [it,_] = windows_.emplace(parent_id, std::move(parent_window));
    GWindow* parent = it->second.get();
    LOG.debug("Created parent window id %d", parent->id());

    // reparent window
    reparent_window(parent_id, child_id, offset);
    parent->set_child(child_id, padding);
    parents_[child_id] = parent_id;
    LOG.info("Reparented window %d (parent %d)", child_id, parent_id);

    // set properties
    resources_->set_property(parent_id, "child", child_id);
    resources_->set_property(child_id, "parent", parent_id);

    THEME.call_opt("wm.after_window_registered", parent);

    focus_manager_.set_focus(parent);
}

void GWindowManager::on_destroy_child(WHandle child_id)
{
    auto it = parents_.find(child_id);
    if (it != parents_.end()) {
        window_do(it->second, [&](GWindow* window) {
            parents_.erase(child_id);
            THEME.call_opt("wm.on_window_unregistered", window);
            LOG.debug("Destroyed parent window id %d", window->id());
            focus_manager_.remove_window(window);
            windows_.erase(window->id());
        });
    }
}

void GWindowManager::on_move_pointer(Point new_pos)
{
    grab_manager_.move_pointer(new_pos);
    THEME.call_opt("wm.on_move_pointer", new_pos);
}

void GWindowManager::on_desktop_click(ClickEvent const &e)
{
    THEME.call_opt("wm.on_desktop_click", e);
}

void GWindowManager::on_window_expose(WHandle parent, Rectangle rectangle)
{
    window_do(parent, [&](GWindow* window) {
        THEME.call_opt("wm.on_expose", window, rectangle);
    });
}

void GWindowManager::on_window_click(WHandle window_id, ClickEvent const &e)
{
    // check for clicks on the parent
    window_do(window_id, [&](GWindow* window) {
        focus_manager_.set_focus(window);

        auto hs = hotspot(window, e.pos);
        if (hs)
            THEME.call_opt("wm.on_hotspot_click", window, hs->first, e);

        THEME.call_opt("wm.on_window_click", window, e);
    });

    // check for clicks on the children
    try {
        WHandle parent_id = parents_.at(window_id);
        window_do(parent_id, [&](GWindow* window) {
            focus_manager_.set_focus(window);
        });
    } catch (std::out_of_range&) {}
}

void GWindowManager::on_window_move_pointer(WHandle parent, Point new_rel_pos)
{
    if (grab_manager_.is_active())
        return;

    window_do(parent, [&](GWindow* window) {
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

        THEME.call_opt("wm.on_window_move_pointer", window, new_rel_pos);
    });
}

void GWindowManager::on_window_configure(WHandle window, Rectangle rectangle)
{
    window_do(window, [&](GWindow* window) {
        window->update_rectangle(rectangle);
    });
}

std::optional<std::pair<std::string, Hotspot>> GWindowManager::hotspot(GWindow* window, Point const& p) const
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

void GWindowManager::grab(LWindow *window, GrabType grab_type, Point const& initial_pos)
{
    grab_manager_.set_grab((GWindow *) window, grab_type, initial_pos);
}

void GWindowManager::set_focus(std::optional<LWindow *> window)
{
    focus_manager_.set_focus(window.has_value() ? (GWindow *) *window : std::optional<GWindow *> {});
}

void GWindowManager::window_do(WHandle handle, std::function<void(GWindow *)> const &f)
{
    auto it = windows_.find(handle);
    if (it != windows_.end())
        f(it->second.get());
}
