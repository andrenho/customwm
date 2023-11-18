#ifndef GRAPHICS_HH_
#define GRAPHICS_HH_

#include <string>
#include "event.hh"

class Graphics {
public:
    explicit Graphics(class Options* options) : options_(options) {}
    virtual ~Graphics() = default;

    virtual void init() = 0;
    virtual std::string interface_name() const = 0;

    virtual void subscribe_to_wm_events() = 0;

    virtual std::optional<Event> next_event() = 0;

protected:
    class Options* options_;
};

#endif //GRAPHICS_HH_
