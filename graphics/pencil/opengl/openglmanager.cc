#include <cstdio>
#include "openglmanager.hh"

void OpenGLManager::print_info()
{
    int major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("OpenGL info:\nVersion %d.%d\nVendor %s\nRenderer %s\n",
           major, minor,
           glGetString(GL_VENDOR),
           glGetString(GL_RENDERER));
}

