#ifndef GRAPHICLIBRARY_HH_
#define GRAPHICLIBRARY_HH_

#include "../wm/wm_events.hh"
#include "../../lib/types.hh"

class GraphicLibrary {
public:
    virtual ~GraphicLibrary() = default;

    virtual void setup(void* data) = 0;

    virtual bool running() const = 0;
    virtual void do_events(WM_Events* events) = 0;

    virtual Area inner_window_size(const Window &w) const = 0;
    virtual Area screen_size() const = 0;

    virtual Handle reparent_window(Window const& w, Point const& pos, Area const& window_sz, Padding const& padding) = 0;
    virtual void   destroy_window(Window const& w) = 0;
};

#endif //GRAPHICLIBRARY_HH_
