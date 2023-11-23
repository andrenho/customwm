#ifndef WMX11_HH_
#define WMX11_HH_

#include ".old/theme/theme.hh"
#include "xwindow.hh"
#include "xresources.hh"
#include ".old/graphics/gwm.hh"

#include <unordered_map>
#include <X11/Xlib.h>

class XWindowManager : public GWindowManager {
public:
    XWindowManager();

    [[nodiscard]] std::string interface_name() const override { return "X11"; }

    [[nodiscard]] Size screen_size() const override;

protected:
    void add_existing_windows() override;
    void setup_event_listener() override;
    void parse_next_event() override;
    void reparent_window(WHandle parent_id, WHandle child_id, Point const &offset) override;
    void expose(GWindow* window) override;

    std::unique_ptr<GWindow> create_window(Rectangle const &rectangle) const override;

    [[nodiscard]] Rectangle get_window_rectangle(WHandle window) const override;

    // overwritten events
    void on_window_configure(WHandle window, Rectangle rectangle) override;
    void on_window_expose(WHandle parent, Rectangle rectangle) override;

private:
    ClickEvent map_to_click_event(XButtonEvent e) const;
    void propagate_keyevent_to_focused_window(XEvent e) const;

    static int on_error(Display* d, XErrorEvent* e);

    XResources* xresources_;
};

#endif //WM_HH_
