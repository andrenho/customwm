#ifndef WM_EVENTS_HH_
#define WM_EVENTS_HH_

#include "wmtypes.hh"

class WM_Events {
public:
    virtual void on_create_window(Handle window_id) = 0;
    virtual void on_destroy_window(Handle window_id) = 0;
    virtual void on_expose_window(Handle window_id, Area area) = 0;
};

#endif //WM_EVENTS_HH_
