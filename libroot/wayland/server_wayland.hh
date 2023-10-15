#ifndef SERVER_WAYLAND_HH_
#define SERVER_WAYLAND_HH_

#include "../server.hh"

class ServerWayland : public Server {
public:
    void setup_event_listeners() override;
    void capture_existing_windows() override;
    void run_event_loop() override;
};

#endif //SERVER_WAYLAND_HH_
