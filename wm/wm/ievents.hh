#ifndef IEVENTS_HH_
#define IEVENTS_HH_

#include "../../lib/types/types.hh"

class IEvents {
public:
    virtual void on_create_window(Handle window_id) = 0;
    virtual void on_destroy_window(Handle window_id) = 0;
    virtual void on_expose_window(Handle window_id, Area area) = 0;
};

#endif //IEVENTS_HH_
