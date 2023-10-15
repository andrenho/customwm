#ifndef SERVER_WAYLAND_HH_
#define SERVER_WAYLAND_HH_

#include <optional>
#include <string>
#include <vector>

extern "C" {
#include <wayland-util.h>
#include <wayland-server.h>
#include <wlr/types/wlr_output.h>
#include <wlr/render/allocator.h>
#define static
#include <wlr/render/wlr_renderer.h>
#undef static
}

#include "../server.hh"
#include "output.hh"

class ServerWayland : public Server {
public:
    ServerWayland();
    ~ServerWayland() override;

    void setup_event_listeners() override;
    void capture_existing_windows() override { /* wayland compositors always start empty (?) */ }
    void run_event_loop() override;

private:
    wl_display*    display_    = nullptr;
    wl_event_loop* event_loop_ = nullptr;
    wlr_backend*   backend_    = nullptr;
    wlr_renderer*  renderer_   = nullptr;
    wlr_allocator* allocator_  = nullptr;

    std::vector<Output> outputs_;

    // event listeners
    wl_listener new_output_listener_;

    void new_output_event(wlr_output* output);
};

#endif //SERVER_WAYLAND_HH_
