#ifndef PENCIL_HH_
#define PENCIL_HH_

#include <optional>

#include "theme/types/types.hh"

class Pencil {
public:
    Pencil(class Window_* window, class Graphics const* graphics) : window_(window), graphics_(graphics) {}
    virtual ~Pencil() = default;

    virtual void init() {}

    virtual void on_expose(Rectangle const& rectangle) const = 0;
    virtual void fill(Color const& color, Rectangle const& rect) const = 0;

protected:
    class Window_*  window_;
    class Graphics const* graphics_;
};

#endif //PENCIL_HH_
