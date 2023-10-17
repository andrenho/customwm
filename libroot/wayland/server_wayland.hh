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
#include <wlr/render/wlr_renderer.h>
}

#include "../server.hh"
#include "output.hh"

class ServerWayland : public Server {
public:
    ServerWayland(Engine& engine);
    ~ServerWayland() override;

    void run() override;

private:
    wl_display*    display_    = nullptr;
    wl_event_loop* event_loop_ = nullptr;
    wlr_backend*   backend_    = nullptr;
    wlr_renderer*  renderer_   = nullptr;
    wlr_allocator* allocator_  = nullptr;

    std::vector<Output> outputs_;

    // event listeners
    wl_listener new_output_listener_;

    void on_new_output(wlr_output* output);

    void setup_event_listeners();
    void run_event_loop();
};

#endif //SERVER_WAYLAND_HH_
