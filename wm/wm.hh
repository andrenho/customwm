#ifndef WM_HH_
#define WM_HH_

#include <unordered_map>
#include <string>
#include <xcb/xcb.h>

#include "../libtheme/theme.hh"
#include "../libtheme/types/types.hh"
#include "window.hh"
#include "../libtheme/types/datafile.hh"

class WM {
public:
    WM(std::string const& display, Theme& theme);
    ~WM();

    WM(WM const&) = delete;
    WM& operator=(WM const&) = delete;
    WM(WM&&) = default;
    WM& operator=(WM&&) = delete;

    void run();

private:
    xcb_connection_t* dpy = nullptr;
    xcb_screen_t*     scr = nullptr;
    Theme&            theme_;
    mutable uint16_t  cascade_ = 0;     // used to calculate the position of the next window to be open
    std::unordered_map<uint32_t, std::unique_ptr<Window>> windows_;
    std::unordered_map<std::string, xcb_pixmap_t> pixmaps_;

    void on_map_request(xcb_map_request_event_t *e);
    void on_unmap_notify(xcb_unmap_notify_event_t *e);
    void on_expose(xcb_expose_event_t *e);

    std::pair<int16_t, int16_t> calculate_starting_position(WindowStartingPos const &pos, xcb_get_geometry_reply_t *geo);

    void load_image(std::string const& name, DataFile const &df);
};


#endif //WM_HH_
