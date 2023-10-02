#ifndef WM_HH_
#define WM_HH_

#include <memory>
#include <optional>
#include <unordered_map>

#include "ievents.hh"
#include "../x11/x11.hh"
#include "../../lib/theme.hh"
#include "../../lib/window.hh"

class WM : public IEvents {
public:
    WM(std::string const& display_name, std::string const& theme_name);

    void run();

    void on_create_window(Handle window_id) override;
    void on_destroy_window(Handle window_id) override;
    void on_expose_window(Handle window_id, Area area) override;

protected:
    Point window_starting_pos(Window const& w, Area const& window_sz, Area const& scr_sz, Padding const& pad) const;

    std::optional<Window*> find_window(Handle id) const;

    X11                x11_;
    Theme              theme_;
    std::unordered_map<size_t, Window> windows_;

    mutable uint8_t cascade_ = 0;
};

#endif //WM_HH_
