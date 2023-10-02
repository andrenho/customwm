#ifndef BRUSH_HH_
#define BRUSH_HH_

#include "../../lib/window.hh"

class Brush : public IBrush {
public:
    explicit Brush(Handle outer_w) : outer_w_(outer_w) {}

    void set_color(uint8_t r, uint8_t g, uint8_t b) override;
    void fill_rectangle(Point p1, Point p2) override;

private:
    Handle outer_w_;
};


#endif //BRUSH_HH_
