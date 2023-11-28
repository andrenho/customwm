#include "openglpencil.hh"

#include <GL/gl.h>
#include <GL/glx.h>

void OpenGLPencil::on_expose(Rectangle const& rectangle) const
{
}

void OpenGLPencil::fill(Color const& color, Rectangle const& rect) const
{
    GLfloat glcolor[] = {
            ((float) color.r) / 255.0f,
            ((float) color.g) / 255.0f,
            ((float) color.b) / 255.0f,
            ((float) color.a) / 255.0f,
    };

    glUseProgram(opengl_manager_->fill().program);
    glUniform4fv(opengl_manager_->fill().bgColorLocation, 1, glcolor);
    glBindVertexArray(opengl_manager_->fill().vao);

    glDrawArrays(GL_QUADS, 0, 4);

    glBindVertexArray(0);

    /*
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     */
}