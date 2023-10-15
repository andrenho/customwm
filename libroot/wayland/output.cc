#include "output.hh"

#include <ctime>
#include <cstdio>

#include <stdexcept>

Output::Output(wlr_output* output, wlr_renderer* renderer, wlr_allocator* allocator)
    : wlr_output_(output), renderer_(renderer), allocator_(allocator)
{
    clock_gettime(CLOCK_MONOTONIC, &last_frame_);

    wlr_output_init_render(wlr_output_, allocator_, renderer_);

    frame_listener_.notify = [](wl_listener* listener, [[maybe_unused]] void* data) {
        Output* output = wl_container_of(listener, output, frame_listener_);
        output->on_frame();
    };
    wl_signal_add(&wlr_output_->events.frame, &frame_listener_);

    wlr_output_state state {};
    wlr_output_state_init(&state);
    wlr_output_state_set_enabled(&state, true);
    struct wlr_output_mode *mode = wlr_output_preferred_mode(output);
    if (mode != nullptr) {
        wlr_output_state_set_mode(&state, mode);
    }
    wlr_output_commit_state(output, &state);
}

void Output::on_frame()
{

    wlr_output_attach_render(wlr_output_, nullptr);

    if (!wlr_renderer_begin(renderer_, 200, 200))
        throw std::runtime_error("Can't start rendering");

    float color[4] = {1.0, 0, 0, 1.0 };
    wlr_renderer_clear(renderer_, color);

    pixman_region32_t r;
    pixman_region32_init_rect(&r, 0, 0, 200, 200);
    wlr_output_set_damage(wlr_output_, &r);
    wlr_renderer_end(renderer_);

    wlr_output_commit(wlr_output_);
}
