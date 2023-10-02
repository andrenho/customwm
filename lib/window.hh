#ifndef WINDOW_HH_
#define WINDOW_HH_

#include <cstdint>
#include <memory>

#include "types.hh"

struct IBrush {
    virtual ~IBrush() = default;
    virtual void set_color(uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void fill_rectangle(Point p1, Point p2) = 0;
};

struct Window {
    Handle outer_id;
    Handle inner_id;
    int w, h;
    std::unique_ptr<IBrush> brush = nullptr;
};

#endif //WINDOW_HH_
