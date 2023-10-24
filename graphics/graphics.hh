#ifndef GRAPHICS_HH_
#define GRAPHICS_HH_

#include <memory>
#include "wm.hh"

class Graphics {
public:
    virtual ~Graphics() = default;

    WM* create_wm();

    virtual std::unique_ptr<WM> create_wm_() = 0;

protected:
    std::unique_ptr<WM> wm_;
};

#endif //GRAPHICS_HH_
