#ifndef ROOT_HH_
#define ROOT_HH_

#include <optional>
#include <string>
#include "eventlistener.hh"

class Root {
public:
    Root([[maybe_unused]] std::optional<std::string> const& display) {}
    virtual ~Root() = default;

    virtual std::string interface_name() const = 0;

    virtual void setup_event_listeners(EventListener &event_listener) { event_listener_ = &event_listener; }
    virtual void capture_existing_windows() = 0;
    virtual void run_event_loop() = 0;

protected:
    EventListener* event_listener_;
};

#endif //ROOT_HH_
