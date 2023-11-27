#ifndef XOPENGLGRAPHICS_HH_
#define XOPENGLGRAPHICS_HH_

#include <GL/gl.h>
#include <GL/glx.h>

#include "graphics/x11/xgraphics.hh"
#include "window/window.hh"
#include "graphics/pencil/opengl/openglmanager.hh"

class XOpenGLGraphics : public XGraphics {
public:
    XOpenGLGraphics(class Options *options, class Theme* theme) : XGraphics(options, theme) {}
    ~XOpenGLGraphics() override;

    void init() override;
    void select_window_for_drawing(WindowHandle window) override;

    std::unique_ptr<Pencil> create_pencil(Window_ *window) override;

private:
    GLXContext context_ = nullptr;
    OpenGLManager opengl_manager_;
    bool          printed_opengl_info_ = false;
};

#endif //XOPENGLGRAPHICS_HH_
