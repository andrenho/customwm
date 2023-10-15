#include "server_wayland.hh"

#include <stdexcept>

extern "C" {
#include <wayland-server.h>
#include <wlr/backend.h>
}

ServerWayland::ServerWayland(std::optional<std::string> const& display)
{
    wl_display = wl_display_create();
    wl_event_loop = wl_display_get_event_loop(wl_display);
    backend = wlr_backend_autocreate(wl_display);

    if (!wlr_backend_start(backend))
        throw std::runtime_error("Failed to start Wayland backend.");
}

ServerWayland::~ServerWayland()
{
    wl_display_destroy(wl_display);
}

void ServerWayland::setup_event_listeners()
{

}

void ServerWayland::capture_existing_windows()
{

}

void ServerWayland::run_event_loop()
{
    wl_display_run(wl_display);
}