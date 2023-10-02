#include "wm.hh"
#include "../graphlib/graphx11.hh"

void WM::run()
{
    graph_ = std::make_unique<GraphX11>();

    graph_->setup((void *) lib_.display().c_str());

    while (graph_->running())
        graph_->do_events(this);
}

void WM::on_create_window(Handle window_id)
{
    // add window to list
    Window& w = windows_.add({ .inner_id = window_id });

    // find window configuration
    Padding padding = lib_.theme().window_padding_width(w, 1);
    Area window_size = graph_->inner_window_size(w);
    Area screen_size = graph_->screen_size();
    Point pos = window_starting_pos(w, window_size, screen_size, padding);

    // create window
    Handle outer_id = graph_->reparent_window(w, pos, window_size, padding);
    w.outer_id = outer_id;
}

void WM::on_destroy_window(Handle window_id)
{
    auto ow = windows_.find(window_id);
    if (ow) {
        graph_->destroy_window(*ow);
        windows_.remove(window_id);
    }
}

void WM::on_expose_window(Handle window_id, Area area)
{

}

Point WM::window_starting_pos(Window const& w, Area const& window_sz, Area const& scr_sz, Padding const& pad) const
{
    WindowStartingPos wsp = lib_.theme().window_starting_pos(w);
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

