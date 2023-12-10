#include "fillprogram.hh"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void FillProgram::upload()
{
    Program::upload();

    projection_loc = glGetUniformLocation(program_, "projection");
    bg_color_loc = glGetUniformLocation(program_, "bgColor");
}


void FillProgram::draw(Size const& viewport_sz, Color const& color, Rectangle const& rect) const
{
    Program::use();

    // vertex parameters

    float vertices[] = {
            (float) rect.x, (float) rect.y,
            (float) rect.x + (float) rect.w, (float) rect.y,
            (float) rect.x + (float) rect.w, (float) rect.y + (float) rect.h,
            (float) rect.x, (float) rect.y + (float) rect.h
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glm::mat4 projection = glm::ortho(0.f, (float) viewport_sz.w, (float) viewport_sz.h, 0.f);
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection[0][0]);

    // fragment parameters

    GLfloat glcolor[] = {
            ((float) color.r) / 255.0f,
            ((float) color.g) / 255.0f,
            ((float) color.b) / 255.0f,
            ((float) color.a) / 255.0f,
    };
    glUniform4fv(bg_color_loc, 1, glcolor);

    glBindVertexArray(vao_);

    // draw

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
}

const char *FillProgram::vertex_shader() const
{
    return R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * vec4(aPos, 1.0);
        }
    )";
}

const char *FillProgram::fragment_shader() const
{
    return R"(
        #version 330 core
        out vec4 FragColor;

        uniform vec4 bgColor;

        void main()
        {
            FragColor = bgColor;
        }
    )";
}