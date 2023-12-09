#ifndef SHADERS_HH_
#define SHADERS_HH_

const char* vertex_shader_fill = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }
)";

const char* fragment_shader_fill = R"(
    #version 330 core
    out vec4 FragColor;

    uniform vec4 bgColor;

    void main()
    {
        FragColor = bgColor;
    }
)";

#endif //SHADERS_HH_
