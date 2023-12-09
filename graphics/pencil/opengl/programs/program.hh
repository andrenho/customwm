#ifndef CUSTOMWM_PROGRAM_HH
#define CUSTOMWM_PROGRAM_HH

#include <GL/glew.h>
#include <GL/gl.h>

class Program {
public:
    virtual ~Program();

    virtual void upload();

protected:
    virtual const char* name() const = 0;
    virtual const char* vertex_shader() const = 0;
    virtual const char* fragment_shader() const = 0;

    void use() const;

    GLuint program_ = 0;
    GLuint vao_ = 0;
    GLuint vbo_ = 0;
};

#endif //CUSTOMWM_PROGRAM_HH
