#include "openglpencil.hh"

#include <GL/gl.h>
#include <GL/glx.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "window/window.hh"

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

    // vertex parameters

    float x = 10.f, y = 10.f, w = 100.f, h = 100.f;
    float vertices[] = {
            x, y,
            x + w, y,
            x + w, y + h,
            x, y + h
    };

    glBindBuffer(GL_ARRAY_BUFFER, opengl_manager_->fill().vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glm::mat4 projection = glm::ortho(0.f, (float) window_->rectangle().w, (float) window_->rectangle().h, 0.f);
    glUniformMatrix4fv(opengl_manager_->fill().projectionLocation, 1, GL_FALSE, &projection[0][0]);

    // fragment parameters

    glUniform4fv(opengl_manager_->fill().bgColorLocation, 1, glcolor);
    glBindVertexArray(opengl_manager_->fill().vao);

    // draw

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
}