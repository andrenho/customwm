#ifndef WM_HH_
#define WM_HH_

#include <map>
#include <unordered_map>

#include "theme/types/lwm.hh"
#include "resources.hh"

class WindowManager : public LWindowManager {
public:
    explicit WindowManager(std::unique_ptr<Resources> resources)
        : resources_(std::move(resources)) {}

    void run();

    [[noreturn]] void main_loop();

protected:
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
    void on_desktop_move_pointer(Point new_pos);
    void on_desktop_click(ClickEvent const& e);

    // window events
    void on_window_expose(WHandle parent, Rectangle rectangle);
    void on_window_click(WHandle parent, ClickEvent const& e);
    void on_window_move_pointer(WHandle parent, Point new_rel_pos);

    std::unique_ptr<Resources> resources_;

private:
    std::unordered_map<WHandle, std::unique_ptr<LWindow>> windows_;
    std::unordered_map<WHandle, WHandle>                  parents_;  // [child] = parent

    std::optional<std::string> current_hotspot_ {};

    std::optional<std::string> hotspot(LWindow* window, Point const& p) const;
};

#endif //WM_HH_
