#ifndef SERVER_WAYLAND_HH_
#define SERVER_WAYLAND_HH_

#include <optional>
#include <string>

#include "../server.hh"

class ServerWayland : public Server {
public:
    explicit ServerWayland(std::optional<std::string> const& display);
    ~ServerWayland() override;

    void setup_event_listeners() override;
    void capture_existing_windows() override;
    void run_event_loop() override;

private:
    struct wl_display*    wl_display = nullptr;
    struct wl_event_loop* wl_event_loop = nullptr;
    struct wlr_backend*   backend = nullptr;
};

#endif //SERVER_WAYLAND_HH_
