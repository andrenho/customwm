#include "xopenglgraphics.hh"

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

WindowHandle XOpenGLGraphics::create_window(Rectangle const &rectangle)
{
    WindowHandle window = XGraphics::create_window(rectangle);

    int num_fbc = 0;
    GLXFBConfig *fbc = glXChooseFBConfig(display, screen, visual_attribs, &num_fbc);
    if (!fbc)
        throw std::runtime_error("glXChooseFBConfig() failed");

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = nullptr;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    if (!glXCreateContextAttribsARB)
        throw std::runtime_error("glXCreateContextAttribsARB() not found");

    GLXContext context = glXCreateContextAttribsARB(display, fbc[0], nullptr, true, context_attribs);
    if (!context)
        throw std::runtime_error("Failed to create OpenGL context. Exiting.");

    contexts_.emplace(window, context);

    glXMakeCurrent(display, window, context);

    int major = 0, minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    printf("OpenGL context created.\nVersion %d.%d\nVendor %s\nRenderer %s\n",
           major, minor,
           glGetString(GL_VENDOR),
           glGetString(GL_RENDERER));

    return window;
}

void XOpenGLGraphics::destroy_window(WindowHandle window)
{
    try {
        glXDestroyContext(display, contexts_.at(window));
        contexts_.erase(window);
    } catch (std::out_of_range&) {}
    XGraphics::destroy_window(window);
}