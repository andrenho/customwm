#ifndef XGLGRAPHICS_HH_
#define XGLGRAPHICS_HH_

#include "graphics/x11/xgraphics.hh"

class XGLGraphics : public XGraphics {
public:
    XGLGraphics(class Options *options, class Theme* theme) : XGraphics(options, theme) {}

    WindowHandle create_window(Rectangle const& rectangle) override;
};

#endif //XGLGRAPHICS_HH_
