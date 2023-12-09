#ifndef OPENGLMANAGER_HH_
#define OPENGLMANAGER_HH_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "graphics/pencil/opengl/programs/fillprogram.hh"

class OpenGLManager {
public:
    void init();
    void print_info();

    FillProgram const& fill() const { return fill_; }

private:
    FillProgram fill_;
};

#endif //OPENGLMANAGER_HH_
