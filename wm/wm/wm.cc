#include "wm.hh"

#include <chrono>
using namespace std::chrono_literals;
using clk = std::chrono::system_clock;

#define CHECK_EVERY 500ms

WM::WM(std::string const& display_name, std::string const& theme_name)
{
    x11_.setup(display_name);
    theme_.load(theme_name);
}

void WM::run()
{
    auto next_theme_check = clk::now() + CHECK_EVERY;

    while (x11_.running()) {
        x11_.do_events(this);

        if (clk::now() > next_theme_check) {
            theme_.reload_if_modified();
            next_theme_check = clk::now() + CHECK_EVERY;
        }
    }
}

void WM::on_create_window(Handle window_id)
{
    // add window to list
    Window& w = windows_.insert({ window_id, { .inner_id = window_id } }).first->second;

    // find window configuration
    Padding padding = theme_.read_padding("window.border_width", w, Padding(1));
    Area window_size = x11_.inner_window_size(w);
    Area screen_size = x11_.screen_size();
    Point pos = window_starting_pos(w, window_size, screen_size, padding);

    // create window
    Handle outer_id = x11_.reparent_window(w, pos, window_size, padding);
    w.outer_id = outer_id;
}

void WM::on_destroy_window(Handle window_id)
{
    auto ow = find_window(window_id);
    if (ow) {
        x11_.destroy_window(*ow);
        windows_.erase(ow->inner_id);
    }
}

void WM::on_expose_window(Handle window_id, Area area)
{

}

Point WM::window_starting_pos(Window const& w, Area const& window_sz, Area const& scr_sz, Padding const& pad) const
{
    WindowStartingPos wsp = theme_.read_starting_pos("wm.window_starting_pos", w);
    switch (wsp.starting_pos) {
        case WindowStartingPos::Cascade: {
            Point p = { cascade_ * pad.left, cascade_ * pad.top };
            ++cascade_;
            if (cascade_ == 5)
                cascade_ = 0;
            return p;
        }
        case WindowStartingPos::Center:
            return { (scr_sz.w / 2) - (window_sz.w / 2), (scr_sz.h / 2) - (window_sz.h / 2) };
        case WindowStartingPos::Random:
            return { rand() % (scr_sz.w / 2), rand() % (scr_sz.h / 2) };
        case WindowStartingPos::Maximized:
            throw std::runtime_error("Not implemented");  // TODO
        case WindowStartingPos::Requested:
            return Point { window_sz.x, window_sz.y };
        case WindowStartingPos::Custom:
            return wsp.point;
    }
}

std::optional<Window> WM::find_window(uint32_t id) const
{
    auto it = windows_.find(id);
    if (it != windows_.end())
        return it->second;

    for (auto const& w: windows_)
        if (w.second.outer_id == id)
            return w.second;

    return {};
}

