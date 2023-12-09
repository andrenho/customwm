#ifndef OPENGLPENCIL_HH_
#define OPENGLPENCIL_HH_

#include "graphics/pencil/pencil.hh"
#include "openglmanager.hh"

class OpenGLPencil : public Pencil {
public:
    OpenGLPencil(class Window_* window, class Graphics const* graphics, OpenGLManager* opengl_manager)
            : Pencil(window, graphics), window_(window), opengl_manager_(opengl_manager) {}

    void on_expose(Rectangle const& rectangle) const override;
    void fill(Color const& color, Rectangle const& rect) const override;

private:
    Window_* window_;
    OpenGLManager* opengl_manager_;
};

#endif //OPENGLPENCIL_HH_
