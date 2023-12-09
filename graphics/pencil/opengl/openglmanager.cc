#include "openglmanager.hh"

#include <cstdio>
#include <stdexcept>
#include <string>

using namespace std::string_literals;

#include "shaders.hh"
#include "util/log.hh"

void OpenGLManager::init()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
        throw std::runtime_error("Error: "s + (const char *) glewGetErrorString(err));
    info("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    init_fill();
}

void OpenGLManager::init_fill()
{
    fill_.program = compile("fill", vertex_shader_fill, fragment_shader_fill);

    glUseProgram(fill_.program);
    fill_.bgColorLocation = glGetUniformLocation(fill_.program, "bgColor");

    glGenVertexArrays(1, &fill_.vao);
    glBindVertexArray(fill_.vao);

    glGenBuffers(1, &fill_.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, fill_.vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
}

OpenGLManager::~OpenGLManager()
{
    // TODO - clear everything
}


void OpenGLManager::print_info()
{
    int major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    info("OpenGL version %d.%d  Vendor %s  Renderer %s",
           major, minor, glGetString(GL_VENDOR), glGetString(GL_RENDERER));
}

GLuint OpenGLManager::compile(const char* name, char const* vertex_shader, char const* fragment_shader)
{
    auto compile_shader = [&name](GLuint shader_type, const char* shader_source) -> GLuint {
        GLuint shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &shader_source, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar info[512];
            glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
            glDeleteShader(shader);
            throw std::runtime_error("Shader "s + name + " compilation failed: " + info);
        }

        return shader;
    };

    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader_fill);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_fill);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetProgramInfoLog(program, sizeof(info), nullptr, info);
        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(program);
        throw std::runtime_error("Error linking shader "s + name + ": " + info);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    debug("`%s` program compiled", name);

    return program;
}

