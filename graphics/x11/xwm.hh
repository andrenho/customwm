#ifndef WMX11_HH_
#define WMX11_HH_

#include "theme/theme.hh"
#include "xwindow.hh"
#include "xresources.hh"
#include "graphics/wm.hh"

#include <unordered_map>
#include <X11/Xlib.h>

class XWindowManager : public WindowManager {
public:
    XWindowManager();

    [[nodiscard]] std::string interface_name() const override { return "X11"; }

    void move_window_with_mouse(bool move, std::optional<LWindow*> window) override;

protected:
    void add_existing_windows() override;
    void setup_event_listener() override;
    void parse_next_event() override;
    void reparent_window(WHandle parent_id, WHandle child_id, Point const &offset) override;

    [[nodiscard]] Rectangle get_window_rectangle(WHandle window) const override;
    [[nodiscard]] Size get_screen_size() const override;

    [[nodiscard]] std::unique_ptr<LWindow> create_window(Rectangle const &rectangle) const override;

    void expose(LWindow* window) override;

private:
    std::unordered_map<Window, std::unique_ptr<XWindow>> windows_;
    std::optional<std::string> current_hotspot_ {};

    std::optional<XWindow*> moving_window_with_mouse_ {};
    Point last_mouse_position_ { 0, 0 };

    static int on_error(Display* d, XErrorEvent* e);

    XResources* xresources_;
};

#endif //WM_HH_
