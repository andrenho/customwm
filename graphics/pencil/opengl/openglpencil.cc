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
    opengl_manager_->fill().draw(window_->rectangle().size(), color, rect);
}