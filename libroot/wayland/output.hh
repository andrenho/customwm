#ifndef OUTPUT_HH_
#define OUTPUT_HH_

extern "C" {
#include <wlr/types/wlr_output.h>
#define static
#include <wlr/render/wlr_renderer.h>
#undef static
}

class Output {
public:
    explicit Output(wlr_output* output, wlr_renderer* renderer, wlr_allocator* allocator);

private:
    wlr_output    *wlr_output_;
    wlr_renderer  *renderer_;
    wlr_allocator *allocator_;
    timespec      last_frame_ = { 0, 0 };

    wl_listener frame_listener_;
};

#endif //OUTPUT_HH_
