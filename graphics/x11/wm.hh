#ifndef WMX11_HH_
#define WMX11_HH_

#include "theme/theme.hh"
#include "xwindow.hh"
#include "resources.hh"
#include "theme/types/l_wm.hh"

#include <unordered_map>
#include <X11/Xlib.h>

struct WM_Window {
    std::unique_ptr<XWindow> parent;
    Window                   child_id;
};

class WM : public L_WM {
public:
    void run();
    [[nodiscard]] std::string interface_name() const override { return "X11"; }

    void move_window_with_mouse(bool move, std::optional<L_Window*> window) override;
    std::string window_name(L_Window *window) const override;

private:
    Resources resources_;
    std::unordered_map<Window, WM_Window> windows_;

    std::optional<XWindow*> moving_window_with_mouse_ {};
    Point last_mouse_position_ { 0, 0 };

    void setup_event_filter();
    void add_existing_windows();

    [[noreturn]] void main_loop();

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
