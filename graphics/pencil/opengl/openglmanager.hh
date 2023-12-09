#ifndef OPENGLMANAGER_HH_
#define OPENGLMANAGER_HH_

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>

struct FillProgram {
    GLuint vao;
    GLuint vbo;
    GLuint program;
    GLint  projectionLocation;
    GLint  bgColorLocation;
};

class OpenGLManager {
public:
    ~OpenGLManager();

    void init();
    void print_info();

    FillProgram const& fill() const { return fill_; }

private:
    FillProgram fill_;

    GLuint compile(const char* name, char const* vertex_shader, char const* fragment_shader);

    void init_fill();
};

#endif //OPENGLMANAGER_HH_
