#ifndef OPENGLMANAGER_HH_
#define OPENGLMANAGER_HH_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>

class OpenGLManager {
public:
    void init();
    void print_info();

    GLuint fill_program() const { return fill_program_; }

private:
    GLuint fill_program_ = 0;

    GLuint compile(char const* vertex_shader, char const* fragment_shader);
};

#endif //OPENGLMANAGER_HH_
