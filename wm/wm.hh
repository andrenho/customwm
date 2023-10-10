#ifndef WM_HH_
#define WM_HH_

#include <unordered_map>
#include <string>
#include <xcb/xcb.h>
#include <xcb/xcb_errors.h>

#include "../libtheme/theme.hh"
#include "../libtheme/types/types.hh"
#include "window.hh"
#include "../libtheme/types/resources.hh"
#include "../libresource/resourcemanager.hh"

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
    xcb_connection_t*     dpy = nullptr;
    xcb_screen_t*         scr = nullptr;
    xcb_errors_context_t* err_ctx;
    Theme&                theme_;
    mutable uint16_t      cascade_ = 0;     // used to calculate the position of the next window to be open
    std::unordered_map<uint32_t, std::unique_ptr<Window>> windows_;
    std::unique_ptr<ResourceManager> resources_;

    void on_map_request(xcb_map_request_event_t *e);
    void on_unmap_notify(xcb_unmap_notify_event_t *e);
    void on_expose(xcb_expose_event_t *e);

    std::pair<int16_t, int16_t> calculate_starting_position(WindowStartingPos const &pos, xcb_get_geometry_reply_t *geo);

    void load_image(std::string const& name, ImageResource const &df);
};


#endif //WM_HH_
