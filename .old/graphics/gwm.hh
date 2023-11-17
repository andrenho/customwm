#ifndef GWM_HH_
#define GWM_HH_

#include <map>
#include <memory>
#include <unordered_map>

#include ".old/theme/types/lwm.hh"
#include "resources.hh"
#include "focusmanager.hh"
#include "grabmanager.hh"
#include "gwindow.hh"

class GWindowManager : public LWindowManager {
public:
    explicit GWindowManager(std::unique_ptr<Resources> resources)
        : resources_(std::move(resources)), focus_manager_(this) {}

    void run();

    // overwritten from parent
    void set_focus(std::optional<LWindow *> window) override;
    virtual void grab(LWindow *window, GrabType grab_type, Point const& initial_pos) override;
    Size usable_screen_size() const override { return screen_size(); }  // TODO

    // to be overwritten in library specific code
    virtual void expose(GWindow* window) = 0;

    // getters
    [[nodiscard]] FocusManager const& focus_manager() const { return focus_manager_; }

protected:
    // to be overwritten in library specific code
    virtual void add_existing_windows() = 0;
    virtual void setup_event_listener() = 0;
    virtual void parse_next_event() = 0;
    virtual void reparent_window(WHandle parent_id, WHandle child_id, Point const& offset) = 0;

    [[nodiscard]] virtual Rectangle get_window_rectangle(WHandle window) const = 0;

    [[nodiscard]] virtual std::unique_ptr<GWindow> create_window(Rectangle const& rectangle) const = 0;

    // desktop events
    void on_create_child(WHandle child_id);
    void on_destroy_child(WHandle child);
    void on_move_pointer(Point new_pos);
    void on_desktop_click(ClickEvent const& e);

    // window events
    virtual void on_window_expose(WHandle parent, Rectangle rectangle);
    virtual void on_window_configure(WHandle window, Rectangle rectangle);
    void on_window_click(WHandle window_id, ClickEvent const& e);
    void on_window_move_pointer(WHandle parent, Point new_rel_pos);

    std::unique_ptr<Resources> resources_;
    GrabManager  grab_manager_;
    std::unordered_map<WHandle, std::unique_ptr<GWindow>> windows_;

private:
    std::unordered_map<WHandle, WHandle> parents_;  // [child] = parent

    std::optional<std::string> current_hotspot_ {};

    FocusManager focus_manager_;

    [[noreturn]] void main_loop();
    std::optional<std::pair<std::string, Hotspot>> hotspot(GWindow* window, Point const& p) const;

    void window_do(WHandle handle, std::function<void(GWindow *)> const& f);
};

#endif //GWM_HH_
