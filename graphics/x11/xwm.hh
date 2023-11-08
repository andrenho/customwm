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
    void set_focus(std::optional<LWindow *> window) override;

protected:
    void add_existing_windows() override;
    void setup_event_listener() override;
    void parse_next_event() override;

    [[nodiscard]] Rectangle get_window_rectangle(WHandle window) const override;
    [[nodiscard]] Size get_screen_size() const override;

    [[nodiscard]] std::unique_ptr<LWindow> create_window(Rectangle const &rectangle) const override;

private:
    std::unordered_map<Window, std::unique_ptr<XWindow>> windows_;
    std::optional<std::string> current_hotspot_ {};

    std::optional<XWindow*> moving_window_with_mouse_ {};
    Point last_mouse_position_ { 0, 0 };

    // events
    /*
    void on_map_request(Window child_id);
    void on_unmap_notify(XUnmapEvent const &e);
    void on_expose(XExposeEvent const &e);
    void on_click(XButtonEvent const &e);
    void on_move(XMotionEvent const &xmotion);
    void on_configure(XConfigureEvent const &xconfigure);
     */
    static int on_error(Display* d, XErrorEvent* e);

    XWindow* find_parent(Window parent_id) const;

    XResources* xresources_;
};

#endif //WM_HH_
