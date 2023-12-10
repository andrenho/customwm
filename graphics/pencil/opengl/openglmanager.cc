#include "openglmanager.hh"

#include <cstdio>
#include <stdexcept>
#include <string>

using namespace std::string_literals;

#include "util/log.hh"

void OpenGLManager::init()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
        throw std::runtime_error("Error: "s + (const char *) glewGetErrorString(err));
    info("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    fill_.upload();
}

void OpenGLManager::print_info()
{
    int major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    info("OpenGL version %d.%d  Vendor %s  Renderer %s",
           major, minor, glGetString(GL_VENDOR), glGetString(GL_RENDERER));
}