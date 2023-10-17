#include "server_wayland.hh"

#include <stdexcept>
#include <csignal>

extern "C" {
#include <wayland-server.h>
#include <wlr/util/log.h>
#include <wlr/backend.h>
#include <wlr/types/wlr_output.h>
}

ServerWayland::ServerWayland()
{
    wlr_log_init(WLR_DEBUG, nullptr);

    // initialize display
    display_ = wl_display_create();
    if (!display_)
        throw std::runtime_error("Error initializing display.");

    event_loop_ = wl_display_get_event_loop(display_);
    if (!event_loop_)
        throw std::runtime_error("Error initializing event loop.");

    backend_ = wlr_backend_autocreate(display_, nullptr);
    if (!backend_)
        throw std::runtime_error("Failed to create backend.");

    renderer_ = wlr_renderer_autocreate(backend_);
    if (!renderer_)
        throw std::runtime_error("Failed to create renderer.");

    allocator_ = wlr_allocator_autocreate(backend_, renderer_);
    if (!allocator_)
        throw std::runtime_error("Failed to create allocator.");
}

ServerWayland::~ServerWayland()
{
    wl_display_destroy(display_);
}

void ServerWayland::setup_event_listeners()
{
    new_output_listener_.notify = [](wl_listener* listener, void* data) {
        ServerWayland* server = wl_container_of(listener, server, new_output_listener_);
        server->on_new_output((wlr_output*) data);
    };
    wl_signal_add(&backend_->events.new_output, &new_output_listener_);
}

void ServerWayland::run_event_loop()
{
    // start backend
    if (!wlr_backend_start(backend_))
        throw std::runtime_error("Failed to start Wayland backend.");

    wl_display_run(display_);
}

void ServerWayland::on_new_output(wlr_output* output)
{
    if (!wl_list_empty(&output->modes)) {
        wlr_output_mode* mode = wl_container_of(output->modes.prev, mode, link);
        wlr_output_set_mode(output, mode);
    }

    outputs_.emplace_back(output, renderer_, allocator_);
}