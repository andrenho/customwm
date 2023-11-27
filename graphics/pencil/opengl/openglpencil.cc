#include "openglpencil.hh"

#include <GL/gl.h>
#include <GL/glx.h>

void OpenGLPencil::on_expose(Rectangle const& rectangle) const
{
}

void OpenGLPencil::fill(Color const& color, Rectangle const& rect) const
{
    float r = ((float) color.r) / 255.0f;
    float g = ((float) color.g) / 255.0f;
    float b = ((float) color.b) / 255.0f;
    float a = ((float) color.a) / 255.0f;

    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}