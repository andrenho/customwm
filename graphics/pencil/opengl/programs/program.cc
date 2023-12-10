#include "program.hh"

#include <stdexcept>
#include <string>

using namespace std::string_literals;

#include "util/log.hh"

void Program::use() const
{
    glUseProgram(program_);
}

Program::~Program()
{
    if (program_ != 0) {
        glDeleteBuffers(1, &vbo_);
        glDeleteVertexArrays(1, &vao_);
        glDeleteProgram(program_);
    }
}

void Program::upload()
{
    auto compile_shader = [this](GLuint shader_type, const char* shader_source) -> GLuint {
        GLuint shader = glCreateShader(shader_type);
        glShaderSource(shader, 1, &shader_source, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar info[512];
            glGetShaderInfoLog(shader, sizeof(info), nullptr, info);
            glDeleteShader(shader);
            throw std::runtime_error("Shader "s + name() + " compilation failed: " + info);
        }

        return shader;
    };

    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader());
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader());

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
        throw std::runtime_error("Error linking shader "s + name() + ": " + info);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    debug("`%s` program compiled", name());

    glUseProgram(program);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    program_ = program;
}
