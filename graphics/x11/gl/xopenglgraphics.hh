#ifndef XOPENGLGRAPHICS_HH_
#define XOPENGLGRAPHICS_HH_

#include <GL/gl.h>
#include <GL/glx.h>

#include "graphics/x11/xgraphics.hh"

class XOpenGLGraphics : public XGraphics {
public:
    XOpenGLGraphics(class Options *options, class Theme* theme) : XGraphics(options, theme) {}

    WindowHandle create_window(Rectangle const& rectangle) override;

    void destroy_window(WindowHandle window) override;

private:
    std::unordered_map<WindowHandle, GLXContext> contexts_;
};

#endif //XOPENGLGRAPHICS_HH_
