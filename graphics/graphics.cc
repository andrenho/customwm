#include "graphics.hh"

WM *Graphics::create_wm()
{
    wm_ = create_wm_();
    return wm_.get();
}
