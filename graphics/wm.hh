#ifndef WM_HH_
#define WM_HH_

#include <map>
#include <unordered_map>

#include "theme/types/lwm.hh"
#include "resources.hh"
#include "focusmanager.hh"
#include "grabmanager.hh"

class WindowManager : public LWindowManager {
public:
    explicit WindowManager(std::unique_ptr<Resources> resources)
        : resources_(std::move(resources)), focus_manager_(this), grab_manager_(this) {}

    void run();

    // overwritten from parent
    void set_focus(std::optional<LWindow *> window) override { focus_manager_.set_focus(window); }
    void grab(LWindow *window, GrabType grab_type) override { grab_manager_.set_grab(window, grab_type); }

    // to be overwritten in library specific code
    virtual void expose(LWindow* window) = 0;
    virtual void bring_window_to_front(LWindow* window) = 0;

    // getters
    [[nodiscard]] FocusManager const& focus_manager() const { return focus_manager_; }

protected:

    // to be overwritten in library specific code
    virtual void add_existing_windows() = 0;
    virtual void setup_event_listener() = 0;
    virtual void parse_next_event() = 0;
    virtual void reparent_window(WHandle parent_id, WHandle child_id, Point const& offset) = 0;

    [[nodiscard]] virtual Rectangle get_window_rectangle(WHandle window) const = 0;
    [[nodiscard]] virtual Size      get_screen_size() const = 0;

    [[nodiscard]] virtual std::unique_ptr<LWindow> create_window(Rectangle const& rectangle) const = 0;

    // desktop events
    void on_create_child(WHandle child_id);
    void on_destroy_child(WHandle child);
    void on_move_pointer(Point new_pos);
    void on_desktop_click(ClickEvent const& e);

    // window events
    void on_window_expose(WHandle parent, Rectangle rectangle);
    void on_window_click(WHandle window_id, ClickEvent const& e);
    void on_window_move_pointer(WHandle parent, Point new_rel_pos);
    void on_window_configure(WHandle window, Rectangle rectangle);

    // shared internal fields
    std::unique_ptr<Resources> resources_;

private:
    std::unordered_map<WHandle, std::unique_ptr<LWindow>> windows_;
    std::unordered_map<WHandle, WHandle>                  parents_;  // [child] = parent

    std::optional<std::string> current_hotspot_ {};

    FocusManager focus_manager_;
    GrabManager  grab_manager_;

    [[noreturn]] void main_loop();
    std::optional<std::pair<std::string, Hotspot>> hotspot(LWindow* window, Point const& p) const;
};

#endif //WM_HH_
