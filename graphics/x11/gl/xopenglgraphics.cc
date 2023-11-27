#include "xopenglgraphics.hh"
#include "graphics/pencil/opengl/openglpencil.hh"

typedef GLXContext (*glXCreateContextAttribsARBProc)
        (Display*, GLXFBConfig, GLXContext, Bool, const int*);

static int visual_attribs[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, true,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        None
};

static int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 2,
        None
};

void XOpenGLGraphics::init()
{
    XGraphics::init();

    int num_fbc = 0;
    GLXFBConfig *fbc = glXChooseFBConfig(display, screen, visual_attribs, &num_fbc);
    if (!fbc)
        throw std::runtime_error("glXChooseFBConfig() failed");

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    if (!glXCreateContextAttribsARB)
        throw std::runtime_error("glXCreateContextAttribsARB() not found");

    context_ = glXCreateContextAttribsARB(display, fbc[0], nullptr, true, context_attribs);
    if (!context_)
        throw std::runtime_error("Failed to create OpenGL context. Exiting.");

    glXMakeCurrent(display, root, context_);

    opengl_manager_.init();
}

XOpenGLGraphics::~XOpenGLGraphics()
{
    if (context_)
        glXDestroyContext(display, context_);
}

std::unique_ptr<Pencil> XOpenGLGraphics::create_pencil(Window_* window)
{
    return std::make_unique<OpenGLPencil>(window, this, &opengl_manager_);
}

void XOpenGLGraphics::paint(WindowHandle window, std::function<void()> paint_function)
{
    glXMakeCurrent(display, window, context_);

    if (!printed_opengl_info_) {
        opengl_manager_.print_info();
        printed_opengl_info_ = true;
    }

    paint_function();

    glXSwapBuffers(display, window);
}
