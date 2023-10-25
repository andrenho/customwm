#ifndef GRAPHICS_HH_
#define GRAPHICS_HH_

#include <memory>
#include "wm.hh"
#include "theme/theme.hh"

class Graphics {
public:
    Graphics(Theme& theme) : theme_(theme) {}
    virtual ~Graphics() = default;

    WM* create_wm();

    virtual std::unique_ptr<WM> create_wm_() = 0;

protected:
    Theme& theme_;
    std::unique_ptr<WM> wm_;
};

#endif //GRAPHICS_HH_
