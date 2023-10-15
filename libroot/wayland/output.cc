#include "output.hh"

#include <ctime>
#include <cstdio>

Output::Output(wlr_output* output, wlr_renderer* renderer, wlr_allocator* allocator)
    : wlr_output_(output), renderer_(renderer), allocator_(allocator)
{
    clock_gettime(CLOCK_MONOTONIC, &last_frame_);

    frame_listener_.notify = [](wl_listener* listener, void* data) {
        Output* output = wl_container_of(listener, output, frame_listener_);
        wlr_output_init_render(output->wlr_output_, output->allocator_, output->renderer_);
        printf("X\n");
    };
    wl_signal_add(&wlr_output_->events.frame, &frame_listener_);
}