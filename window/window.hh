#ifndef WINDOW_HH_
#define WINDOW_HH_

#include "theme/types/types.hh"
#include "graphics/whandle.hh"

class Window_ {
public:
    Window_(class Graphics* graphics, Rectangle const& rectangle);
    virtual ~Window_();

    WindowHandle handle() const { return handle_; }

protected:
    class Graphics* graphics_;

    WindowHandle handle_;
};

#endif //WINDOW_HH_
