#include "wm.hh"

#include "window.hh"

WM::WM(std::string const& display, Theme& theme)
    : theme_(theme)
{
    // setup connection
    dpy = xcb_connect(display.c_str(), nullptr);
    if (xcb_connection_has_error(dpy))
        throw std::runtime_error("Could not connect to display " + display);

    // get screen
    scr = xcb_setup_roots_iterator(xcb_get_setup(dpy)).data;

    // load images
    resources_ = std::make_unique<ResourceManager>(dpy, scr);
    resources_->load_resources(theme_);

    // select event filter
    uint32_t values = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    xcb_change_window_attributes_checked(dpy, scr->root, XCB_CW_EVENT_MASK, &values);
    xcb_flush(dpy);
}

WM::~WM()
{
    resources_.reset();
    xcb_disconnect(dpy);
}

void WM::run()
{
    while (!xcb_connection_has_error(dpy)) {
        xcb_generic_event_t *ev = xcb_wait_for_event(dpy);
        if (ev != nullptr) {
            switch (ev->response_type & ~0x80) {
                case XCB_MAP_REQUEST: {
                    auto* e = reinterpret_cast<xcb_map_request_event_t *>(ev);
                    on_map_request(e);
                    break;
                }
                case XCB_UNMAP_NOTIFY: {
                    auto* e = reinterpret_cast<xcb_unmap_notify_event_t *>(ev);
                    on_unmap_notify(e);
                    break;
                }
                case XCB_EXPOSE: {
                    auto* e = reinterpret_cast<xcb_expose_event_t *>(ev);
                    on_expose(e);
                    break;
                }
            }
            free(ev);
        }
        xcb_flush(dpy);
    }
}

void WM::on_map_request(xcb_map_request_event_t *e)
{
    // figure out information about the window
    auto geo = xcb_get_geometry_reply(dpy, xcb_get_geometry(dpy, e->window), nullptr);

    // get configuration from theme
    Rectangle child_sz { geo->x, geo->y, geo->width, geo->height };
    Padding padding = theme_.read<Padding>("window.padding", child_sz);
    WindowStartingPos wps = theme_.read<WindowStartingPos>("window.starting_pos", child_sz);
    auto [x, y] = calculate_starting_position(wps, geo);

    // calculate outer window size
    uint16_t w = geo->width + padding.left + padding.right + 1;
    uint16_t h = geo->height + padding.top + padding.bottom + 1;

    // add window to list
    auto window = std::make_unique<Window>(dpy, scr, Rectangle { x, y, w, h }, e->window,
                                           Point { padding.left, padding.top }, resources_.get());
    windows_.emplace(std::pair<uint32_t, std::unique_ptr<Window>> { window->id, std::move(window) });

    // cleanup
    free(geo);
}

void WM::on_unmap_notify(xcb_unmap_notify_event_t *e)
{
    for (auto& kv : windows_) {
        Window const* w = kv.second.get();
        if (w->child_id == e->window) {
            windows_.erase(w->id);
            break;
        }
    }
}

void WM::on_expose(xcb_expose_event_t *e)
{
    try {
        IWindow* iwindow = windows_.at(e->window).get();
        Rectangle r { (int16_t) e->x, int16_t (e->y) , e->width, e->height };
        theme_.call("window.on_expose", iwindow, r);
    } catch (PropertyNotFoundException& unused) {
    } catch (std::out_of_range& unused) {}
}

std::pair<int16_t, int16_t> WM::calculate_starting_position(WindowStartingPos const &wsp, xcb_get_geometry_reply_t *geo)
{
    switch (wsp.starting_pos) {
        case WindowStartingPos::Cascade: {
            int16_t x = cascade_ * 24;
            int16_t y = cascade_ * 24;
            ++cascade_;
            if (cascade_ == 5)
                cascade_ = 0;
            return { x, y };
        }
        case WindowStartingPos::Center:
            return { (scr->width_in_pixels / 2) - (geo->width / 2), (scr->height_in_pixels / 2) - (geo->height / 2) };
        case WindowStartingPos::Random:
            return { rand() % (scr->width_in_pixels / 3), rand() % (scr->height_in_pixels / 3) };
        case WindowStartingPos::Maximized:
            throw std::runtime_error("Not implemented");  // TODO
        case WindowStartingPos::Requested:
            return { geo->x, geo->y };
        case WindowStartingPos::Custom:
            return { wsp.point.x, wsp.point.y };
    }
}
