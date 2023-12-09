#include "openglpencil.hh"

#include <GL/gl.h>
#include <GL/glx.h>

static float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
};


void OpenGLPencil::on_expose([[maybe_unused]] Rectangle const& rectangle) const
{
}

void OpenGLPencil::fill(Color const& color, [[maybe_unused]] Rectangle const& rect) const
{
    // TODO - remove this
    glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat glcolor[] = {
            ((float) color.r) / 255.0f,
            ((float) color.g) / 255.0f,
            ((float) color.b) / 255.0f,
            ((float) color.a) / 255.0f,
    };

    glUseProgram(opengl_manager_->fill().program);
    glUniform4fv(opengl_manager_->fill().bgColorLocation, 1, glcolor);
    glBindVertexArray(opengl_manager_->fill().vao);

    glBindBuffer(GL_ARRAY_BUFFER, opengl_manager_->fill().vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}