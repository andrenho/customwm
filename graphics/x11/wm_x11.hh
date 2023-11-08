#ifndef WMX11_HH_
#define WMX11_HH_

#include "theme/theme.hh"
#include "xwindow.hh"
#include "resources.hh"
#include "graphics/wm.hh"

#include <unordered_map>
#include <X11/Xlib.h>

class WM_X11 : public WM {
public:
    void run() override;
    [[nodiscard]] std::string interface_name() const override { return "X11"; }

    void move_window_with_mouse(bool move, std::optional<L_Window*> window) override;
    void set_focus(std::optional<L_Window *> window) override;

private:
    Resources resources_;
    std::unordered_map<Window, std::unique_ptr<XWindow>> windows_;
    std::optional<std::string> current_hotspot_ {};

    std::optional<XWindow*> moving_window_with_mouse_ {};
    Point last_mouse_position_ { 0, 0 };

    void setup_event_filter();
    void add_existing_windows();

    [[noreturn]] void main_loop();

    // events
    void on_map_request(Window child_id);
    void on_unmap_notify(XUnmapEvent const &e);
    void on_expose(XExposeEvent const &e);
    void on_click(XButtonEvent const &e);
    void on_move(XMotionEvent const &xmotion);
    void on_configure(XConfigureEvent const &xconfigure);
    static int on_error(Display* d, XErrorEvent* e);

    XWindow* find_parent(Window parent_id) const;
};

#endif //WM_HH_
