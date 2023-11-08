#ifndef GRAPHICS_HH_
#define GRAPHICS_HH_

#include <optional>
#include <string>
#include "wm.hh"

struct Graphics {
    virtual ~Graphics() = default;
    virtual std::unique_ptr<WM> create_wm() = 0;

    static Graphics* create(std::optional<std::string> const& display);

protected:
    Graphics() = default;

    static std::unique_ptr<Graphics> graphics_;
};

#endif //GRAPHICS_HH_
