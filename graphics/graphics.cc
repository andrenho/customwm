#include "graphics.hh"

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

#include "graphics/x11/xgraphics.hh"
#include "graphics/dummy/dgraphics.hh"
#include "graphics/x11/gl/xopenglgraphics.hh"
#include "graphics/x11/xlib/xlibgraphics.hh"
#include "util/log.hh"

std::unique_ptr<Graphics> Graphics::create_unique_ptr(Options* options, [[maybe_unused]] class Theme* theme)
{
    switch (options->backend) {
        case Options::Backend::Dummy:
            return std::make_unique<DGraphics>(options);

        case Options::Backend::X11:
#ifdef WITH_X11
            if (options->render == Options::Render::OpenGL) {
#  ifdef WITH_OPENGL
                return std::make_unique<XOpenGLGraphics>(options, theme);
#  else
                error("Application compiled without OpenGL support");
                exit(EXIT_FAILURE);
#  endif
            } else {
                return std::make_unique<XlibGraphics>(options, theme);
            }
#else
            error("Application compiled without Xlib support");
            exit(EXIT_FAILURE);
#endif

        case Options::Backend::Wayland:
#ifdef WITH_WAYLAND
            error("sorry: wayland not supported yet");
            exit(EXIT_FAILURE);
#else
            error("Application compiled without Wayland support.");
            exit(EXIT_FAILURE);
#endif
    }
    throw std::runtime_error("Invalid option");
}
